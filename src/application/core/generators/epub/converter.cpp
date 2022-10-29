/*
    SPDX-FileCopyrightText: 2008 Ely Levy <elylevy@cs.huji.ac.il>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "converter.h"
#include <core/action.h>
#include <core/annotations.h>
#include <core/movie.h>
#include <core/sound.h>
#include <KLocalizedString>
#include <QAbstractTextDocumentLayout>
#include <QApplication>  // Because of the HACK
#include <QDebug>
#include <QFileInfo>
#include <QStandardPaths>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QTextFrame>

using namespace Epub;

Converter::Converter() :
    mTextDocument(nullptr)
{
}

Converter::~Converter()
{
}

// join the char * array into one QString
QString _strPack(char** str, int size)
{
    QString res;

    res = QString::fromUtf8(str[0]);

    for(int i = 1; i < size; i++)
    {
        res += QLatin1String(", ");
        res += QString::fromUtf8(str[i]);
    }

    return res;
}

// Q_EMIT data wrap function that map between epub metadata to okular's
void Converter::_emitData(Okular::DocumentInfo::Key key,
                          enum epub_metadata type)
{
    int size;
    unsigned char** data;

    data = epub_get_metadata(mTextDocument->getEpub(), type, &size);

    if(data)
    {
        Q_EMIT addMetaData(key, _strPack((char**)data, size));
        for(int i = 0; i < size; i++)
        {
            free(data[i]);
        }
        free(data);
    }
}

// Got over the blocks from start and add them to hashes use name as the
// prefix for local links
void Converter::_handle_anchors(const QTextBlock& start, const QString& name)
{
    const QString curDir = QFileInfo(name).path();

    for(QTextBlock bit = start; bit != mTextDocument->end(); bit = bit.next())
    {
        for(QTextBlock::iterator fit = bit.begin(); !(fit.atEnd()); ++fit)
        {
            QTextFragment frag = fit.fragment();

            if(frag.isValid() && frag.charFormat().isAnchor())
            {
                QString hrefString = frag.charFormat().anchorHref();

                // remove ./ or ../
                // making it easier to compare, with links
                while(!hrefString.isNull() &&
                      (hrefString.at(0) == QLatin1Char('.') ||
                       hrefString.at(0) == QLatin1Char('/')))
                {
                    hrefString.remove(0, 1);
                }

                QUrl href(hrefString);
                if(href.isValid() && !href.isEmpty())
                {
                    if(href.isRelative())
                    {  // Inside document link
                        if(!hrefString.indexOf(QLatin1Char('#')))
                        {
                            hrefString = name + hrefString;
                        }
                        else if(QFileInfo(hrefString).path() ==
                                    QLatin1String(".") &&
                                curDir != QLatin1String("."))
                        {
                            hrefString = curDir + QLatin1Char('/') + hrefString;
                        }

                        // QTextCharFormat sometimes splits a link in two
                        // if there's no white space between words & the first
                        // one is an anchor consider whole word to be an anchor
                        ++fit;
                        int fragLen = frag.length();
                        if(!fit.atEnd() &&
                           ((fit.fragment().position() - frag.position()) == 1))
                        {
                            fragLen += fit.fragment().length();
                        }
                        --fit;

                        _insert_local_links(
                            hrefString,
                            QPair<int, int>(frag.position(),
                                            frag.position() + fragLen));
                    }
                    else
                    {  // Outside document link
                        Okular::BrowseAction* action =
                            new Okular::BrowseAction(QUrl(href.toString()));

                        Q_EMIT addAction(action, frag.position(),
                                         frag.position() + frag.length());
                    }
                }

                const QStringList& names = frag.charFormat().anchorNames();
                if(!names.empty())
                {
                    for(QStringList::const_iterator lit = names.constBegin();
                        lit != names.constEnd(); ++lit)
                    {
                        mSectionMap.insert(name + QLatin1Char('#') + *lit, bit);
                    }
                }

            }  // end anchor case
        }
    }
}

void Converter::_insert_local_links(const QString& key,
                                    const QPair<int, int> value)
{
    if(mLocalLinks.contains(key))
    {
        mLocalLinks[key].append(value);
    }
    else
    {
        QVector<QPair<int, int>> vec;
        vec.append(value);
        mLocalLinks.insert(key, vec);
    }
}

static QPoint calculateXYPosition(QTextDocument* document, int startPosition)
{
    const QTextBlock startBlock = document->findBlock(startPosition);
    const QRectF startBoundingRect =
        document->documentLayout()->blockBoundingRect(startBlock);

    QTextLayout* startLayout = startBlock.layout();
    if(!startLayout)
    {
        qWarning() << "Start layout not found" << startLayout;
        return QPoint();
    }

    int startPos = startPosition - startBlock.position();
    const QTextLine startLine = startLayout->lineForTextPosition(startPos);

    double x = startBoundingRect.x();
    double y = startBoundingRect.y() + startLine.y();

    y = (int)y % 800;

    return QPoint(x, y);
}

QTextDocument* Converter::convert(const QString& fileName)
{
    EpubDocument* newDocument =
        new EpubDocument(fileName, generator()->generalSettings()->font());
    if(!newDocument->isValid())
    {
        Q_EMIT error(i18n("Error while opening the EPub document."), -1);
        delete newDocument;
        return nullptr;
    }
    mTextDocument = newDocument;

    QTextCursor* _cursor = new QTextCursor(mTextDocument);

    mLocalLinks.clear();
    mSectionMap.clear();

    // Emit the document meta data
    _emitData(Okular::DocumentInfo::Title, EPUB_TITLE);
    _emitData(Okular::DocumentInfo::Author, EPUB_CREATOR);
    _emitData(Okular::DocumentInfo::Subject, EPUB_SUBJECT);
    _emitData(Okular::DocumentInfo::Creator, EPUB_PUBLISHER);

    _emitData(Okular::DocumentInfo::Description, EPUB_DESCRIPTION);

    _emitData(Okular::DocumentInfo::CreationDate, EPUB_DATE);
    _emitData(Okular::DocumentInfo::Category, EPUB_TYPE);
    _emitData(Okular::DocumentInfo::Copyright, EPUB_RIGHTS);
    Q_EMIT addMetaData(Okular::DocumentInfo::MimeType,
                       QStringLiteral("application/epub+zip"));

    struct eiterator* it;

    // iterate over the book
    it = epub_get_iterator(mTextDocument->getEpub(), EITERATOR_SPINE, 0);

    // if the background color of the document is non-white it will be handled
    // by QTextDocument::setHtml()
    bool firstPage = true;
    QVector<Okular::MovieAnnotation*> movieAnnots;
    QVector<Okular::SoundAction*> soundActions;

    // HACK BEGIN Get the links without CSS to be blue
    //            Remove if Qt ever gets fixed and the code in
    //            textdocumentgenerator.cpp works
    const QPalette orig = qApp->palette();
    QPalette p = orig;
    p.setColor(QPalette::Link, Qt::blue);
    // HACK END

    const QSize videoSize(320, 240);
    do
    {
        if(!epub_it_get_curr(it))
        {
            continue;
        }

        movieAnnots.clear();
        soundActions.clear();

        const QString link = QString::fromUtf8(epub_it_get_curr_url(it));
        mTextDocument->setCurrentSubDocument(link);
        QString htmlContent = QString::fromUtf8(epub_it_get_curr(it));

        // as QTextCharFormat::anchorNames() ignores sections, replace it with
        // <p>
        htmlContent.replace(QRegExp(QStringLiteral("< *section")),
                            QStringLiteral("<p"));
        htmlContent.replace(QRegExp(QStringLiteral("< */ *section")),
                            QStringLiteral("</p"));

        // convert svg tags to img
        const int maxHeight = mTextDocument->maxContentHeight();
        const int maxWidth = mTextDocument->maxContentWidth();
        QDomDocument dom;
        if(dom.setContent(htmlContent))
        {
            QDomNodeList svgs = dom.elementsByTagName(QStringLiteral("svg"));
            if(!svgs.isEmpty())
            {
                QList<QDomNode> imgNodes;
                for(int i = 0; i < svgs.length(); ++i)
                {
                    QDomNodeList images =
                        svgs.at(i).toElement().elementsByTagName(
                            QStringLiteral("image"));
                    for(int j = 0; j < images.length(); ++j)
                    {
                        QString lnk = images.at(i).toElement().attribute(
                            QStringLiteral("xlink:href"));
                        int ht = images.at(i)
                                     .toElement()
                                     .attribute(QStringLiteral("height"))
                                     .toInt();
                        int wd = images.at(i)
                                     .toElement()
                                     .attribute(QStringLiteral("width"))
                                     .toInt();
                        QImage img =
                            mTextDocument
                                ->loadResource(QTextDocument::ImageResource,
                                               QUrl(lnk))
                                .value<QImage>();
                        if(ht == 0)
                        {
                            ht = img.height();
                        }
                        if(wd == 0)
                        {
                            wd = img.width();
                        }
                        if(ht > maxHeight)
                        {
                            ht = maxHeight;
                        }
                        if(wd > maxWidth)
                        {
                            wd = maxWidth;
                        }
                        mTextDocument->addResource(QTextDocument::ImageResource,
                                                   QUrl(lnk), img);
                        QDomDocument newDoc;
                        newDoc.setContent(
                            QStringLiteral(
                                "<img src=\"%1\" height=\"%2\" width=\"%3\" />")
                                .arg(lnk)
                                .arg(ht)
                                .arg(wd));
                        imgNodes.append(newDoc.documentElement());
                    }
                    for(const QDomNode& nd : qAsConst(imgNodes))
                    {
                        svgs.at(i).parentNode().replaceChild(nd, svgs.at(i));
                    }
                }
            }

            // handle embedded videos
            QDomNodeList videoTags =
                dom.elementsByTagName(QStringLiteral("video"));
            while(!videoTags.isEmpty())
            {
                QDomNodeList sourceTags =
                    videoTags.at(0).toElement().elementsByTagName(
                        QStringLiteral("source"));
                if(!sourceTags.isEmpty())
                {
                    QString lnk = sourceTags.at(0).toElement().attribute(
                        QStringLiteral("src"));

                    Okular::Movie* movie = new Okular::Movie(
                        mTextDocument
                            ->loadResource(EpubDocument::MovieResource,
                                           QUrl(lnk))
                            .toString());
                    movie->setSize(videoSize);
                    movie->setShowControls(true);

                    Okular::MovieAnnotation* annot =
                        new Okular::MovieAnnotation;
                    annot->setMovie(movie);

                    movieAnnots.push_back(annot);
                    QDomDocument tempDoc;
                    tempDoc.setContent(QStringLiteral(
                        "<pre>&lt;video&gt;&lt;/video&gt;</pre>"));
                    videoTags.at(0).parentNode().replaceChild(
                        tempDoc.documentElement(), videoTags.at(0));
                }
            }

            // handle embedded audio
            QDomNodeList audioTags =
                dom.elementsByTagName(QStringLiteral("audio"));
            while(!audioTags.isEmpty())
            {
                QDomElement element = audioTags.at(0).toElement();
                bool repeat = element.hasAttribute(QStringLiteral("loop"));
                QString lnk = element.attribute(QStringLiteral("src"));

                Okular::Sound* sound = new Okular::Sound(
                    mTextDocument
                        ->loadResource(EpubDocument::AudioResource, QUrl(lnk))
                        .toByteArray());

                Okular::SoundAction* soundAction =
                    new Okular::SoundAction(1.0, true, repeat, false, sound);
                soundActions.push_back(soundAction);

                QDomDocument tempDoc;
                tempDoc.setContent(
                    QStringLiteral("<pre>&lt;audio&gt;&lt;/audio&gt;</pre>"));
                audioTags.at(0).parentNode().replaceChild(
                    tempDoc.documentElement(), audioTags.at(0));
            }
            htmlContent = dom.toString();
        }

        // HACK BEGIN
        qApp->setPalette(p);
        // HACK END

        QTextBlock before;
        if(firstPage)
        {
            mTextDocument->setHtml(htmlContent);
            firstPage = false;
            before = mTextDocument->begin();
        }
        else
        {
            before = _cursor->block();
            _cursor->insertHtml(htmlContent);
        }
        // HACK BEGIN
        qApp->setPalette(orig);
        // HACK END

        QTextCursor csr(before);  // a temporary cursor pointing at the begin of
                                  // the last inserted block
        int index = 0;

        while(
            !movieAnnots.isEmpty() &&
            !(csr = mTextDocument->find(QStringLiteral("<video></video>"), csr))
                 .isNull())
        {
            const int posStart = csr.position();
            const QPoint startPoint =
                calculateXYPosition(mTextDocument, posStart);
            QImage img(QStandardPaths::locate(
                QStandardPaths::GenericDataLocation,
                QStringLiteral("okular/pics/okular-epub-movie.png")));
            img = img.scaled(videoSize);
            csr.insertImage(img);
            const int posEnd = csr.position();
            const QRect videoRect(startPoint, videoSize);
            movieAnnots[index]->setBoundingRectangle(Okular::NormalizedRect(
                videoRect, mTextDocument->pageSize().width(),
                mTextDocument->pageSize().height()));
            Q_EMIT addAnnotation(movieAnnots[index++], posStart, posEnd);
            csr.movePosition(QTextCursor::NextWord);
        }

        csr = QTextCursor(before);
        index = 0;
        const QString keyToSearch(QStringLiteral("<audio></audio>"));
        while(!soundActions.isEmpty() &&
              !(csr = mTextDocument->find(keyToSearch, csr)).isNull())
        {
            const int posStart = csr.position() - keyToSearch.size();
            const QImage img(QStandardPaths::locate(
                QStandardPaths::GenericDataLocation,
                QStringLiteral("okular/pics/okular-epub-sound-icon.png")));
            csr.insertImage(img);
            const int posEnd = csr.position();
            qDebug() << posStart << posEnd;
            ;
            Q_EMIT addAction(soundActions[index++], posStart, posEnd);
            csr.movePosition(QTextCursor::NextWord);
        }

        mSectionMap.insert(link, before);

        _handle_anchors(before, link);

        const int page = mTextDocument->pageCount();

        // it will clear the previous format
        // useful when the last line had a bullet
        _cursor->insertBlock(QTextBlockFormat());

        while(mTextDocument->pageCount() == page)
        {
            _cursor->insertText(QStringLiteral("\n"));
        }
    }
    while(epub_it_get_next(it));

    epub_free_iterator(it);

    // handle toc
    struct titerator* tit;

    // FIXME: support other method beside NAVMAP and GUIDE
    tit = epub_get_titerator(mTextDocument->getEpub(), TITERATOR_NAVMAP, 0);
    if(!tit)
    {
        tit = epub_get_titerator(mTextDocument->getEpub(), TITERATOR_GUIDE, 0);
    }

    if(tit)
    {
        do
        {
            if(epub_tit_curr_valid(tit))
            {
                char* clink = epub_tit_get_curr_link(tit);
                QString link = QString::fromUtf8(clink);
                char* label = epub_tit_get_curr_label(tit);
                QTextBlock block =
                    mTextDocument->begin();  // must point somewhere

                if(mSectionMap.contains(link))
                {
                    block = mSectionMap.value(link);
                }
                else
                {  // load missing resource
                    char* data = nullptr;
                    // epub_get_data can't handle whitespace url encodings
                    QByteArray ba =
                        link.replace(QLatin1String("%20"), QLatin1String(" "))
                            .toLatin1();
                    const char* clinkClean = ba.data();
                    int size = epub_get_data(mTextDocument->getEpub(),
                                             clinkClean, &data);

                    if(data)
                    {
                        _cursor->insertBlock();

                        // try to load as image and if not load as html
                        block = _cursor->block();
                        QImage image;
                        mSectionMap.insert(link, block);
                        if(image.loadFromData((unsigned char*)data, size))
                        {
                            mTextDocument->addResource(
                                QTextDocument::ImageResource, QUrl(link),
                                image);
                            _cursor->insertImage(link);
                        }
                        else
                        {
                            _cursor->insertHtml(QString::fromUtf8(data));
                            // Add anchors to hashes
                            _handle_anchors(block, link);
                        }

                        // Start new file in a new page
                        int page = mTextDocument->pageCount();
                        while(mTextDocument->pageCount() == page)
                        {
                            _cursor->insertText(QStringLiteral("\n"));
                        }
                    }

                    free(data);
                }

                if(block.isValid())
                {  // be sure we actually got a block
                    Q_EMIT addTitle(epub_tit_get_curr_depth(tit),
                                    QString::fromUtf8(label), block);
                }
                else
                {
                    qDebug() << "Error: no block found for" << link;
                }

                if(clink)
                {
                    free(clink);
                }
                if(label)
                {
                    free(label);
                }
            }
        }
        while(epub_tit_next(tit));

        epub_free_titerator(tit);
    }
    else
    {
        qDebug() << "no toc found";
    }

    // adding link actions
    QHashIterator<QString, QVector<QPair<int, int>>> hit(mLocalLinks);
    while(hit.hasNext())
    {
        hit.next();

        const QTextBlock block = mSectionMap.value(hit.key());

        for(int i = 0; i < hit.value().size(); ++i)
        {
            if(block.isValid())
            {  // be sure we actually got a block
                Okular::DocumentViewport viewport =
                    calculateViewport(mTextDocument, block);

                Okular::GotoAction* action =
                    new Okular::GotoAction(QString(), viewport);

                Q_EMIT addAction(action, hit.value()[i].first,
                                 hit.value()[i].second);
            }
            else
            {
                qDebug() << "Error: no block found for " << hit.key();
            }
        }
    }

    delete _cursor;

    return mTextDocument;
}
