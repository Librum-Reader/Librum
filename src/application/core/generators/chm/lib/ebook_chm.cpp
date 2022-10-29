/*
    Kchmviewer - a CHM and EPUB file viewer with broad language support
    SPDX-FileCopyrightText: 2004-2014 George Yunaev gyunaev@ulduzsoft.com

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "ebook_chm.h"
#include <QDebug>
#include <QFile>
#include <QVector>
#include "bitfiddle.h"
#include "ebook_chm_encoding.h"

// Big-enough buffer size for use with various routines.
#define BUF_SIZE 4096
#define COMMON_BUF_LEN 1025

#define TOPICS_ENTRY_LEN 16
#define URLTBL_ENTRY_LEN 12

//#define DEBUGPARSER(A)	qDebug A
#define DEBUGPARSER(A)

#define URL_SCHEME_CHM QStringLiteral("ms-its")

EBook_CHM::EBook_CHM() :
    EBook()
{
    m_envOptions = QString::fromLatin1(qgetenv("KCHMVIEWEROPTS"));
    m_chmFile = nullptr;
    m_filename = m_font = QString();

    m_textCodec = nullptr;
    m_textCodecForSpecialFiles = nullptr;
    m_detectedLCID = 0;
    m_currentEncoding = QStringLiteral("UTF-8");
}

EBook_CHM::~EBook_CHM()
{
    close();
}

void EBook_CHM::close()
{
    if(m_chmFile == nullptr)
    {
        return;
    }

    chm_close(m_chmFile);

    m_chmFile = nullptr;
    m_filename = m_font = QString();

    m_home.clear();
    m_topicsFile.clear();
    m_indexFile.clear();

    m_textCodec = nullptr;
    m_textCodecForSpecialFiles = nullptr;
    m_detectedLCID = 0;
    m_currentEncoding = QStringLiteral("UTF-8");
}

QString EBook_CHM::title() const
{
    return encodeWithCurrentCodec(m_title);
}

QUrl EBook_CHM::homeUrl() const
{
    return pathToUrl(QString::fromUtf8(m_home));
}

bool EBook_CHM::hasFeature(EBook::Feature code) const
{
    switch(code)
    {
    case FEATURE_TOC:
        return m_tocAvailable;

    case FEATURE_INDEX:
        return m_indexAvailable;

    case FEATURE_ENCODING:
        return true;
    }

    return false;
}

bool EBook_CHM::getTableOfContents(QList<EBookTocEntry>& toc) const
{
    if(parseBinaryTOC(toc))
    {
        return true;
    }

    // Parse the plain text TOC
    QList<ParsedEntry> parsed;

    if(!parseFileAndFillArray(QString::fromUtf8(m_topicsFile), parsed, false))
    {
        return false;
    }

    // Find out the root offset, and reduce the indent level to it
    // so the toc starts from zero offset.
    int root_offset = -1;

    // Fill up the real toc
    toc.reserve(parsed.size());
    for(const ParsedEntry& e : qAsConst(parsed))
    {
        if(root_offset == -1)
        {
            root_offset = e.indent;
        }

        EBookTocEntry entry;
        entry.iconid = (EBookTocEntry::Icon)e.iconid;
        entry.indent = e.indent - root_offset;
        entry.name = e.name;

        if(!e.urls.empty())
        {
            entry.url = e.urls[0];
        }

        toc.append(entry);
    }

    return true;
}

bool EBook_CHM::getIndex(QList<EBookIndexEntry>& index) const
{
    // Parse the plain text index
    QList<ParsedEntry> parsed;

    if(!parseFileAndFillArray(QString::fromUtf8(m_indexFile), parsed, true))
    {
        return false;
    }

    // Find out the root offset, and reduce the indent level to it
    // so the index starts from zero offset.
    int root_offset = 0;

    // Fill up the real index
    index.reserve(parsed.size());

    // Find the index root offset
    const QList<ParsedEntry>& parsedList = parsed;
    for(const ParsedEntry& e : parsedList)
    {
        if(e.urls.empty())
        {
            continue;
        }

        root_offset = qMin(root_offset, e.indent);
    }

    // And apply the index
    for(const ParsedEntry& e : parsedList)
    {
        if(e.urls.empty())
        {
            continue;
        }

        EBookIndexEntry entry;
        entry.name = e.name;
        entry.urls = e.urls;
        entry.seealso = e.seealso;

        // If the index array is empty, make sure the first entry is on root
        // offset
        if(index.isEmpty())
        {
            entry.indent = root_offset;
        }
        else
        {
            entry.indent = e.indent - root_offset;
        }

        index.append(entry);
        printf("%d: %s\n", entry.indent, qPrintable(entry.name));
    }

    return true;
}

bool EBook_CHM::getFileContentAsString(QString& str, const QUrl& url) const
{
    return getTextContent(str, urlToPath(url));
}

bool EBook_CHM::getFileContentAsBinary(QByteArray& data, const QUrl& url) const
{
    return getBinaryContent(data, urlToPath(url));
}

bool EBook_CHM::getBinaryContent(QByteArray& data, const QString& url) const
{
    chmUnitInfo ui;

    if(!ResolveObject(url, &ui))
    {
        return false;
    }

    data.resize(ui.length);

    if(RetrieveObject(&ui, (unsigned char*)data.data(), 0, ui.length))
    {
        return true;
    }

    return false;
}

bool EBook_CHM::getTextContent(QString& str, const QString& url,
                               bool internal_encoding) const
{
    QByteArray buf;

    if(getBinaryContent(buf, url))
    {
        unsigned int length = buf.size();

        if(length > 0)
        {
            buf.resize(length + 1);
            buf[length] = '\0';

            str = internal_encoding ? QString::fromUtf8(buf.constData())
                                    : encodeWithCurrentCodec(buf.constData());
            return true;
        }
    }

    return false;
}

int EBook_CHM::getContentSize(const QString& url)
{
    chmUnitInfo ui;

    if(!ResolveObject(url, &ui))
    {
        return -1;
    }

    return ui.length;
}

bool EBook_CHM::load(const QString& archiveName)
{
    QString filename;

    // If the file has a file:// prefix, remove it
    if(archiveName.startsWith(QLatin1String("file://")))
    {
        filename = archiveName.mid(7);  // strip it
    }
    else
    {
        filename = archiveName;
    }

    if(m_chmFile)
    {
        close();
    }

#if defined(WIN32)
    m_chmFile = chm_open((BSTR)QFile::encodeName(filename).constData());
#else
    m_chmFile = chm_open(QFile::encodeName(filename).constData());
#endif

    if(m_chmFile == nullptr)
    {
        return false;
    }

    m_filename = filename;

    // Reset encoding
    m_textCodec = nullptr;
    m_textCodecForSpecialFiles = nullptr;
    m_currentEncoding = QStringLiteral("UTF-8");

    // Get information from /#WINDOWS and /#SYSTEM files (encoding, title,
    // context file and so) and guess the encoding
    getInfoFromWindows();
    getInfoFromSystem();
    guessTextEncoding();

    // Check whether the search tables are present
    if(ResolveObject(QStringLiteral("/#TOPICS"), &m_chmTOPICS) &&
       ResolveObject(QStringLiteral("/#STRINGS"), &m_chmSTRINGS) &&
       ResolveObject(QStringLiteral("/#URLTBL"), &m_chmURLTBL) &&
       ResolveObject(QStringLiteral("/#URLSTR"), &m_chmURLSTR))
    {
        m_lookupTablesValid = true;
        fillTopicsUrlMap();
    }
    else
    {
        m_lookupTablesValid = false;
    }

    // Some CHM files have toc and index files, but do not set the name
    // properly. Some heuristics here.
    if(m_topicsFile.isEmpty() && hasFile(QStringLiteral("/toc.hhc")))
    {
        m_topicsFile = "/toc.hhc";
    }

    if(m_indexFile.isEmpty() && hasFile(QStringLiteral("/index.hhk")))
    {
        m_indexFile = "/index.hhk";
    }

    if(!m_topicsFile.isEmpty() ||
       (m_lookupTablesValid && hasFile(QStringLiteral("/#TOCIDX"))))
    {
        m_tocAvailable = true;
    }
    else
    {
        m_tocAvailable = false;
    }

    if(!m_indexFile.isEmpty() ||
       (m_lookupTablesValid &&
        hasFile(QStringLiteral("/$WWKeywordLinks/BTree"))))
    {
        m_indexAvailable = true;
    }
    else
    {
        m_indexAvailable = false;
    }

    return true;
}

int EBook_CHM::findStringInQuotes(const QString& tag, int offset,
                                  QString& value, bool firstquote,
                                  bool decodeentities) const
{
    int qbegin = tag.indexOf(QLatin1Char('"'), offset);

    if(qbegin == -1)
    {
        qFatal("EBook_CHMImpl::findStringInQuotes: cannot find first quote in "
               "<param> tag: '%s'",
               qPrintable(tag));
    }

    int qend = firstquote ? tag.indexOf(QLatin1Char('"'), qbegin + 1)
                          : tag.lastIndexOf(QLatin1Char('"'));

    if(qend == -1 || qend <= qbegin)
    {
        qFatal("EBook_CHMImpl::findStringInQuotes: cannot find last quote in "
               "<param> tag: '%s'",
               qPrintable(tag));
    }

    // If we do not need to decode HTML entities, just return.
    if(decodeentities)
    {
        QString htmlentity = QString();
        bool fill_entity = false;

        value.reserve(qend - qbegin);  // to avoid multiple memory allocations

        for(int i = qbegin + 1; i < qend; i++)
        {
            if(!fill_entity)
            {
                if(tag[i] == QLatin1Char('&'))
                {  // HTML entity starts
                    fill_entity = true;
                }
                else
                {
                    value.append(tag[i]);
                }
            }
            else
            {
                if(tag[i] == QLatin1Char(';'))  // HTML entity ends
                {
                    // If entity is an ASCII code, just decode it
                    QString decode = m_htmlEntityDecoder.decode(htmlentity);

                    if(decode.isNull())
                    {
                        break;
                    }

                    value.append(decode);
                    htmlentity = QString();
                    fill_entity = false;
                }
                else
                {
                    htmlentity.append(tag[i]);
                }
            }
        }
    }
    else
    {
        value = tag.mid(qbegin + 1, qend - qbegin - 1);
    }

    return qend + 1;
}

bool EBook_CHM::parseFileAndFillArray(const QString& file,
                                      QList<ParsedEntry>& data,
                                      bool asIndex) const
{
    QString src;
    const int MAX_NEST_DEPTH = 256;

    if(!getTextContent(src, file) || src.isEmpty())
    {
        return false;
    }

    /*
        // Save the index for debugging purposes
        QFile outfile( "parsed.htm" );

        if ( outfile.open( QIODevice::WriteOnly ) )
        {
            QTextStream textstream( &outfile );
            textstream << src;
            outfile.close();
        }
    */

    EBookTocEntry::Icon defaultimagenum = EBookTocEntry::IMAGE_AUTO;
    int pos = 0, indent = 0, root_indent_offset = 0;
    bool in_object = false, root_indent_offset_set = false;

    ParsedEntry entry;
    entry.iconid = defaultimagenum;

    // Split the HHC file by HTML tags
    int stringlen = src.length();

    while(pos < stringlen && (pos = src.indexOf(QLatin1Char('<'), pos)) != -1)
    {
        int i, word_end = 0;

        for(i = ++pos; i < stringlen; i++)
        {
            // If a " or ' is found, skip to the next one.
            if((src[i] == QLatin1Char('"') || src[i] == QLatin1Char('\'')))
            {
                // find where quote ends, either by another quote, or by '>'
                // symbol (some people don't know HTML)
                int nextpos = src.indexOf(src[i], i + 1);
                if(nextpos == -1 &&
                   (nextpos = src.indexOf(QLatin1Char('>'), i + 1)) == -1)
                {
                    qWarning(
                        "EBook_CHMImpl::ParseHhcAndFillTree: corrupted TOC: %s",
                        qPrintable(src.mid(i)));
                    return false;
                }

                i = nextpos;
            }
            else if(src[i] == QLatin1Char('>'))
            {
                break;
            }
            else if(!src[i].isLetterOrNumber() && src[i] != QLatin1Char('/') &&
                    !word_end)
            {
                word_end = i;
            }
        }

        QString tagword, tag = src.mid(pos, i - pos);

        if(word_end)
        {
            tagword = src.mid(pos, word_end - pos).toLower();
        }
        else
        {
            tagword = tag.toLower();
        }

        // DEBUGPARSER(("tag: '%s', tagword: '%s'\n", qPrintable( tag ),
        // qPrintable( tagword ) ));

        // <OBJECT type="text/sitemap"> - a topic entry
        if(tagword == QLatin1String("object") &&
           tag.indexOf(QLatin1String("text/sitemap"), 0, Qt::CaseInsensitive) !=
               -1)
        {
            in_object = true;
        }
        else if(tagword == QLatin1String("/object") && in_object)
        {
            // a topic entry closed. Add a tree item
            if(entry.name.isEmpty() && entry.urls.isEmpty())
            {
                qWarning("EBook_CHMImpl::ParseAndFillTopicsTree: <object> tag "
                         "is parsed, but both name and url are empty.");
            }
            else
            {
                // If the name is empty, use the URL as name
                if(entry.name.isEmpty())
                {
                    entry.name = entry.urls[0].toString();
                }

                if(!root_indent_offset_set)
                {
                    root_indent_offset_set = true;
                    root_indent_offset = indent;

                    if(root_indent_offset > 1)
                    {
                        qWarning(
                            "CHM has improper index; root indent offset is %d",
                            root_indent_offset);
                    }
                }

                // Trim the entry name
                entry.name = entry.name.trimmed();

                int real_indent = indent - root_indent_offset;

                entry.indent = real_indent;
                data.push_back(entry);
            }

            entry.name = QString();
            entry.urls.clear();
            entry.iconid = defaultimagenum;
            entry.seealso.clear();
            in_object = false;
        }
        else if(tagword == QLatin1String("param") && in_object)
        {
            // <param name="Name" value="First Page">
            int offset;  // strlen("param ")
            const QString name_pattern = QStringLiteral("name="),
                          value_pattern = QStringLiteral("value=");
            QString pname, pvalue;

            if((offset = tag.indexOf(name_pattern, 0, Qt::CaseInsensitive)) ==
               -1)
            {
                qFatal("EBook_CHMImpl::ParseAndFillTopicsTree: bad <param> tag "
                       "'%s': no name=\n",
                       qPrintable(tag));
            }

            // offset+5 skips 'name='
            offset = findStringInQuotes(tag, offset + name_pattern.length(),
                                        pname, true, false);
            pname = pname.toLower();

            if((offset = tag.indexOf(value_pattern, offset,
                                     Qt::CaseInsensitive)) == -1)
            {
                qFatal("EBook_CHMImpl::ParseAndFillTopicsTree: bad <param> tag "
                       "'%s': no value=\n",
                       qPrintable(tag));
            }

            // offset+6 skips 'value='
            findStringInQuotes(tag, offset + value_pattern.length(), pvalue,
                               false, true);

            // DEBUGPARSER(("<param>: name '%s', value '%s'", qPrintable( pname
            // ), qPrintable( pvalue )));

            if(pname == QLatin1String("name") ||
               pname == QLatin1String("keyword"))
            {
                // Some help files contain duplicate names, where the second
                // name is empty. Work it around by keeping the first one
                if(!pvalue.isEmpty())
                {
                    entry.name = pvalue;
                }
            }
            else if(pname == QLatin1String("merge"))
            {
                // MERGE implementation is experimental
                QUrl mergeurl = pathToUrl(pvalue);
                QString mergecontent;

                if(getFileContentAsString(mergecontent, mergeurl) &&
                   !mergecontent.isEmpty())
                {
                    qWarning("MERGE is used in index; the implementation is "
                             "experimental. Please let me know if it works");

                    // Merge the read value into the current parsed file.
                    // To save memory it is done in a kinda hacky way:
                    src = mergecontent + src.mid(i);
                    pos = 0;
                    stringlen = src.length();
                }
                else
                {
                    qWarning("MERGE is used in index but file %s was not found "
                             "in CHM archive",
                             qPrintable(pvalue));
                }
            }
            else if(pname == QLatin1String("local"))
            {
                // Check for URL duplication
                QUrl url = pathToUrl(pvalue);

                if(!entry.urls.contains(url))
                {
                    entry.urls.push_back(url);
                }
            }
            else if(pname == QLatin1String("see also") && asIndex &&
                    entry.name != pvalue)
            {
                entry.urls.push_back(QUrl(QStringLiteral("seealso")));
                entry.seealso = pvalue;
            }
            else if(pname == QLatin1String("imagenumber"))
            {
                bool bok;
                int imgnum = pvalue.toInt(&bok);

                if(bok && imgnum >= 0 &&
                   imgnum < EBookTocEntry::MAX_BUILTIN_ICONS)
                {
                    entry.iconid = (EBookTocEntry::Icon)imgnum;
                }
            }
        }
        else if(tagword == QLatin1String("ul"))  // increase indent level
        {
            // Fix for buggy help files
            if(++indent >= MAX_NEST_DEPTH)
            {
                qFatal("EBook_CHMImpl::ParseAndFillTopicsTree: max nest depth "
                       "(%d) is reached, error in help file",
                       MAX_NEST_DEPTH);
            }

            DEBUGPARSER(
                ("<ul>: new intent is %d\n", indent - root_indent_offset));
        }
        else if(tagword == QLatin1String("/ul"))  // decrease indent level
        {
            if(--indent < root_indent_offset)
            {
                indent = root_indent_offset;
            }

            DEBUGPARSER(
                ("</ul>: new intent is %d\n", indent - root_indent_offset));
        }

        pos = i;
    }

    // Dump our array
    //    for ( int i = 0; i < data.size(); i++ )
    //        qDebug() << data[i].indent << data[i].name << data[i].urls;

    return true;
}

bool EBook_CHM::ResolveObject(const QString& fileName, chmUnitInfo* ui) const
{
    return m_chmFile != nullptr &&
           ::chm_resolve_object(m_chmFile, qPrintable(fileName), ui) ==
               CHM_RESOLVE_SUCCESS;
}

bool EBook_CHM::hasFile(const QString& fileName) const
{
    chmUnitInfo ui;

    return m_chmFile != nullptr &&
           ::chm_resolve_object(m_chmFile, qPrintable(fileName), &ui) ==
               CHM_RESOLVE_SUCCESS;
}

size_t EBook_CHM::RetrieveObject(const chmUnitInfo* ui, unsigned char* buffer,
                                 LONGUINT64 fileOffset,
                                 LONGINT64 bufferSize) const
{
    return ::chm_retrieve_object(m_chmFile, const_cast<chmUnitInfo*>(ui),
                                 buffer, fileOffset, bufferSize);
}

bool EBook_CHM::getInfoFromWindows()
{
#define WIN_HEADER_LEN 0x08
    unsigned char buffer[BUF_SIZE];
    unsigned int factor;
    chmUnitInfo ui;
    long size = 0;

    if(ResolveObject(QStringLiteral("/#WINDOWS"), &ui))
    {
        if(!RetrieveObject(&ui, buffer, 0, WIN_HEADER_LEN))
        {
            return false;
        }

        unsigned int entries =
            get_int32_le(reinterpret_cast<unsigned int*>(buffer));
        unsigned int entry_size =
            get_int32_le(reinterpret_cast<unsigned int*>(buffer + 0x04));

        QVector<unsigned char> uptr(entries * entry_size);
        unsigned char* raw = (unsigned char*)uptr.data();

        if(!RetrieveObject(&ui, raw, 8, entries * entry_size))
        {
            return false;
        }

        if(!ResolveObject(QStringLiteral("/#STRINGS"), &ui))
        {
            return false;
        }

        for(unsigned int i = 0; i < entries; ++i)
        {
            unsigned int offset = i * entry_size;

            unsigned int off_title = get_int32_le(
                reinterpret_cast<unsigned int*>(raw + offset + 0x14));
            unsigned int off_home = get_int32_le(
                reinterpret_cast<unsigned int*>(raw + offset + 0x68));
            unsigned int off_hhc = get_int32_le(
                reinterpret_cast<unsigned int*>(raw + offset + 0x60));
            unsigned int off_hhk = get_int32_le(
                reinterpret_cast<unsigned int*>(raw + offset + 0x64));

            factor = off_title / 4096;

            if(size == 0)
            {
                size = RetrieveObject(&ui, buffer, factor * 4096, BUF_SIZE);
            }

            if(size && off_title)
            {
                m_title = QByteArray((const char*)(buffer + off_title % 4096));
            }

            if(factor != off_home / 4096)
            {
                factor = off_home / 4096;
                size = RetrieveObject(&ui, buffer, factor * 4096, BUF_SIZE);
            }

            if(size && off_home)
            {
                m_home = QByteArray("/") +
                         QByteArray((const char*)buffer + off_home % 4096);
            }

            if(factor != off_hhc / 4096)
            {
                factor = off_hhc / 4096;
                size = RetrieveObject(&ui, buffer, factor * 4096, BUF_SIZE);
            }

            if(size && off_hhc)
            {
                m_topicsFile = QByteArray("/") +
                               QByteArray((const char*)buffer + off_hhc % 4096);
            }

            if(factor != off_hhk / 4096)
            {
                factor = off_hhk / 4096;
                size = RetrieveObject(&ui, buffer, factor * 4096, BUF_SIZE);
            }

            if(size && off_hhk)
            {
                m_indexFile = QByteArray("/") +
                              QByteArray((const char*)buffer + off_hhk % 4096);
            }
        }
    }
    return true;
}

bool EBook_CHM::getInfoFromSystem()
{
    unsigned char buffer[BUF_SIZE];
    chmUnitInfo ui;

    int index = 0;
    unsigned char *cursor = nullptr, *p;
    unsigned short value = 0;
    long size = 0;

    // Run the first loop to detect the encoding. We need this, because title
    // could be already encoded in user encoding. Same for file names
    if(!ResolveObject(QStringLiteral("/#SYSTEM"), &ui))
    {
        return false;
    }

    // Can we pull BUFF_SIZE bytes of the #SYSTEM file?
    if((size = RetrieveObject(&ui, buffer, 4, BUF_SIZE)) == 0)
    {
        return false;
    }

    buffer[size - 1] = 0;

    // First loop to detect the encoding
    for(index = 0; index < (size - 1 - (long)sizeof(unsigned short));)
    {
        cursor = buffer + index;
        value = UINT16ARRAY(cursor);

        switch(value)
        {
        case 0:
            index += 2;
            cursor = buffer + index;

            if(m_topicsFile.isEmpty())
            {
                m_topicsFile = QByteArray("/") +
                               QByteArray((const char*)buffer + index + 2);
            }

            break;

        case 1:
            index += 2;
            cursor = buffer + index;

            if(m_indexFile.isEmpty())
            {
                m_indexFile = QByteArray("/") +
                              QByteArray((const char*)buffer + index + 2);
            }
            break;

        case 2:
            index += 2;
            cursor = buffer + index;

            if(m_home.isEmpty() || m_home == "/")
            {
                m_home = QByteArray("/") +
                         QByteArray((const char*)buffer + index + 2);
            }
            break;

        case 3:
            index += 2;
            cursor = buffer + index;
            m_title = QByteArray((const char*)(buffer + index + 2));
            break;

        case 4:
            index += 2;
            cursor = buffer + index;

            p = buffer + index + 2;
            m_detectedLCID = (short)(p[0] | (p[1] << 8));

            break;

        case 6:
            index += 2;
            cursor = buffer + index;

            if(m_topicsFile.isEmpty())
            {
                QString topicAttempt = QStringLiteral("/");
                topicAttempt +=
                    QString(QString::fromUtf8((const char*)buffer + index + 2));

                QString tmp = topicAttempt + QStringLiteral(".hhc");

                if(ResolveObject(tmp, &ui))
                {
                    m_topicsFile = qPrintable(tmp);
                }

                tmp = topicAttempt + QStringLiteral(".hhk");

                if(ResolveObject(tmp, &ui))
                {
                    m_indexFile = qPrintable(tmp);
                }
            }
            break;

        case 16:
            index += 2;
            cursor = buffer + index;

            m_font =
                QString(QString::fromUtf8((const char*)buffer + index + 2));
            break;

        default:
            index += 2;
            cursor = buffer + index;
        }

        value = UINT16ARRAY(cursor);
        index += value + 2;
    }

    return true;
}

QString EBook_CHM::getTopicByUrl(const QUrl& url)
{
    QMap<QUrl, QString>::const_iterator it = m_url2topics.constFind(url);

    if(it == m_url2topics.constEnd())
    {
        return QString();
    }

    return it.value();
}

static int chm_enumerator_callback(struct chmFile*, struct chmUnitInfo* ui,
                                   void* context)
{
    EBook_CHM tmp;
    ((QList<QUrl>*)context)
        ->push_back(tmp.pathToUrl(QString::fromUtf8(ui->path)));
    return CHM_ENUMERATOR_CONTINUE;
}

bool EBook_CHM::enumerateFiles(QList<QUrl>& files)
{
    files.clear();
    return chm_enumerate(m_chmFile, CHM_ENUMERATE_ALL, chm_enumerator_callback,
                         &files);
}

QString EBook_CHM::currentEncoding() const
{
    return m_currentEncoding;
}

bool EBook_CHM::setCurrentEncoding(const char* encoding)
{
    m_currentEncoding = QString::fromUtf8(encoding);
    return changeFileEncoding(m_currentEncoding);
}

bool EBook_CHM::isSupportedUrl(const QUrl& url)
{
    return url.scheme() == URL_SCHEME_CHM;
}

bool EBook_CHM::guessTextEncoding()
{
    if(!m_detectedLCID)
    {
        qWarning("Could not detect LCID");
        return false;
    }

    QString enc = Ebook_CHM_Encoding::guessByLCID(m_detectedLCID);

    if(changeFileEncoding(enc))
    {
        m_currentEncoding = enc;
        return true;
    }

    return false;
}

bool EBook_CHM::changeFileEncoding(const QString& qtencoding)
{
    // Encoding could be either simple Qt codepage, or set like CP1251/KOI8,
    // which allows to set up encodings separately for text (first) and internal
    // files (second)
    int p = qtencoding.indexOf(QLatin1Char('/'));

    if(p != -1)
    {
        QString global = qtencoding.left(p);
        QString special = qtencoding.mid(p + 1);

        m_textCodec = QTextCodec::codecForName(global.toUtf8());

        if(!m_textCodec)
        {
            qWarning("Could not set up Text Codec for encoding '%s'",
                     qPrintable(global));
            return false;
        }

        m_textCodecForSpecialFiles = QTextCodec::codecForName(special.toUtf8());

        if(!m_textCodecForSpecialFiles)
        {
            qWarning("Could not set up Text Codec for encoding '%s'",
                     qPrintable(special));
            return false;
        }
    }
    else
    {
        m_textCodecForSpecialFiles = m_textCodec =
            QTextCodec::codecForName(qtencoding.toUtf8());

        if(!m_textCodec)
        {
            qWarning("Could not set up Text Codec for encoding '%s'",
                     qPrintable(qtencoding));
            return false;
        }
    }

    m_htmlEntityDecoder.changeEncoding(m_textCodec);
    return true;
}

void EBook_CHM::fillTopicsUrlMap()
{
    if(!m_lookupTablesValid)
    {
        return;
    }

    // Read those tables
    QVector<unsigned char> topics(m_chmTOPICS.length),
        urltbl(m_chmURLTBL.length), urlstr(m_chmURLSTR.length),
        strings(m_chmSTRINGS.length);

    if(!RetrieveObject(&m_chmTOPICS, (unsigned char*)topics.data(), 0,
                       m_chmTOPICS.length) ||
       !RetrieveObject(&m_chmURLTBL, (unsigned char*)urltbl.data(), 0,
                       m_chmURLTBL.length) ||
       !RetrieveObject(&m_chmURLSTR, (unsigned char*)urlstr.data(), 0,
                       m_chmURLSTR.length) ||
       !RetrieveObject(&m_chmSTRINGS, (unsigned char*)strings.data(), 0,
                       m_chmSTRINGS.length))
    {
        return;
    }

    for(LONGUINT64 i = 0; i < m_chmTOPICS.length; i += TOPICS_ENTRY_LEN)
    {
        unsigned int off_title = get_int32_le(
            reinterpret_cast<unsigned int*>(topics.data() + i + 4));
        unsigned int off_url = get_int32_le(
            reinterpret_cast<unsigned int*>(topics.data() + i + 8));
        off_url = get_int32_le(reinterpret_cast<unsigned int*>(urltbl.data() +
                                                               off_url + 8)) +
                  8;

        QUrl url =
            pathToUrl(QString::fromUtf8((const char*)urlstr.data() + off_url));

        if(off_title < (unsigned int)strings.size())
        {
            m_url2topics[url] =
                encodeWithCurrentCodec((const char*)strings.data() + off_title);
        }
        else
        {
            m_url2topics[url] = QStringLiteral("Untitled");
        }
    }
}

bool EBook_CHM::parseBinaryTOC(QList<EBookTocEntry>& toc) const
{
    if(!m_lookupTablesValid)
    {
        return false;
    }

    QByteArray tocidx, topics, urltbl, urlstr, strings;

    // Read the index tables
    if(!getBinaryContent(tocidx, QStringLiteral("/#TOCIDX")) ||
       !getBinaryContent(topics, QStringLiteral("/#TOPICS")) ||
       !getBinaryContent(urltbl, QStringLiteral("/#URLTBL")) ||
       !getBinaryContent(urlstr, QStringLiteral("/#URLSTR")) ||
       !getBinaryContent(strings, QStringLiteral("/#STRINGS")))
    {
        return false;
    }

    // Shamelessly stolen from xchm
    if(!RecurseLoadBTOC(tocidx, topics, urltbl, urlstr, strings,
                        UINT32ARRAY(tocidx.data()), toc, 0))
    {
        qWarning("Failed to parse binary TOC, fallback to text-based TOC");
        toc.clear();
        return false;
    }

    return true;
}

//
// This piece of code was based on the one in xchm written by  Razvan Cojocaru
// <razvanco@gmx.net>
//
bool EBook_CHM::RecurseLoadBTOC(const QByteArray& tocidx,
                                const QByteArray& topics,
                                const QByteArray& urltbl,
                                const QByteArray& urlstr,
                                const QByteArray& strings, int offset,
                                QList<EBookTocEntry>& entries, int level) const
{
    while(offset)
    {
        // If this is end of TOCIDX, return.
        if(tocidx.size() < offset + 20)
        {
            return true;
        }

        unsigned int flags = UINT32ARRAY(tocidx.data() + offset + 4);
        int index = UINT32ARRAY(tocidx.data() + offset + 8);

        if((flags & 0x04) || (flags & 0x08))
        {
            QString name, value;

            if((flags & 0x08) == 0)
            {
                if(strings.size() < index + 1)
                {
                    qWarning("EBook_CHM::RecurseLoadBTOC: invalid name index "
                             "(%d) for book TOC entry!",
                             index);
                    return false;
                }

                name = encodeWithCurrentCodec(strings.data() + index);
            }
            else
            {
                if(topics.size() < (index * 16) + 12)
                {
                    qWarning("EBook_CHM::RecurseLoadBTOC: invalid name index "
                             "(%d) for local TOC entry!",
                             index);
                    return false;
                }

                int tocoffset =
                    (int)UINT32ARRAY(topics.data() + (index * 16) + 4);

                if(strings.size() < tocoffset + 1)
                {
                    qWarning("EBook_CHM::RecurseLoadBTOC: invalid name "
                             "tocoffset (%d) for TOC entry!",
                             tocoffset);
                    return false;
                }

                if(tocoffset < 0)
                {
                    name.clear();
                }
                else
                {
                    name = encodeWithCurrentCodec(strings.data() + tocoffset);
                }

                // #URLTBL index
                tocoffset = (int)UINT32ARRAY(topics.data() + (index * 16) + 8);

                if(tocoffset < 0 || urltbl.size() < tocoffset + 12)
                {
                    qWarning("EBook_CHM::RecurseLoadBTOC: invalid url index "
                             "(%d) for TOC entry!",
                             tocoffset);
                    return false;
                }

                tocoffset = (int)UINT32ARRAY(urltbl.data() + tocoffset + 8);

                if(tocoffset < 0 || urlstr.size() < tocoffset)
                {
                    qWarning("EBook_CHM::RecurseLoadBTOC: invalid url offset "
                             "(%d) for TOC entry!",
                             tocoffset);
                    return false;
                }

                value = encodeWithCurrentCodec(urlstr.data() + tocoffset + 8);
            }

            EBookTocEntry entry;
            entry.name = name.trimmed();

            if(!entry.name.isEmpty())
            {
                if(!value.isEmpty())
                {
                    entry.url = pathToUrl(value);
                }

                entry.iconid = EBookTocEntry::IMAGE_AUTO;
                entry.indent = level;
                entries.push_back(entry);
            }
        }

        if(flags & 0x04)
        {
            // book
            if(tocidx.size() < offset + 24)
            {
                qWarning("EBook_CHM::RecurseLoadBTOC: invalid child entry "
                         "offset (%d)",
                         offset);
                return false;
            }

            unsigned int childoffset = UINT32ARRAY(tocidx.data() + offset + 20);

            if(childoffset)
            {
                if(!RecurseLoadBTOC(tocidx, topics, urltbl, urlstr, strings,
                                    childoffset, entries, level + 1))
                {
                    return false;
                }
            }
        }

        offset = UINT32ARRAY(tocidx.data() + offset + 0x10);
    }

    return true;
}

bool EBook_CHM::hasOption(const QString& name) const
{
    if(!m_envOptions.isEmpty() && m_envOptions.contains(name))
    {
        return true;
    }

    return false;
}

QUrl EBook_CHM::pathToUrl(const QString& link) const
{
    if(link.startsWith(QLatin1String("http://")) ||
       link.startsWith(QLatin1String("https://")))
    {
        return QUrl(link);
    }

    QUrl url;
    url.setScheme(URL_SCHEME_CHM);
    url.setHost(URL_SCHEME_CHM);

    // Does the link contain the fragment as well?
    int off = link.indexOf(QLatin1Char('#'));
    QString path;

    if(off != -1)
    {
        path = link.left(off);
        url.setFragment(link.mid(off + 1));
    }
    else
    {
        path = link;
    }

    if(!path.startsWith(QLatin1Char('/')))
    {
        path.prepend(QLatin1Char('/'));
    }

    url.setPath(QUrl::fromPercentEncoding(path.toUtf8()));
    return url;
}

QString EBook_CHM::urlToPath(const QUrl& link) const
{
    if(link.scheme() == URL_SCHEME_CHM)
    {
        if(link.path() == QLatin1String("/") || link.path().isEmpty())
        {
            return QString::fromUtf8(m_home);
        }

        return link.path();
    }

    return QLatin1String("");
}

EBook_CHM::ParsedEntry::ParsedEntry()
{
    iconid = 0;
    indent = 0;
}
