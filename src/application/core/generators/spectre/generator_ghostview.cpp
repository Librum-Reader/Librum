/*
    SPDX-FileCopyrightText: 2007-2008 Albert Astals Cid <aacid@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator_ghostview.h"
#include <core/document.h>
#include <core/fileprinter.h>
#include <core/page.h>
#include <core/utils.h>
#include <math.h>
#include <KAboutData>
#include <KConfigDialog>
#include <KLocalizedString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMimeDatabase>
#include <QMimeType>
#include <QPainter>
#include <QPixmap>
#include <QPrinter>
#include <QSize>
#include <QTemporaryFile>
#include "gssettings.h"
#include "rendererthread.h"
#include "spectre_debug.h"
#include "ui_gssettingswidget.h"

OKULAR_EXPORT_PLUGIN(GSGenerator, "libokularGenerator_ghostview.json")

GSGenerator::GSGenerator(QObject* parent, const QVariantList& args) :
    Okular::Generator(parent, args),
    m_internalDocument(nullptr),
    m_request(nullptr)
{
    setFeature(PrintPostscript);
    setFeature(PrintToFile);

    GSRendererThread* renderer = GSRendererThread::getCreateRenderer();
    if(!renderer->isRunning())
    {
        renderer->start();
    }
    connect(renderer, &GSRendererThread::imageDone, this,
            &GSGenerator::slotImageGenerated, Qt::QueuedConnection);
}

GSGenerator::~GSGenerator()
{
}

bool GSGenerator::reparseConfig()
{
    bool changed = false;
    if(m_internalDocument)
    {
#define SET_HINT(hintname, hintdefvalue, hintvar)                         \
    {                                                                     \
        bool newhint = documentMetaData(hintname, hintdefvalue).toBool(); \
        if(newhint != cache_##hintvar)                                    \
        {                                                                 \
            cache_##hintvar = newhint;                                    \
            changed = true;                                               \
        }                                                                 \
    }
        SET_HINT(GraphicsAntialiasMetaData, true, AAgfx)
        SET_HINT(TextAntialiasMetaData, true, AAtext)
#undef SET_HINT
    }
    return changed;
}

void GSGenerator::addPages(KConfigDialog* dlg)
{
    Ui_GSSettingsWidget gsw;
    QWidget* w = new QWidget(dlg);
    gsw.setupUi(w);
    dlg->addPage(w, GSSettings::self(), i18n("Ghostscript"),
                 QStringLiteral("okular-gv"),
                 i18n("Ghostscript Backend Configuration"));
}

Okular::Document::PrintError GSGenerator::print(QPrinter& printer)
{
    // Create tempfile to write to
    QTemporaryFile tf(QDir::tempPath() + QLatin1String("/okular_XXXXXX.ps"));

    // Get list of pages to print
    QList<int> pageList = Okular::FilePrinter::pageList(
        printer, spectre_document_get_n_pages(m_internalDocument),
        document()->currentPage() + 1, document()->bookmarkedPageList());

    // Default to Postscript export, but if printing to PDF use that instead
    SpectreExporterFormat exportFormat = SPECTRE_EXPORTER_FORMAT_PS;
    if(printer.outputFileName().right(3) == QLatin1String("pdf"))
    {
        exportFormat = SPECTRE_EXPORTER_FORMAT_PDF;
        tf.setFileTemplate(QDir::tempPath() +
                           QLatin1String("/okular_XXXXXX.pdf"));
    }

    if(!tf.open())
    {
        return Okular::Document::TemporaryFileOpenPrintError;
    }

    SpectreExporter* exporter =
        spectre_exporter_new(m_internalDocument, exportFormat);
    SpectreStatus exportStatus =
        spectre_exporter_begin(exporter, tf.fileName().toLatin1().constData());

    int i = 0;
    while(i < pageList.count() && exportStatus == SPECTRE_STATUS_SUCCESS)
    {
        exportStatus = spectre_exporter_do_page(exporter, pageList.at(i) - 1);
        i++;
    }

    SpectreStatus endStatus = SPECTRE_STATUS_EXPORTER_ERROR;
    if(exportStatus == SPECTRE_STATUS_SUCCESS)
    {
        endStatus = spectre_exporter_end(exporter);
    }

    spectre_exporter_free(exporter);

    const QString fileName = tf.fileName();
    tf.close();

    if(exportStatus == SPECTRE_STATUS_SUCCESS &&
       endStatus == SPECTRE_STATUS_SUCCESS)
    {
        tf.setAutoRemove(false);
        return Okular::FilePrinter::printFile(
            printer, fileName, document()->orientation(),
            Okular::FilePrinter::SystemDeletesFiles,
            Okular::FilePrinter::ApplicationSelectsPages,
            document()->bookmarkedPageRange());
    }

    return Okular::Document::UnknownPrintError;
}

bool GSGenerator::loadDocument(const QString& fileName,
                               QVector<Okular::Page*>& pagesVector)
{
    cache_AAtext = documentMetaData(TextAntialiasMetaData, true).toBool();
    cache_AAgfx = documentMetaData(GraphicsAntialiasMetaData, true).toBool();

    m_internalDocument = spectre_document_new();
    spectre_document_load(m_internalDocument,
                          QFile::encodeName(fileName).constData());
    const SpectreStatus loadStatus =
        spectre_document_status(m_internalDocument);
    if(loadStatus != SPECTRE_STATUS_SUCCESS)
    {
        qCDebug(OkularSpectreDebug)
            << "ERR:" << spectre_status_to_string(loadStatus);
        spectre_document_free(m_internalDocument);
        m_internalDocument = nullptr;
        return false;
    }
    pagesVector.resize(spectre_document_get_n_pages(m_internalDocument));
    qCDebug(OkularSpectreDebug) << "Page count:" << pagesVector.count();
    return loadPages(pagesVector);
}

bool GSGenerator::doCloseDocument()
{
    spectre_document_free(m_internalDocument);
    m_internalDocument = nullptr;

    return true;
}

void GSGenerator::slotImageGenerated(QImage* img,
                                     Okular::PixmapRequest* request)
{
    // This can happen as GSInterpreterCMD is a singleton and on creation
    // signals all the slots of all the generators attached to it
    if(request != m_request)
    {
        return;
    }

    if(!request->page()->isBoundingBoxKnown())
    {
        updatePageBoundingBox(request->page()->number(),
                              Okular::Utils::imageBoundingBox(img));
    }

    m_request = nullptr;
    QPixmap* pix = new QPixmap(QPixmap::fromImage(*img));
    delete img;
    request->page()->setPixmap(request->observer(), pix);
    signalPixmapRequestDone(request);
}

bool GSGenerator::loadPages(QVector<Okular::Page*>& pagesVector)
{
    for(uint i = 0; i < spectre_document_get_n_pages(m_internalDocument); i++)
    {
        SpectrePage* page;
        int width = 0, height = 0;
        SpectreOrientation pageOrientation = SPECTRE_ORIENTATION_PORTRAIT;
        page = spectre_document_get_page(m_internalDocument, i);
        if(spectre_document_status(m_internalDocument))
        {
            qCDebug(OkularSpectreDebug)
                << "Error getting page" << i
                << spectre_status_to_string(
                       spectre_document_status(m_internalDocument));
        }
        else
        {
            spectre_page_get_size(page, &width, &height);
            pageOrientation = spectre_page_get_orientation(page);
        }
        spectre_page_free(page);
        if(pageOrientation % 2 == 1)
        {
            qSwap(width, height);
        }
        pagesVector[i] =
            new Okular::Page(i, width, height, orientation(pageOrientation));
    }
    return pagesVector.count() > 0;
}

void GSGenerator::generatePixmap(Okular::PixmapRequest* req)
{
    qCDebug(OkularSpectreDebug) << "receiving" << *req;

    SpectrePage* page =
        spectre_document_get_page(m_internalDocument, req->pageNumber());

    GSRendererThread* renderer = GSRendererThread::getCreateRenderer();

    GSRendererThreadRequest gsreq(this);
    gsreq.spectrePage = page;
    gsreq.platformFonts = GSSettings::platformFonts();
    int graphicsAA = 1;
    int textAA = 1;
    if(cache_AAgfx)
    {
        graphicsAA = 4;
    }
    if(cache_AAtext)
    {
        textAA = 4;
    }
    gsreq.textAAbits = textAA;
    gsreq.graphicsAAbits = graphicsAA;

    gsreq.orientation = req->page()->orientation();
    if(req->page()->rotation() == Okular::Rotation90 ||
       req->page()->rotation() == Okular::Rotation270)
    {
        gsreq.magnify = qMax((double)req->height() / req->page()->width(),
                             (double)req->width() / req->page()->height());
    }
    else
    {
        gsreq.magnify = qMax((double)req->width() / req->page()->width(),
                             (double)req->height() / req->page()->height());
    }
    gsreq.request = req;
    m_request = req;
    renderer->addRequest(gsreq);
}

bool GSGenerator::canGeneratePixmap() const
{
    return !m_request;
}

Okular::DocumentInfo GSGenerator::generateDocumentInfo(
    const QSet<Okular::DocumentInfo::Key>& keys) const
{
    Okular::DocumentInfo docInfo;
    if(keys.contains(Okular::DocumentInfo::Title))
    {
        docInfo.set(
            Okular::DocumentInfo::Title,
            QString::fromUtf8(spectre_document_get_title(m_internalDocument)));
    }
    if(keys.contains(Okular::DocumentInfo::Author))
    {
        docInfo.set(
            Okular::DocumentInfo::Author,
            QString::fromUtf8(spectre_document_get_for(m_internalDocument)));
    }
    if(keys.contains(Okular::DocumentInfo::Creator))
    {
        docInfo.set(Okular::DocumentInfo::Creator,
                    QString::fromUtf8(
                        spectre_document_get_creator(m_internalDocument)));
    }
    if(keys.contains(Okular::DocumentInfo::CreationDate))
    {
        docInfo.set(Okular::DocumentInfo::CreationDate,
                    QString::fromUtf8(spectre_document_get_creation_date(
                        m_internalDocument)));
    }
    if(keys.contains(Okular::DocumentInfo::CustomKeys))
    {
        docInfo.set(
            QStringLiteral("dscversion"),
            QString::fromUtf8(spectre_document_get_format(m_internalDocument)),
            i18n("Document version"));
    }

    if(keys.contains(Okular::DocumentInfo::MimeType))
    {
        int languageLevel =
            spectre_document_get_language_level(m_internalDocument);
        if(languageLevel > 0)
        {
            docInfo.set(QStringLiteral("langlevel"),
                        QString::number(languageLevel), i18n("Language Level"));
        }
        if(spectre_document_is_eps(m_internalDocument))
        {
            docInfo.set(Okular::DocumentInfo::MimeType,
                        QStringLiteral("image/x-eps"));
        }
        else
        {
            docInfo.set(Okular::DocumentInfo::MimeType,
                        QStringLiteral("application/postscript"));
        }
    }

    if(keys.contains(Okular::DocumentInfo::Pages))
    {
        docInfo.set(
            Okular::DocumentInfo::Pages,
            QString::number(spectre_document_get_n_pages(m_internalDocument)));
    }

    return docInfo;
}

Okular::Rotation GSGenerator::orientation(
    SpectreOrientation pageOrientation) const
{
    switch(pageOrientation)
    {
    case SPECTRE_ORIENTATION_PORTRAIT:
        return Okular::Rotation0;
    case SPECTRE_ORIENTATION_LANDSCAPE:
        return Okular::Rotation90;
    case SPECTRE_ORIENTATION_REVERSE_PORTRAIT:
        return Okular::Rotation180;
    case SPECTRE_ORIENTATION_REVERSE_LANDSCAPE:
        return Okular::Rotation270;
    }
    // get rid of warnings, should never happen
    return Okular::Rotation0;
}

QVariant GSGenerator::metaData(const QString& key, const QVariant& option) const
{
    Q_UNUSED(option)
    if(key == QLatin1String("DocumentTitle"))
    {
        const char* title = spectre_document_get_title(m_internalDocument);
        if(title)
        {
            return QString::fromLatin1(title);
        }
    }
    return QVariant();
}

#include "generator_ghostview.moc"
