/*
    SPDX-FileCopyrightText: 2006-2009 Luigi Toscano <luigi.toscano@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_dvi.h"
#include <core/action.h>
#include <core/document.h>
#include <core/fileprinter.h>
#include <core/page.h>
#include <core/sourcereference.h>
#include <core/textpage.h>
#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMutex>
#include <QStack>
#include <QString>
#include <QTemporaryFile>
#include <QUrl>
#include <QVector>
#include "TeXFont.h"
#include "debug_dvi.h"
#include "dviFile.h"
#include "dviPageInfo.h"
#include "dviRenderer.h"
#include "dviexport.h"
#include "pageSize.h"

#ifdef DVI_OPEN_BUSYLOOP
    #include <QThread>
#endif

OKULAR_EXPORT_PLUGIN(DviGenerator, "libokularGenerator_dvi.json")

DviGenerator::DviGenerator(QObject* parent, const QVariantList& args) :
    Okular::Generator(parent, args),
    m_fontExtracted(false),
    m_docSynopsis(nullptr),
    m_dviRenderer(nullptr)
{
    setFeature(Threaded);
    setFeature(TextExtraction);
    setFeature(FontInfo);
    setFeature(PrintPostscript);
    if(Okular::FilePrinter::ps2pdfAvailable())
    {
        setFeature(PrintToFile);
    }
}

bool DviGenerator::loadDocument(const QString& fileName,
                                QVector<Okular::Page*>& pagesVector)
{
    // qCDebug(OkularDviDebug) << "file:" << fileName;
    QUrl base(QUrl::fromLocalFile(fileName));

    (void)userMutex();

    m_dviRenderer = new dviRenderer(
        documentMetaData(TextHintingMetaData, QVariant()).toBool());
    connect(m_dviRenderer, &dviRenderer::error, this, &DviGenerator::error);
    connect(m_dviRenderer, &dviRenderer::warning, this, &DviGenerator::warning);
    connect(m_dviRenderer, &dviRenderer::notice, this, &DviGenerator::notice);
#ifdef DVI_OPEN_BUSYLOOP
    static const ushort s_waitTime = 800;  // milliseconds
    static const int s_maxIterations = 10;
    int iter = 0;
    for(; !m_dviRenderer->isValidFile(fileName) && iter < s_maxIterations;
        ++iter)
    {
        qCDebug(OkularDviDebug).nospace()
            << "file not valid after iteration #" << iter << "/"
            << s_maxIterations << ", waiting for " << s_waitTime;
        QThread::msleep(s_waitTime);
    }
    if(iter >= s_maxIterations && !m_dviRenderer->isValidFile(fileName))
    {
        qCDebug(OkularDviDebug)
            << "file still not valid after" << s_maxIterations;
        delete m_dviRenderer;
        m_dviRenderer = 0;
        return false;
    }
#else
    if(!m_dviRenderer->isValidFile(fileName))
    {
        delete m_dviRenderer;
        m_dviRenderer = nullptr;
        return false;
    }
#endif
    if(!m_dviRenderer->setFile(fileName, base))
    {
        delete m_dviRenderer;
        m_dviRenderer = nullptr;
        return false;
    }

    qCDebug(OkularDviDebug)
        << "# of pages:" << m_dviRenderer->dviFile->total_pages;

    m_resolution = dpi().height();
    loadPages(pagesVector);

    return true;
}

bool DviGenerator::doCloseDocument()
{
    delete m_docSynopsis;
    m_docSynopsis = nullptr;
    delete m_dviRenderer;
    m_dviRenderer = nullptr;

    m_linkGenerated.clear();
    m_fontExtracted = false;

    return true;
}

void DviGenerator::fillViewportFromAnchor(Okular::DocumentViewport& vp,
                                          const Anchor anch,
                                          const Okular::Page* page) const
{
    fillViewportFromAnchor(vp, anch, page->width(), page->height());
}

void DviGenerator::fillViewportFromAnchor(Okular::DocumentViewport& vp,
                                          const Anchor anch, int pW,
                                          int pH) const
{
    vp.pageNumber = static_cast<quint16>(anch.page) - 1;

    SimplePageSize ps = m_dviRenderer->sizeOfPage(PageNumber(vp.pageNumber));
    double resolution = 0;
    if(ps.isValid())
    {
        resolution = (double)(pW) / ps.width().getLength_in_inch();
    }
    else
    {
        resolution = m_resolution;
    }

    double py =
        (double)anch.distance_from_top.getLength_in_inch() * resolution + 0.5;

    vp.rePos.normalizedX = 0.5;
    vp.rePos.normalizedY = py / (double)pH;
    vp.rePos.enabled = true;
    vp.rePos.pos = Okular::DocumentViewport::Center;
}

QList<Okular::ObjectRect*> DviGenerator::generateDviLinks(
    const dviPageInfo* pageInfo)
{
    QList<Okular::ObjectRect*> dviLinks;

    int pageWidth = pageInfo->width, pageHeight = pageInfo->height;

    for(const Hyperlink& dviLink : qAsConst(pageInfo->hyperLinkList))
    {
        QRect boxArea = dviLink.box;
        double nl = (double)boxArea.left() / pageWidth,
               nt = (double)boxArea.top() / pageHeight,
               nr = (double)boxArea.right() / pageWidth,
               nb = (double)boxArea.bottom() / pageHeight;

        QString linkText = dviLink.linkText;
        if(linkText.startsWith(QLatin1String("#")))
        {
            linkText = linkText.mid(1);
        }
        Anchor anch = m_dviRenderer->findAnchor(linkText);

        Okular::Action* okuLink = nullptr;

        /* distinguish between local (-> anchor) and remote links */
        if(anch.isValid())
        {
            /* internal link */
            Okular::DocumentViewport vp;
            fillViewportFromAnchor(vp, anch, pageWidth, pageHeight);

            okuLink = new Okular::GotoAction(QLatin1String(""), vp);
        }
        else
        {
            okuLink =
                new Okular::BrowseAction(QUrl::fromUserInput(dviLink.linkText));
        }
        if(okuLink)
        {
            Okular::ObjectRect* orlink = new Okular::ObjectRect(
                nl, nt, nr, nb, false, Okular::ObjectRect::Action, okuLink);
            dviLinks.push_front(orlink);
        }
    }
    return dviLinks;
}

QImage DviGenerator::image(Okular::PixmapRequest* request)
{
    dviPageInfo* pageInfo = new dviPageInfo();
    pageSize ps;
    QImage ret;

    pageInfo->width = request->width();
    pageInfo->height = request->height();

    pageInfo->pageNumber = request->pageNumber() + 1;

    //  pageInfo->resolution = m_resolution;

    QMutexLocker lock(userMutex());

    if(m_dviRenderer)
    {
        SimplePageSize s = m_dviRenderer->sizeOfPage(pageInfo->pageNumber);

        /*       if ( s.width() != pageInfo->width) */
        //   if (!useDocumentSpecifiedSize)
        //    s = userPreferredSize;

        if(s.isValid())
        {
            pageInfo->resolution =
                (double)(pageInfo->width) / s.width().getLength_in_inch();
        }
        else
        {
            pageInfo->resolution =
                (double)(pageInfo->width) / ps.width().getLength_in_inch();
        }

        m_dviRenderer->drawPage(pageInfo);

        if(!pageInfo->img.isNull())
        {
            qCDebug(OkularDviDebug) << "Image OK";

            ret = pageInfo->img;

            if(!m_linkGenerated[request->pageNumber()])
            {
                request->page()->setObjectRects(generateDviLinks(pageInfo));
                m_linkGenerated[request->pageNumber()] = true;
            }
        }
    }

    lock.unlock();

    delete pageInfo;

    return ret;
}

Okular::TextPage* DviGenerator::textPage(Okular::TextRequest* request)
{
    const Okular::Page* page = request->page();

    qCDebug(OkularDviDebug);
    dviPageInfo* pageInfo = new dviPageInfo();
    pageSize ps;

    pageInfo->width = page->width();
    pageInfo->height = page->height();

    pageInfo->pageNumber = page->number() + 1;

    pageInfo->resolution = m_resolution;

    QMutexLocker lock(userMutex());

    // get page text from m_dviRenderer
    Okular::TextPage* ktp = nullptr;
    if(m_dviRenderer)
    {
        SimplePageSize s = m_dviRenderer->sizeOfPage(pageInfo->pageNumber);
        pageInfo->resolution =
            (double)(pageInfo->width) / ps.width().getLength_in_inch();

        m_dviRenderer->getText(pageInfo);
        lock.unlock();

        ktp = extractTextFromPage(pageInfo);
    }
    delete pageInfo;
    return ktp;
}

Okular::TextPage* DviGenerator::extractTextFromPage(dviPageInfo* pageInfo)
{
    QList<Okular::TextEntity*> textOfThePage;

    QVector<TextBox>::ConstIterator it = pageInfo->textBoxList.constBegin();
    QVector<TextBox>::ConstIterator itEnd = pageInfo->textBoxList.constEnd();

    int pageWidth = pageInfo->width, pageHeight = pageInfo->height;

    for(; it != itEnd; ++it)
    {
        TextBox curTB = *it;
        textOfThePage.push_back(new Okular::TextEntity(
            curTB.text,
            new Okular::NormalizedRect(curTB.box, pageWidth, pageHeight)));
    }

    Okular::TextPage* ktp = new Okular::TextPage(textOfThePage);

    return ktp;
}

Okular::DocumentInfo DviGenerator::generateDocumentInfo(
    const QSet<Okular::DocumentInfo::Key>& keys) const
{
    Okular::DocumentInfo docInfo;

    if(keys.contains(Okular::DocumentInfo::MimeType))
    {
        docInfo.set(Okular::DocumentInfo::MimeType,
                    QStringLiteral("application/x-dvi"));
    }

    QMutexLocker lock(userMutex());

    if(m_dviRenderer && m_dviRenderer->dviFile)
    {
        dvifile* dvif = m_dviRenderer->dviFile;

        // read properties from dvif
        // docInfo.set( "filename", dvif->filename, i18n("Filename") );
        if(keys.contains(Okular::DocumentInfo::CustomKeys))
        {
            docInfo.set(QStringLiteral("generatorDate"), dvif->generatorString,
                        i18n("Generator/Date"));
        }
        if(keys.contains(Okular::DocumentInfo::Pages))
        {
            docInfo.set(Okular::DocumentInfo::Pages,
                        QString::number(dvif->total_pages));
        }
    }
    return docInfo;
}

const Okular::DocumentSynopsis* DviGenerator::generateDocumentSynopsis()
{
    if(m_docSynopsis)
    {
        return m_docSynopsis;
    }

    m_docSynopsis = new Okular::DocumentSynopsis();

    userMutex()->lock();

    QVector<PreBookmark> prebookmarks = m_dviRenderer->getPrebookmarks();

    userMutex()->unlock();

    if(prebookmarks.isEmpty())
    {
        return m_docSynopsis;
    }

    QStack<QDomElement> stack;

    QVector<PreBookmark>::ConstIterator it = prebookmarks.constBegin();
    QVector<PreBookmark>::ConstIterator itEnd = prebookmarks.constEnd();
    for(; it != itEnd; ++it)
    {
        QDomElement domel = m_docSynopsis->createElement((*it).title);
        Anchor a = m_dviRenderer->findAnchor((*it).anchorName);
        if(a.isValid())
        {
            Okular::DocumentViewport vp;

            const Okular::Page* p =
                document()->page(static_cast<quint16>(a.page) - 1);

            fillViewportFromAnchor(vp, a, (int)p->width(), (int)p->height());
            domel.setAttribute(QStringLiteral("Viewport"), vp.toString());
        }
        if(stack.isEmpty())
        {
            m_docSynopsis->appendChild(domel);
        }
        else
        {
            stack.top().appendChild(domel);
            stack.pop();
        }
        for(int i = 0; i < (*it).noOfChildren; ++i)
        {
            stack.push(domel);
        }
    }

    return m_docSynopsis;
}

Okular::FontInfo::List DviGenerator::fontsForPage(int page)
{
    Q_UNUSED(page);

    Okular::FontInfo::List list;

    // the list of the fonts is extracted once
    if(m_fontExtracted)
    {
        return list;
    }

    if(m_dviRenderer && m_dviRenderer->dviFile &&
       m_dviRenderer->dviFile->font_pool)
    {
        const QList<TeXFontDefinition*> fonts =
            m_dviRenderer->dviFile->font_pool->fontList;
        for(const TeXFontDefinition* font : fonts)
        {
            Okular::FontInfo of;
            QString name;
            int zoom = (int)(font->enlargement * 100 + 0.5);
#ifdef HAVE_FREETYPE
            if(font->getFullFontName().isEmpty())
            {
                name = QStringLiteral("%1, %2%").arg(font->fontname).arg(zoom);
            }
            else
            {
                name = QStringLiteral("%1 (%2), %3%")
                           .arg(font->fontname, font->getFullFontName(),
                                QString::number(zoom));
            }
#else
            name = QString::fromLatin1("%1, %2%").arg(font->fontname).arg(zoom);
#endif
            of.setName(name);

            QString fontFileName;
            if(!(font->flags & TeXFontDefinition::FONT_VIRTUAL))
            {
                if(font->font != nullptr)
                {
                    fontFileName = font->font->errorMessage;
                }
                else
                {
                    fontFileName = i18n("Font file not found");
                }

                if(fontFileName.isEmpty())
                {
                    fontFileName = font->filename;
                }
            }

            of.setFile(fontFileName);

            Okular::FontInfo::FontType ft;
            switch(font->getFontType())
            {
            case TeXFontDefinition::TEX_PK:
                ft = Okular::FontInfo::TeXPK;
                break;
            case TeXFontDefinition::TEX_VIRTUAL:
                ft = Okular::FontInfo::TeXVirtual;
                break;
            case TeXFontDefinition::TEX_FONTMETRIC:
                ft = Okular::FontInfo::TeXFontMetric;
                break;
            case TeXFontDefinition::FREETYPE:
                ft = Okular::FontInfo::TeXFreeTypeHandled;
                break;
            }
            of.setType(ft);

            // DVI has not the concept of "font embedding"
            of.setEmbedType(Okular::FontInfo::NotEmbedded);
            of.setCanBeExtracted(false);

            list.append(of);
        }

        m_fontExtracted = true;
    }

    return list;
}

void DviGenerator::loadPages(QVector<Okular::Page*>& pagesVector)
{
    QSize pageRequiredSize;

    int numofpages = m_dviRenderer->dviFile->total_pages;
    pagesVector.resize(numofpages);

    m_linkGenerated.fill(false, numofpages);

    // qCDebug(OkularDviDebug) << "resolution:" << m_resolution << ",
    // dviFile->preferred?";

    /* get the suggested size */
    if(m_dviRenderer->dviFile->suggestedPageSize)
    {
        pageRequiredSize =
            m_dviRenderer->dviFile->suggestedPageSize->sizeInPixel(
                m_resolution);
    }
    else
    {
        pageSize ps;
        pageRequiredSize = ps.sizeInPixel(m_resolution);
    }

    for(int i = 0; i < numofpages; ++i)
    {
        // qCDebug(OkularDviDebug) << "getting status of page" << i << ":";

        if(pagesVector[i])
        {
            delete pagesVector[i];
        }

        Okular::Page* page =
            new Okular::Page(i, pageRequiredSize.width(),
                             pageRequiredSize.height(), Okular::Rotation0);

        pagesVector[i] = page;
    }
    qCDebug(OkularDviDebug) << "pagesVector successfully inizialized!";

    // filling the pages with the source references rects
    const QVector<DVI_SourceFileAnchor>& sourceAnchors =
        m_dviRenderer->sourceAnchors();
    QVector<QList<Okular::SourceRefObjectRect*>> refRects(numofpages);
    for(const DVI_SourceFileAnchor& sfa : sourceAnchors)
    {
        if(sfa.page < 1 || (int)sfa.page > numofpages)
        {
            continue;
        }

        Okular::NormalizedPoint p(
            -1.0,
            (double)sfa.distance_from_top.getLength_in_pixel(dpi().height()) /
                (double)pageRequiredSize.height());
        Okular::SourceReference* sourceRef =
            new Okular::SourceReference(sfa.fileName, sfa.line);
        refRects[sfa.page - 1].append(
            new Okular::SourceRefObjectRect(p, sourceRef));
    }
    for(int i = 0; i < refRects.size(); ++i)
    {
        if(!refRects.at(i).isEmpty())
        {
            pagesVector[i]->setSourceReferences(refRects.at(i));
        }
    }
}

Okular::Document::PrintError DviGenerator::print(QPrinter& printer)
{
    // Create tempfile to write to
    QTemporaryFile tf(QDir::tempPath() + QLatin1String("/okular_XXXXXX.ps"));
    if(!tf.open())
    {
        return Okular::Document::TemporaryFileOpenPrintError;
    }

    const QList<int> pageList = Okular::FilePrinter::pageList(
        printer, static_cast<quint16>(m_dviRenderer->totalPages()),
        document()->currentPage() + 1, document()->bookmarkedPageList());
    QString pages;
    QStringList printOptions;
    // List of pages to print.
    for(const int p : pageList)
    {
        pages += QStringLiteral(",%1").arg(p);
    }
    if(!pages.isEmpty())
    {
        printOptions << QStringLiteral("-pp") << pages.mid(1);
    }

    QEventLoop el;
    m_dviRenderer->setEventLoop(&el);
    m_dviRenderer->exportPS(tf.fileName(), printOptions, &printer,
                            document()->orientation());

    tf.close();

    // Error messages are handled by the generator - ugly, but it works.
    return Okular::Document::NoPrintError;
}

QVariant DviGenerator::metaData(const QString& key,
                                const QVariant& option) const
{
    if(key == QLatin1String("NamedViewport") && !option.toString().isEmpty())
    {
        const Anchor anchor = m_dviRenderer->parseReference(option.toString());
        if(anchor.isValid())
        {
            const Okular::Page* page =
                document()->page(static_cast<quint16>(anchor.page) - 1);
            Q_ASSERT_X(page, "DviGenerator::metaData()",
                       "NULL page as result of valid Anchor");
            Okular::DocumentViewport viewport;
            fillViewportFromAnchor(viewport, anchor, page);
            if(viewport.isValid())
            {
                return viewport.toString();
            }
        }
    }
    return QVariant();
}

Q_LOGGING_CATEGORY(OkularDviDebug, "org.kde.okular.generators.dvi.core",
                   QtWarningMsg)
Q_LOGGING_CATEGORY(OkularDviShellDebug, "org.kde.okular.generators.dvi.shell",
                   QtWarningMsg)

#include "generator_dvi.moc"
