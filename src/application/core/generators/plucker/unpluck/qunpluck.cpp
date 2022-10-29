/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    Based on code written by Bill Janssen 2002

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "qunpluck.h"
#include <core/action.h>
#include <core/document.h>
#include <QAbstractTextDocumentLayout>
#include <QDateTime>
#include <QFile>
#include <QFont>
#include <QHash>
#include <QLabel>
#include <QStack>
#include <QString>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextFrame>
#include <QUrl>
#include "image.h"

#define GET_FUNCTION_CODE_TYPE(x) (((x) >> 3) & 0x1F)
#define GET_FUNCTION_CODE_DATALEN(x) ((x)&0x7)

#define CELLS(row, col) cells[row * cols + col]

#define READ_BIGENDIAN_SHORT(p) (((p)[0] << 8) | ((p)[1]))
#define READ_BIGENDIAN_LONG(p) \
    (((p)[0] << 24) | ((p)[1] << 16) | ((p)[2] << 8) | ((p)[3]))

/*
static void LinkRecords
    (
    char*  dir
    )
{
    RecordNode*  ptr;
    char*        realfilename;
    char*        linkname;

    realfilename = (char*)malloc (strlen (dir) + 20);
    linkname = (char*)malloc (strlen (dir) + 20);

    for (ptr = records; ptr != NULL; ptr = ptr->next) {
        if (ptr->page_id != ptr->index) {
            sprintf (realfilename, "%s/r%d.html", dir, ptr->page_id);
            sprintf (linkname, "%s/r%d.html", dir, ptr->index);
            link (realfilename, linkname);
        }
    }

    free (realfilename);
    free (linkname);
}
*/

class Context
{
public:
    int recordId;
    QTextDocument* document;
    QTextCursor* cursor;
    QStack<QTextCharFormat> stack;
    QList<int> images;

    QString linkUrl;
    int linkStart;
    int linkPage;
};

class RecordNode
{
public:
    int index;
    int page_id;
    bool done;
};

static Okular::DocumentViewport calculateViewport(QTextDocument* document,
                                                  const QTextBlock& block)
{
    if(!block.isValid())
    {
        return Okular::DocumentViewport();
    }

    const QRectF rect = document->documentLayout()->blockBoundingRect(block);
    const QSizeF size = document->size();

    int page = qRound(rect.y()) / qRound(size.height());

    Okular::DocumentViewport viewport(page);
    viewport.rePos.normalizedX = (double)rect.x() / (double)size.width();
    viewport.rePos.normalizedY = (double)rect.y() / (double)size.height();
    viewport.rePos.enabled = true;
    viewport.rePos.pos = Okular::DocumentViewport::Center;

    return viewport;
}

QUnpluck::QUnpluck() :
    mDocument(nullptr)
{
}

QUnpluck::~QUnpluck()
{
    mLinks.clear();
    mNamedTargets.clear();
    mPages.clear();
}

bool QUnpluck::open(const QString& fileName)
{
    mLinks.clear();
    mNamedTargets.clear();
    mPages.clear();

    mDocument = plkr_OpenDBFile(QFile::encodeName(fileName).data());
    if(!mDocument)
    {
        mErrorString = QObject::tr("Unable to open document");
        return false;
    }

    //     bool status = true;

    mInfo.insert(QStringLiteral("name"),
                 QString::fromLocal8Bit(plkr_GetName(mDocument)));
    mInfo.insert(QStringLiteral("title"),
                 QString::fromLocal8Bit(plkr_GetTitle(mDocument)));
    mInfo.insert(QStringLiteral("author"),
                 QString::fromLocal8Bit(plkr_GetAuthor(mDocument)));
    mInfo.insert(QStringLiteral("time"), QDateTime::fromSecsSinceEpoch(
                                             plkr_GetPublicationTime(mDocument))
                                             .toString());

    AddRecord(plkr_GetHomeRecordID(mDocument));

    int number = GetNextRecordNumber();
    while(number > 0)
    {
        /*status = */ TranscribeRecord(number);
        number = GetNextRecordNumber();
    }

    // Iterate over all records again to add those which aren't linked directly
    for(int i = 1; i < plkr_GetRecordCount(mDocument); ++i)
    {
        AddRecord(plkr_GetUidForIndex(mDocument, i));
    }

    number = GetNextRecordNumber();
    while(number > 0)
    {
        /*status = */ TranscribeRecord(number);
        number = GetNextRecordNumber();
    }

    for(int i = 0; i < mRecords.count(); ++i)
    {
        delete mRecords[i];
    }

    mRecords.clear();

    plkr_CloseDoc(mDocument);

    /**
     * Calculate hash map
     */
    QHash<int, int> pageHash;
    for(int i = 0; i < mContext.count(); ++i)
    {
        pageHash.insert(mContext[i]->recordId, i);
    }

    /**
     * Convert ids
     */
    for(int i = 0; i < mContext.count(); ++i)
    {
        Context* context = mContext[i];
        for(int j = 0; j < context->images.count(); ++j)
        {
            int imgNumber = context->images[j];
            context->document->addResource(
                QTextDocument::ImageResource,
                QUrl(QStringLiteral("%1.jpg").arg(imgNumber)),
                mImages[imgNumber]);
        }

        mPages.append(context->document);
    }
    qDeleteAll(mContext);
    mContext.clear();

    // convert record_id into page
    for(int i = 0; i < mLinks.count(); ++i)
    {
        mLinks[i].page = pageHash[mLinks[i].page];
        if(mLinks[i].url.startsWith(QLatin1String("page:")))
        {
            int page = mLinks[i].url.midRef(5).toInt();
            Okular::DocumentViewport viewport(pageHash[page]);
            viewport.rePos.normalizedX = 0;
            viewport.rePos.normalizedY = 0;
            viewport.rePos.enabled = true;
            viewport.rePos.pos = Okular::DocumentViewport::TopLeft;
            mLinks[i].link = new Okular::GotoAction(QString(), viewport);
        }
        else if(mLinks[i].url.startsWith(QLatin1String("para:")))
        {
            QPair<int, QTextBlock> data = mNamedTargets[mLinks[i].url];

            QTextDocument* document = mPages[mLinks[i].page];

            Okular::DocumentViewport viewport =
                calculateViewport(document, data.second);

            mLinks[i].link = new Okular::GotoAction(QString(), viewport);
        }
        else
        {
            mLinks[i].link = new Okular::BrowseAction(QUrl(mLinks[i].url));
        }
    }

    return true;
}

int QUnpluck::GetNextRecordNumber()
{
    int index = 0;

    for(int pos = 0; pos < mRecords.count(); ++pos)
    {
        if(!mRecords[pos]->done)
        {
            index = mRecords[pos]->index;
            break;
        }
    }

    return index;
}

int QUnpluck::GetPageID(int index)
{
    for(int pos = 0; pos < mRecords.count(); ++pos)
    {
        if(mRecords[pos]->index == index)
        {
            return mRecords[pos]->page_id;
        }
    }

    return 0;
}

void QUnpluck::AddRecord(int index)
{
    for(int pos = 0; pos < mRecords.count(); ++pos)
    {
        if(mRecords[pos]->index == index)
        {
            return;
        }
    }

    RecordNode* node = new RecordNode;
    node->done = false;
    node->index = index;
    node->page_id = index;

    mRecords.append(node);
}

void QUnpluck::MarkRecordDone(int index)
{
    for(int pos = 0; pos < mRecords.count(); ++pos)
    {
        if(mRecords[pos]->index == index)
        {
            mRecords[pos]->done = true;
            return;
        }
    }

    AddRecord(index);
    MarkRecordDone(index);
}

void QUnpluck::SetPageID(int index, int page_id)
{
    for(int pos = 0; pos < mRecords.count(); ++pos)
    {
        if(mRecords[pos]->index == index)
        {
            mRecords[pos]->page_id = page_id;
            return;
        }
    }

    AddRecord(index);
    SetPageID(index, page_id);
}

QString QUnpluck::MailtoURLFromBytes(unsigned char* record_data)
{
    unsigned char* bytes = record_data + 8;

    int to_offset = (bytes[0] << 8) + bytes[1];
    int cc_offset = (bytes[2] << 8) + bytes[3];
    int subject_offset = (bytes[4] << 8) + bytes[5];
    int body_offset = (bytes[6] << 8) + bytes[7];

    QString url(QStringLiteral("mailto:"));
    if(to_offset != 0)
    {
        url += QString::fromLatin1((char*)(bytes + to_offset));
    }

    if((cc_offset != 0) || (subject_offset != 0) || (body_offset != 0))
    {
        url += QLatin1String("?");
    }

    if(cc_offset != 0)
    {
        url += QLatin1String("cc=") +
               QString::fromLatin1((char*)(bytes + cc_offset));
    }

    if(subject_offset != 0)
    {
        url += QLatin1String("subject=") +
               QString::fromLatin1((char*)(bytes + subject_offset));
    }

    if(body_offset != 0)
    {
        url += QLatin1String("body=") +
               QString::fromLatin1((char*)(bytes + body_offset));
    }

    return url;
}

QImage QUnpluck::TranscribeImageRecord(unsigned char* bytes)
{
    QImage image;

    TranscribePalmImageToJPEG(bytes + 8, image);

    return image;
}

void QUnpluck::DoStyle(Context* context, int style, bool start)
{
    if(start)
    {
        QTextCharFormat format(context->cursor->charFormat());
        context->stack.push(format);

        int pointSize = qRound(format.fontPointSize());
        switch(style)
        {
        case 1:
            format.setFontWeight(QFont::Bold);
            pointSize += 3;
            break;
        case 2:
            format.setFontWeight(QFont::Bold);
            pointSize += 2;
            break;
        case 3:
            format.setFontWeight(QFont::Bold);
            pointSize += 1;
            break;
        case 4:
            format.setFontWeight(QFont::Bold);
            break;
        case 5:
            format.setFontWeight(QFont::Bold);
            pointSize += -1;
            break;
        case 6:
            format.setFontWeight(QFont::Bold);
            pointSize += -2;
            break;
        case 7:
            format.setFontWeight(QFont::Bold);
            break;
        case 8:
            format.setFontFamily(QStringLiteral("Courier New,courier"));
            break;
        }
        format.setFontPointSize(qMax(pointSize, 1));
        context->cursor->setCharFormat(format);
    }
    else
    {
        if(!context->stack.isEmpty())
        {
            context->cursor->setCharFormat(context->stack.pop());
        }
    }
}

void QUnpluck::ParseText(plkr_Document* doc, unsigned char* ptr, int text_len,
                         int* font, int* style, Context* context)
{
    unsigned char* end;
    int fctype;
    int fclen;

    end = ptr + text_len;
    while(ptr < end)
    {
        if(ptr[0])
        {
            context->cursor->insertText(QString::fromLocal8Bit((char*)ptr));
            ptr += strlen((char*)ptr);
        }
        else
        {
            fctype = GET_FUNCTION_CODE_TYPE(ptr[1]);
            fclen = 2 + GET_FUNCTION_CODE_DATALEN(ptr[1]);
            switch(fctype)
            {
            case PLKR_TFC_LINK:
                switch(fclen)
                {
                case 4: /* ANCHOR_BEGIN */
                {
                    int record_id = (ptr[2] << 8) + ptr[3];

                    /** TODO:
                    plkr_DataRecordType   type =
                        (plkr_DataRecordType)plkr_GetRecordType (doc,
                    record_id); if (type == PLKR_DRTYPE_IMAGE
                        || type ==
                        PLKR_DRTYPE_IMAGE_COMPRESSED)
                        output += QString( "<A HREF=\"r%1.jpg\">"
                    ).arg(record_id); else output += QString( "<A
                    HREF=\"r%1.html\">" ).arg(record_id);
                        */
                    AddRecord(record_id);
                }
                break;
                case 2: /* ANCHOR_END */
                    // TODO:  output += QString( "</A>" );
                    break;
                }
                ptr += fclen;
                break;
            case PLKR_TFC_FONT:
                DoStyle(context, *style, false);
                *style = ptr[2];
                DoStyle(context, *style, true);
                ptr += fclen;
                break;
            case PLKR_TFC_NEWLINE:
            {
                // TODO: remove the setCharFormat when Qt is fixed
                QTextCharFormat format(context->cursor->charFormat());
                context->cursor->insertText(QStringLiteral("\n"));
                context->cursor->setCharFormat(format);
                ptr += fclen;
                break;
            }
            case PLKR_TFC_BITALIC:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontItalic(true);
                context->cursor->setCharFormat(format);
                ptr += fclen;
                break;
            }
            case PLKR_TFC_EITALIC:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontItalic(false);
                context->cursor->setCharFormat(format);
                ptr += fclen;
                break;
            }
            case PLKR_TFC_COLOR:
                if(*font)
                {
                    (*font)--;
                    if(!context->stack.isEmpty())
                    {
                        context->cursor->setCharFormat(context->stack.pop());
                    }
                }

                {
                    QTextCharFormat format(context->cursor->charFormat());
                    context->stack.push(format);

                    format.setForeground(
                        QColor((ptr[2] << 16), (ptr[3] << 8), ptr[4]));
                    context->cursor->setCharFormat(format);
                }

                (*font)++;
                ptr += fclen;
                break;
            case PLKR_TFC_BULINE:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontUnderline(true);
                context->cursor->setCharFormat(format);
                ptr += fclen;
            }
            break;
            case PLKR_TFC_EULINE:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontUnderline(false);
                context->cursor->setCharFormat(format);
                ptr += fclen;
            }
            break;
            case PLKR_TFC_BSTRIKE:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontStrikeOut(true);
                context->cursor->setCharFormat(format);
                ptr += fclen;
                break;
            }
            case PLKR_TFC_ESTRIKE:
            {
                QTextCharFormat format(context->cursor->charFormat());
                format.setFontStrikeOut(false);
                context->cursor->setCharFormat(format);
                ptr += fclen;
                break;
            }
            case PLKR_TFC_TABLE:
                if(fclen == 4)
                {
                    int record_id, datalen;
                    plkr_DataRecordType type = (plkr_DataRecordType)0;
                    unsigned char* bytes = nullptr;

                    record_id = (ptr[2] << 8) + ptr[3];
                    bytes =
                        plkr_GetRecordBytes(doc, record_id, &datalen, &type);
                    TranscribeTableRecord(doc, context, bytes);
                }
                ptr += fclen;
                break;
            default:
                ptr += fclen;
            }
        }
    }
}

bool QUnpluck::TranscribeTableRecord(plkr_Document* doc, Context* context,
                                     unsigned char* bytes)
{
    unsigned char* ptr = &bytes[24];
    unsigned char* end;
    //    char*           align_names[] = { "left", "right", "center" };
    //     bool            in_row = false;
    //     int             cols;
    int size;
    //     int             rows;
    //     int             border;
    int record_id;
    //     int             align;
    int text_len;
    //     int             colspan;
    //     int             rowspan;
    int font = 0;
    int style = 0;
    int fctype;
    int fclen;
    //     long            border_color;
    //     long            link_color;

    size = (bytes[8] << 8) + bytes[9];
    //     cols = (bytes[10] << 8) + bytes[11];
    //     rows = (bytes[12] << 8) + bytes[13];
    //     border = bytes[15];
    //     border_color = (bytes[17] << 16) + (bytes[18] << 8) + (bytes[19] <<
    //     8); link_color = (bytes[21] << 16) + (bytes[22] << 8) + (bytes[23] <<
    //     8);

    end = ptr + size - 1;
    /**
        output += QString( "<TABLE border=%1 bordercolor=\"#%2\" "
                 "linkcolor=\"#%3\">\n" ).arg(border, border_color, link_color);
    */
    while(ptr < end)
    {
        if(ptr[0] == '\0')
        {
            fctype = GET_FUNCTION_CODE_TYPE(ptr[1]);
            fclen = 2 + GET_FUNCTION_CODE_DATALEN(ptr[1]);
            switch(fctype)
            {
            case PLKR_TFC_TABLE:
                switch(fclen)
                {
                case 2: /* NEW_ROW */
                    /*
                        if (in_row)
                            output += QString( "</TR>\n" );
                        output += QString( "<TR>\n" );
                        in_row = true;
                        */
                    ptr += fclen;
                    break;
                case 9: /* NEW_CELL */
                    //                             align = ptr[2];
                    //                             colspan = ptr[5];
                    //                             rowspan = ptr[6];
                    /**
                    output += QString( "<TD align=\"%1\" colspan=%2 "
                             "rowspan=%3 bordercolor=\"#\">" ).arg(
                             align_names[align], colspan, rowspan );
//                                     border_color);
*/
                    if((record_id = READ_BIGENDIAN_SHORT(&ptr[3])))
                    {
                        QTextCharFormat format = context->cursor->charFormat();
                        context->cursor->insertImage(
                            QStringLiteral("%1.jpg").arg(record_id));
                        context->cursor->setCharFormat(format);
                        context->images.append(record_id);
                        AddRecord(record_id);
                    }
                    DoStyle(context, style, true);
                    text_len = READ_BIGENDIAN_SHORT(&ptr[7]);
                    ptr += fclen;
                    ParseText(doc, ptr, text_len, &font, &style, context);
                    ptr += text_len;
                    DoStyle(context, style, false);
                    // output += QString( "</TD>\n" );
                    break;
                default:
                    ptr += fclen;
                }
                break;
            default:
                ptr += fclen;
            }
        }
        else
        {
            // output += QString( "</TABLE>\n" );
            return false;
        }
    }

    //    output += QString( "</TABLE>\n" );
    return true;
}

typedef struct
{
    int size;
    int attributes;
} ParagraphInfo;

static ParagraphInfo* ParseParagraphInfo(unsigned char* bytes, int* nparas)
{
    ParagraphInfo* paragraph_info;
    int j;
    int n;

    n = (bytes[2] << 8) + bytes[3];
    paragraph_info = (ParagraphInfo*)malloc(sizeof(ParagraphInfo) * n);
    for(j = 0; j < n; j++)
    {
        paragraph_info[j].size =
            (bytes[8 + (j * 4) + 0] << 8) + bytes[8 + (j * 4) + 1];
        paragraph_info[j].attributes =
            (bytes[8 + (j * 4) + 2] << 8) + bytes[8 + (j * 4) + 3];
    }
    *nparas = n;
    return paragraph_info;
}

bool QUnpluck::TranscribeTextRecord(plkr_Document* doc, int id,
                                    Context* context, unsigned char* bytes,
                                    plkr_DataRecordType type)
{
    unsigned char* ptr;
    unsigned char* run;
    unsigned char* para_start;
    unsigned char* data;
    unsigned char* start;
    ParagraphInfo* paragraphs;
    bool first_record_of_page = true;
    bool current_link;
    bool current_italic;
    bool current_struckthrough;
    bool current_underline;
    int fctype;
    int fclen;
    int para_index;
    int para_len;
    int textlen;
    int data_len;
    int current_font;
    int record_index;
    //     int             current_alignment;
    //     int             current_left_margin;
    //     int             current_right_margin;
    int nparagraphs;
    //     long            current_color;

    record_index = id;

    paragraphs = ParseParagraphInfo(bytes, &nparagraphs);
    start = bytes + 8 + ((bytes[2] << 8) + bytes[3]) * 4;

    for(para_index = 0, ptr = start, run = start; para_index < nparagraphs;
        para_index++)
    {
        para_len = paragraphs[para_index].size;

        /* If the paragraph is the last in the record, and it consists
           of a link to the next record in the logical page, we trim off
           the paragraph and instead insert the whole page */

        if(((para_index + 1) == nparagraphs) &&
           (para_len == (sizeof("Click here for the next part") + 5)) &&
           (*ptr == 0) && (ptr[1] == ((PLKR_TFC_LINK << 3) + 2)) &&
           (strcmp((char*)(ptr + 4), "Click here for the next part") == 0))
        {
            record_index = (ptr[2] << 8) + ptr[3];
            if((data = plkr_GetRecordBytes(doc, record_index, &data_len,
                                           &type)) == nullptr)
            {
                //                ShowWarning ("Can't open record %d!",
                //                record_index);
                free(paragraphs);
                return false;
            }
            else if(!(type == PLKR_DRTYPE_TEXT_COMPRESSED ||
                      type == PLKR_DRTYPE_TEXT))
            {
                //                ShowWarning ("Bad record type %d in record
                //                linked from end of record %d", type, id);
                free(paragraphs);
                return false;
            }
            first_record_of_page = false;
            para_index = 0;
            ptr = data + 8 + ((data[2] << 8) + data[3]) * 4;
            run = ptr;
            free(paragraphs);
            paragraphs = ParseParagraphInfo(data, &nparagraphs);
            para_len = paragraphs[para_index].size;
            MarkRecordDone(record_index);
            SetPageID(record_index, id);
        }

        if((para_index == 0) && !first_record_of_page && (*ptr == 0) &&
           (ptr[1] == ((PLKR_TFC_LINK << 3) + 2)) &&
           (strcmp((char*)(ptr + 4), "Click here for the previous part") == 0))
        {
            /* throw away this inserted paragraph */
            ptr += para_len;
            run = ptr;
            continue;
        }

        QTextCharFormat format(context->cursor->charFormat());
        QTextBlockFormat blockFormat(context->cursor->blockFormat());
        blockFormat.setAlignment(Qt::AlignLeft);
        context->cursor->insertBlock(blockFormat);
        context->cursor->setCharFormat(format);

        mNamedTargets.insert(
            QStringLiteral("para:%1-%2").arg(record_index).arg(para_index),
            QPair<int, QTextBlock>(GetPageID(record_index),
                                   context->cursor->block()));

        current_link = false;

        /* at the beginning of a paragraph, we start with a clean graphics
         * context */
        current_font = 0;
        //         current_alignment = 0;
        //         current_color = 0;
        current_italic = false;
        current_underline = false;
        current_struckthrough = false;
        //         current_left_margin = 0;
        //         current_right_margin = 0;

        for(para_start = ptr, textlen = 0; (ptr - para_start) < para_len;)
        {
            if(*ptr == 0)
            {
                /* function code */

                if((ptr - run) > 0)
                {
                    /* write out any pending text */
                    context->cursor->insertText(
                        QString::fromLatin1((char*)run, ptr - run));
                    textlen += (ptr - run);
                }

                ptr++;
                fctype = GET_FUNCTION_CODE_TYPE(*ptr);
                fclen = GET_FUNCTION_CODE_DATALEN(*ptr);
                ptr++;

                if(fctype == PLKR_TFC_NEWLINE)
                {
                    // TODO: remove the setCharFormat when Qt is fixed
                    QTextCharFormat format(context->cursor->charFormat());
                    context->cursor->insertText(QStringLiteral("\n"));
                    context->cursor->setCharFormat(format);
                }
                else if(fctype == PLKR_TFC_LINK)
                {
                    int record_id, real_record_id, datalen;
                    plkr_DataRecordType type = (plkr_DataRecordType)0;
                    unsigned char* bytes = nullptr;
                    char* url = nullptr;

                    if(fclen == 0)
                    {
                        if(current_link)
                        {
                            if(!context->stack.isEmpty())
                            {
                                context->cursor->setCharFormat(
                                    context->stack.pop());
                            }

                            if(!context->linkUrl.isEmpty())
                            {
                                Link link;
                                link.url = context->linkUrl;
                                link.start = context->linkStart;
                                link.end = context->cursor->position();
                                link.page = GetPageID(id);
                                mLinks.append(link);
                            }
                        }
                        current_link = false;
                    }
                    else
                    {
                        record_id = (ptr[0] << 8) + ptr[1];
                        bytes = plkr_GetRecordBytes(doc, record_id, &datalen,
                                                    &type);
                        if(!bytes)
                        {
                            url = plkr_GetRecordURL(doc, record_id);
                        }
                        if(bytes && (type == PLKR_DRTYPE_MAILTO))
                        {
                            context->linkUrl = MailtoURLFromBytes(bytes);
                            context->linkStart = context->cursor->position();

                            QTextCharFormat format(
                                context->cursor->charFormat());
                            context->stack.push(format);
                            format.setForeground(Qt::blue);
                            format.setUnderlineStyle(
                                QTextCharFormat::SingleUnderline);
                            context->cursor->setCharFormat(format);
                            current_link = true;
                        }
                        else if(!bytes && url)
                        {
                            context->linkUrl = QString::fromLatin1(url);
                            context->linkStart = context->cursor->position();

                            QTextCharFormat format(
                                context->cursor->charFormat());
                            context->stack.push(format);
                            format.setForeground(Qt::blue);
                            format.setUnderlineStyle(
                                QTextCharFormat::SingleUnderline);
                            context->cursor->setCharFormat(format);
                            current_link = true;
                        }
                        else if(bytes && (fclen == 2))
                        {
                            AddRecord(record_id);
                            real_record_id = GetPageID(record_id);
                            if(type == PLKR_DRTYPE_IMAGE ||
                               type == PLKR_DRTYPE_IMAGE_COMPRESSED)
                            {
                                context->linkUrl =
                                    QStringLiteral("%1.jpg").arg(record_id);
                                context->linkStart =
                                    context->cursor->position();
                            }
                            else
                            {
                                context->linkUrl =
                                    QStringLiteral("page:%1").arg(
                                        real_record_id);
                                context->linkStart =
                                    context->cursor->position();
                            }
                            QTextCharFormat format(
                                context->cursor->charFormat());
                            context->stack.push(format);
                            format.setForeground(Qt::blue);
                            format.setUnderlineStyle(
                                QTextCharFormat::SingleUnderline);
                            context->cursor->setCharFormat(format);
                            current_link = true;
                        }
                        else if(bytes && (fclen == 4))
                        {
                            AddRecord(record_id);

                            context->linkUrl = QStringLiteral("para:%1-%2")
                                                   .arg(record_id)
                                                   .arg((ptr[2] << 8) + ptr[3]);
                            context->linkStart = context->cursor->position();

                            QTextCharFormat format(
                                context->cursor->charFormat());
                            context->stack.push(format);
                            format.setForeground(Qt::blue);
                            format.setUnderlineStyle(
                                QTextCharFormat::SingleUnderline);
                            context->cursor->setCharFormat(format);
                            current_link = true;
                        }
                        else
                        {
                            //    ShowWarning("odd link found:  record_id=%d,
                            //    bytes=0x%p, type=%d, url=%s", record_id,
                            //    bytes, type, (url ? url : "0x0"));
                        }
                    }
                }
                else if(fctype == PLKR_TFC_FONT)
                {
                    if(current_font != *ptr)
                    {
                        if(!context->stack.isEmpty())
                        {
                            context->cursor->setCharFormat(
                                context->stack.pop());
                        }

                        QTextCharFormat format(context->cursor->charFormat());
                        context->stack.push(format);

                        int pointSize = qRound(format.fontPointSize());
                        if(*ptr == 1)
                        {
                            format.setFontWeight(QFont::Bold);
                            pointSize += 3;
                        }
                        else if(*ptr == 2)
                        {
                            format.setFontWeight(QFont::Bold);
                            pointSize += 2;
                        }
                        else if(*ptr == 3)
                        {
                            format.setFontWeight(QFont::Bold);
                            pointSize += 1;
                        }
                        else if(*ptr == 4)
                        {
                            format.setFontWeight(QFont::Bold);
                        }
                        else if(*ptr == 5)
                        {
                            format.setFontWeight(QFont::Bold);
                            pointSize += -1;
                        }
                        else if(*ptr == 6)
                        {
                            format.setFontWeight(QFont::Bold);
                            pointSize += -2;
                        }
                        else if(*ptr == 7)
                        {
                            format.setFontWeight(QFont::Bold);
                        }
                        else if(*ptr == 8)
                        {
                            format.setFontFamily(
                                QStringLiteral("Courier New,courier"));
                        }
                        else if(*ptr == 11)
                        {
                            format.setVerticalAlignment(
                                QTextCharFormat::AlignSuperScript);
                        }
                        format.setFontPointSize(qMax(pointSize, 1));

                        context->cursor->setCharFormat(format);

                        current_font = *ptr;
                    }
                }
                else if(fctype == PLKR_TFC_BITALIC)
                {
                    QTextCharFormat format(context->cursor->charFormat());
                    format.setFontItalic(true);
                    context->cursor->setCharFormat(format);

                    current_italic = true;
                }
                else if(fctype == PLKR_TFC_EITALIC)
                {
                    if(current_italic)
                    {
                        QTextCharFormat format(context->cursor->charFormat());
                        format.setFontItalic(false);
                        context->cursor->setCharFormat(format);
                        current_italic = false;
                    }
                }
                else if(fctype == PLKR_TFC_BULINE)
                {
                    QTextCharFormat format(context->cursor->charFormat());
                    format.setFontUnderline(true);
                    context->cursor->setCharFormat(format);
                    current_underline = true;
                }
                else if(fctype == PLKR_TFC_EULINE)
                {
                    if(current_underline)
                    {
                        QTextCharFormat format(context->cursor->charFormat());
                        format.setFontUnderline(false);
                        context->cursor->setCharFormat(format);
                        current_underline = false;
                    }
                }
                else if(fctype == PLKR_TFC_BSTRIKE)
                {
                    QTextCharFormat format(context->cursor->charFormat());
                    format.setFontStrikeOut(true);
                    context->cursor->setCharFormat(format);
                    current_struckthrough = true;
                }
                else if(fctype == PLKR_TFC_ESTRIKE)
                {
                    if(current_struckthrough)
                    {
                        QTextCharFormat format(context->cursor->charFormat());
                        format.setFontStrikeOut(false);
                        context->cursor->setCharFormat(format);
                        current_struckthrough = false;
                    }
                }
                else if(fctype == PLKR_TFC_HRULE)
                {
                    QTextCharFormat charFormat = context->cursor->charFormat();
                    QTextBlockFormat oldBlockFormat =
                        context->cursor->blockFormat();

                    QTextBlockFormat blockFormat;
                    blockFormat.setProperty(
                        QTextFormat::BlockTrailingHorizontalRulerWidth,
                        QStringLiteral("100%"));
                    context->cursor->insertBlock(blockFormat);
                    context->cursor->insertBlock(oldBlockFormat);
                    context->cursor->setCharFormat(charFormat);
                }
                else if(fctype == PLKR_TFC_ALIGN)
                {
                    //                     current_alignment = 0;

                    if(*ptr < 4)
                    {
                        QTextBlockFormat format(context->cursor->blockFormat());
                        if(*ptr == 0)
                        {
                            format.setAlignment(Qt::AlignLeft);
                        }
                        else if(*ptr == 1)
                        {
                            format.setAlignment(Qt::AlignRight);
                        }
                        else if(*ptr == 2)
                        {
                            format.setAlignment(Qt::AlignCenter);
                        }
                        else if(*ptr == 3)
                        {
                            format.setAlignment(Qt::AlignJustify);
                        }

                        QTextCharFormat charFormat(
                            context->cursor->charFormat());
                        context->cursor->insertBlock(format);
                        context->cursor->setCharFormat(charFormat);

                        //                         current_alignment = (*ptr) +
                        //                         1;
                    }
                }
                else if(fctype == PLKR_TFC_COLOR)
                {
                    /* not sure what to do here yet */
                    /*
                    fprintf (fp, "<!-- color=\"#%02x%02x%02x\" -->",
                             ptr[0], ptr[1], ptr[2]);*/
                    //                     current_color =
                    //                         (ptr[0] << 16) + (ptr[1] << 8) +
                    //                         ptr[2];
                }
                else if(fctype == PLKR_TFC_IMAGE || fctype == PLKR_TFC_IMAGE2)
                {
                    QTextCharFormat format = context->cursor->charFormat();
                    context->cursor->insertImage(
                        QStringLiteral("%1.jpg").arg((ptr[0] << 8) + ptr[1]));
                    context->images.append((ptr[0] << 8) + ptr[1]);
                    context->cursor->setCharFormat(format);
                    AddRecord((ptr[0] << 8) + ptr[1]);
                }
                else if(fctype == PLKR_TFC_TABLE)
                {
                    int record_id, datalen;
                    plkr_DataRecordType type = (plkr_DataRecordType)0;
                    unsigned char* bytes = nullptr;

                    record_id = (ptr[0] << 8) + ptr[1];
                    bytes =
                        plkr_GetRecordBytes(doc, record_id, &datalen, &type);

                    TranscribeTableRecord(doc, context, bytes);
                }
                else if(fctype == PLKR_TFC_UCHAR)
                {
                    if(fclen == 3)
                    {
                        context->cursor->insertText(
                            QChar((ptr[1] << 8) + ptr[2]));
                    }
                    else if(fclen == 5)
                    {
                        context->cursor->insertText(
                            QChar((ptr[3] << 8) + ptr[4]));
                    }
                    /* skip over alternate text */
                    ptr += ptr[0];
                }

                ptr += fclen;
                run = ptr;
            }
            else
            {
                ptr++;
            }
        }

        if((ptr - run) > 0)
        {
            /* output any pending text at the end of the paragraph */
            context->cursor->insertText(
                QString::fromLatin1((char*)run, ptr - run));
            textlen += (ptr - run);
            run = ptr;
        }

        /* clear the graphics state again */

        if(current_font > 0 && current_font < 9)
        {
            if(!context->stack.isEmpty())
            {
                context->cursor->setCharFormat(context->stack.pop());
            }
        }

        if(current_italic)
        {
            QTextCharFormat format(context->cursor->charFormat());
            format.setFontItalic(false);
            context->cursor->setCharFormat(format);
        }
        if(current_underline)
        {
            QTextCharFormat format(context->cursor->charFormat());
            format.setFontUnderline(false);
            context->cursor->setCharFormat(format);
        }
        if(current_struckthrough)
        {
            QTextCharFormat format(context->cursor->charFormat());
            format.setFontStrikeOut(false);
            context->cursor->setCharFormat(format);
        }
    }
    free(paragraphs);
    return true;
}

bool QUnpluck::TranscribeRecord(int index)
{
    plkr_DataRecordType type;
    int data_len;
    bool status = true;

    unsigned char* data =
        plkr_GetRecordBytes(mDocument, index, &data_len, &type);
    if(!data)
    {
        MarkRecordDone(index);
        return false;
    }

    if(type == PLKR_DRTYPE_TEXT_COMPRESSED || type == PLKR_DRTYPE_TEXT)
    {
        QTextDocument* document = new QTextDocument;

        QTextFrameFormat format(document->rootFrame()->frameFormat());
        format.setMargin(20);
        document->rootFrame()->setFrameFormat(format);

        Context* context = new Context;
        context->recordId = index;
        context->document = document;
        context->cursor = new QTextCursor(document);

        QTextCharFormat charFormat;
        charFormat.setFontPointSize(10);
        charFormat.setFontFamily(QStringLiteral("Helvetica"));
        context->cursor->setCharFormat(charFormat);

        status = TranscribeTextRecord(mDocument, index, context, data, type);
        document->setTextWidth(600);

        delete context->cursor;
        mContext.append(context);
    }
    else if(type == PLKR_DRTYPE_IMAGE_COMPRESSED || type == PLKR_DRTYPE_IMAGE)
    {
        QImage image = TranscribeImageRecord(data);
        mImages.insert(index, image);
    }
    else if(type == PLKR_DRTYPE_MULTIIMAGE)
    {
        QImage image;
        if(TranscribeMultiImageRecord(mDocument, image, data))
        {
            mImages.insert(index, image);
        }
    }
    else
    {
        status = false;
    }

    // plkr_GetHomeRecordID (doc)))

    MarkRecordDone(index);

    return status;
}
