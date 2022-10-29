/*
    SPDX-FileCopyrightText: 2005 Piotr Szymanski <niedakh@gmail.com>
    SPDX-FileCopyrightText: 2008 Albert Astals Cid <aacid@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "generator.h"
#include <KLocalizedString>
#include <QApplication>
#include <QDebug>
#include <QEventLoop>
#include <QIcon>
#include <QMimeDatabase>
#include <QPrinter>
#include <QTimer>
#include "generator_p.h"
#include "observer.h"

#ifdef WITH_KWALLET
    #include <KWallet>
#endif

#include "document_p.h"
#include "page.h"
#include "page_p.h"
#include "textpage.h"
#include "utils.h"

using namespace Okular;

GeneratorPrivate::GeneratorPrivate() :
    m_document(nullptr),
    mPixmapGenerationThread(nullptr),
    mTextPageGenerationThread(nullptr),
    mPixmapReady(true),
    mTextPageReady(true),
    m_closing(false),
    m_closingLoop(nullptr),
    m_dpi(72.0, 72.0)
{
    qRegisterMetaType<Okular::Page*>();
}

GeneratorPrivate::~GeneratorPrivate()
{
    if(mPixmapGenerationThread)
    {
        mPixmapGenerationThread->wait();
    }

    delete mPixmapGenerationThread;

    if(mTextPageGenerationThread)
    {
        mTextPageGenerationThread->wait();
    }

    delete mTextPageGenerationThread;
}

PixmapGenerationThread* GeneratorPrivate::pixmapGenerationThread()
{
    if(mPixmapGenerationThread)
    {
        return mPixmapGenerationThread;
    }

    Q_Q(Generator);
    mPixmapGenerationThread = new PixmapGenerationThread(q);
    QObject::connect(
        mPixmapGenerationThread, &PixmapGenerationThread::finished, q,
        [this]
        {
            pixmapGenerationFinished();
        },
        Qt::QueuedConnection);

    return mPixmapGenerationThread;
}

TextPageGenerationThread* GeneratorPrivate::textPageGenerationThread()
{
    if(mTextPageGenerationThread)
    {
        return mTextPageGenerationThread;
    }

    Q_Q(Generator);
    mTextPageGenerationThread = new TextPageGenerationThread(q);
    QObject::connect(
        mTextPageGenerationThread, &TextPageGenerationThread::finished, q,
        [this]
        {
            textpageGenerationFinished();
        },
        Qt::QueuedConnection);

    return mTextPageGenerationThread;
}

void GeneratorPrivate::pixmapGenerationFinished()
{
    Q_Q(Generator);
    PixmapRequest* request = mPixmapGenerationThread->request();
    const QImage& img = mPixmapGenerationThread->image();
    mPixmapGenerationThread->endGeneration();

    QMutexLocker locker(threadsLock());

    if(m_closing)
    {
        mPixmapReady = true;
        delete request;
        if(mTextPageReady)
        {
            locker.unlock();
            m_closingLoop->quit();
        }
        return;
    }

    if(!request->shouldAbortRender())
    {
        request->page()->setPixmap(request->observer(),
                                   new QPixmap(QPixmap::fromImage(img)),
                                   request->normalizedRect());
        const int pageNumber = request->page()->number();

        if(mPixmapGenerationThread->calcBoundingBox())
        {
            q->updatePageBoundingBox(pageNumber,
                                     mPixmapGenerationThread->boundingBox());
        }
    }
    else
    {
        // Cancel the text page generation too if it's still running
        if(mTextPageGenerationThread && mTextPageGenerationThread->isRunning())
        {
            mTextPageGenerationThread->abortExtraction();
            mTextPageGenerationThread->wait();
        }
    }

    mPixmapReady = true;
    q->signalPixmapRequestDone(request);
}

void GeneratorPrivate::textpageGenerationFinished()
{
    Q_Q(Generator);
    Page* page = mTextPageGenerationThread->page();
    mTextPageGenerationThread->endGeneration();

    QMutexLocker locker(threadsLock());
    mTextPageReady = true;

    if(m_closing)
    {
        delete mTextPageGenerationThread->textPage();
        if(mPixmapReady)
        {
            locker.unlock();
            m_closingLoop->quit();
        }
        return;
    }

    if(mTextPageGenerationThread->textPage())
    {
        TextPage* tp = mTextPageGenerationThread->textPage();
        page->setTextPage(tp);
        q->signalTextGenerationDone(page, tp);
    }
}

QMutex* GeneratorPrivate::threadsLock()
{
    return &m_threadsMutex;
}

QVariant GeneratorPrivate::metaData(const QString&, const QVariant&) const
{
    return QVariant();
}

QImage GeneratorPrivate::image(PixmapRequest*)
{
    return QImage();
}

Generator::Generator(QObject* parent, const QVariantList& args) :
    Generator(*new GeneratorPrivate(), parent, args)
{
    // the delegated constructor does it all
}

Generator::Generator(GeneratorPrivate& dd, QObject* parent,
                     const QVariantList& args) :
    QObject(parent),
    d_ptr(&dd)
{
    d_ptr->q_ptr = this;
    Q_UNUSED(args)
}

Generator::~Generator()
{
    delete d_ptr;
}

bool Generator::loadDocument(const QString& fileName,
                             QVector<Page*>& pagesVector)
{
    Q_UNUSED(fileName);
    Q_UNUSED(pagesVector);

    return false;
}

bool Generator::loadDocumentFromData(const QByteArray&, QVector<Page*>&)
{
    return false;
}

Document::OpenResult Generator::loadDocumentWithPassword(
    const QString& fileName, QVector<Page*>& pagesVector, const QString&)
{
    return loadDocument(fileName, pagesVector) ? Document::OpenSuccess
                                               : Document::OpenError;
}

Document::OpenResult Generator::loadDocumentFromDataWithPassword(
    const QByteArray& fileData, QVector<Page*>& pagesVector, const QString&)
{
    return loadDocumentFromData(fileData, pagesVector) ? Document::OpenSuccess
                                                       : Document::OpenError;
}

Generator::SwapBackingFileResult Generator::swapBackingFile(
    QString const& /*newFileName */, QVector<Okular::Page*>& /*newPagesVector*/)
{
    return SwapBackingFileError;
}

bool Generator::closeDocument()
{
    Q_D(Generator);

    d->m_closing = true;

    d->threadsLock()->lock();
    if(!(d->mPixmapReady && d->mTextPageReady))
    {
        QEventLoop loop;
        d->m_closingLoop = &loop;

        d->threadsLock()->unlock();

        loop.exec();

        d->m_closingLoop = nullptr;
    }
    else
    {
        d->threadsLock()->unlock();
    }

    bool ret = doCloseDocument();

    d->m_closing = false;

    return ret;
}

bool Generator::canGeneratePixmap() const
{
    Q_D(const Generator);
    return d->mPixmapReady;
}

bool Generator::canSign() const
{
    return false;
}

bool Generator::sign(const NewSignatureData&, const QString&)
{
    return false;
}

CertificateStore* Generator::certificateStore() const
{
    return nullptr;
}

void Generator::generatePixmap(PixmapRequest* request)
{
    Q_D(Generator);
    d->mPixmapReady = false;

    const bool calcBoundingBox =
        !request->isTile() && !request->page()->isBoundingBoxKnown();

    if(request->asynchronous() && hasFeature(Threaded))
    {
        if(d->textPageGenerationThread()->isFinished() &&
           !canGenerateTextPage())
        {
            // It can happen that the text generation has already finished but
            // mTextPageReady is still false because textpageGenerationFinished
            // didn't have time to run, if so queue ourselves
            QTimer::singleShot(0, this,
                               [this, request]
                               {
                                   generatePixmap(request);
                               });
            return;
        }

        /**
         * We create the text page for every page that is visible to the
         * user, so he can use the text extraction tools without a delay.
         */
        if(hasFeature(TextExtraction) && !request->page()->hasTextPage() &&
           canGenerateTextPage() && !d->m_closing)
        {
            d->mTextPageReady = false;
            d->textPageGenerationThread()->setPage(request->page());

            // dummy is used as a way to make sure the lambda gets disconnected
            // each time it is executed since not all the times the pixmap
            // generation thread starts we want the text generation thread to
            // also start
            QObject* dummy = new QObject();
            connect(d_ptr->pixmapGenerationThread(), &QThread::started, dummy,
                    [this, dummy]
                    {
                        delete dummy;
                        d_ptr->textPageGenerationThread()->startGeneration();
                    });
        }
        // pixmap generation thread must be started *after* connect(), else we
        // may miss the start signal and get lock-ups (see bug 396137)
        d->pixmapGenerationThread()->startGeneration(request, calcBoundingBox);

        return;
    }

    const QImage& img = image(request);
    request->page()->setPixmap(request->observer(),
                               new QPixmap(QPixmap::fromImage(img)),
                               request->normalizedRect());
    const int pageNumber = request->page()->number();

    d->mPixmapReady = true;

    signalPixmapRequestDone(request);
    if(calcBoundingBox)
    {
        updatePageBoundingBox(pageNumber, Utils::imageBoundingBox(&img));
    }
}

bool Generator::canGenerateTextPage() const
{
    Q_D(const Generator);
    return d->mTextPageReady;
}

void Generator::generateTextPage(Page* page)
{
    TextRequest treq(page);
    TextPage* tp = textPage(&treq);
    page->setTextPage(tp);
    signalTextGenerationDone(page, tp);
}

QImage Generator::image(PixmapRequest* request)
{
    Q_D(Generator);
    return d->image(request);
}

TextPage* Generator::textPage(TextRequest*)
{
    return nullptr;
}

DocumentInfo Generator::generateDocumentInfo(
    const QSet<DocumentInfo::Key>& keys) const
{
    Q_UNUSED(keys);

    return DocumentInfo();
}

const DocumentSynopsis* Generator::generateDocumentSynopsis()
{
    return nullptr;
}

FontInfo::List Generator::fontsForPage(int)
{
    return FontInfo::List();
}

const QList<EmbeddedFile*>* Generator::embeddedFiles() const
{
    return nullptr;
}

Generator::PageSizeMetric Generator::pagesSizeMetric() const
{
    return None;
}

bool Generator::isAllowed(Permission) const
{
    return true;
}

void Generator::rotationChanged(Rotation, Rotation)
{
}

PageSize::List Generator::pageSizes() const
{
    return PageSize::List();
}

void Generator::pageSizeChanged(const PageSize&, const PageSize&)
{
}

Document::PrintError Generator::print(QPrinter&)
{
    return Document::UnknownPrintError;
}

void Generator::opaqueAction(const BackendOpaqueAction* /*action*/)
{
}

void Generator::freeOpaqueActionContents(const BackendOpaqueAction& /*action*/)
{
}

QVariant Generator::metaData(const QString& key, const QVariant& option) const
{
    Q_D(const Generator);
    return d->metaData(key, option);
}

ExportFormat::List Generator::exportFormats() const
{
    return ExportFormat::List();
}

bool Generator::exportTo(const QString&, const ExportFormat&)
{
    return false;
}

void Generator::walletDataForFile(const QString& fileName, QString* walletName,
                                  QString* walletFolder,
                                  QString* walletKey) const
{
#ifdef WITH_KWALLET
    *walletKey = fileName.section(QLatin1Char('/'), -1, -1);
    *walletName = KWallet::Wallet::NetworkWallet();
    *walletFolder = QStringLiteral("KPdf");
#endif
}

bool Generator::hasFeature(GeneratorFeature feature) const
{
    Q_D(const Generator);
    return d->m_features.contains(feature);
}

void Generator::signalPixmapRequestDone(PixmapRequest* request)
{
    Q_D(Generator);
    if(d->m_document)
    {
        d->m_document->requestDone(request);
    }
    else
    {
        delete request;
    }
}

void Generator::signalTextGenerationDone(Page* page, TextPage* textPage)
{
    Q_D(Generator);
    if(d->m_document)
    {
        d->m_document->textGenerationDone(page);
    }
    else
    {
        delete textPage;
    }
}

void Generator::signalPartialPixmapRequest(PixmapRequest* request,
                                           const QImage& image)
{
    if(request->shouldAbortRender())
    {
        return;
    }

    PagePrivate* pagePrivate = PagePrivate::get(request->page());
    pagePrivate->setPixmap(
        request->observer(), new QPixmap(QPixmap::fromImage(image)),
        request->normalizedRect(), true /* isPartialPixmap */);

    const int pageNumber = request->page()->number();
    request->observer()->notifyPageChanged(pageNumber,
                                           Okular::DocumentObserver::Pixmap);
}

const Document* Generator::document() const
{
    Q_D(const Generator);
    if(d->m_document)
    {
        return d->m_document->m_parent;
    }
    return nullptr;
}

void Generator::setFeature(GeneratorFeature feature, bool on)
{
    Q_D(Generator);
    if(on)
    {
        d->m_features.insert(feature);
    }
    else
    {
        d->m_features.remove(feature);
    }
}

QVariant Generator::documentMetaData(const DocumentMetaDataKey key,
                                     const QVariant& option) const
{
    Q_D(const Generator);
    if(!d->m_document)
    {
        return QVariant();
    }

    return d->m_document->documentMetaData(key, option);
}

QMutex* Generator::userMutex() const
{
    Q_D(const Generator);
    return &d->m_mutex;
}

void Generator::updatePageBoundingBox(int page,
                                      const NormalizedRect& boundingBox)
{
    Q_D(Generator);
    if(d->m_document)
    {  // still connected to document?
        d->m_document->setPageBoundingBox(page, boundingBox);
    }
}

QByteArray Generator::requestFontData(const Okular::FontInfo& /*font*/)
{
    return {};
}

void Generator::setDPI(const QSizeF dpi)
{
    Q_D(Generator);
    d->m_dpi = dpi;
}

QSizeF Generator::dpi() const
{
    Q_D(const Generator);
    return d->m_dpi;
}

QAbstractItemModel* Generator::layersModel() const
{
    return nullptr;
}

TextRequest::TextRequest() :
    d(new TextRequestPrivate)
{
    d->mPage = nullptr;
    d->mShouldAbortExtraction = 0;
}

TextRequest::TextRequest(Page* page) :
    d(new TextRequestPrivate)
{
    d->mPage = page;
    d->mShouldAbortExtraction = 0;
}

TextRequest::~TextRequest()
{
    delete d;
}

Page* TextRequest::page() const
{
    return d->mPage;
}

bool TextRequest::shouldAbortExtraction() const
{
    return d->mShouldAbortExtraction != 0;
}

TextRequestPrivate* TextRequestPrivate::get(const TextRequest* req)
{
    return req->d;
}

PixmapRequest::PixmapRequest(DocumentObserver* observer, int pageNumber,
                             int width, int height, int priority,
                             PixmapRequestFeatures features) :
    PixmapRequest(observer, pageNumber, width, height, qApp->devicePixelRatio(),
                  priority, features)
{
}

PixmapRequest::PixmapRequest(DocumentObserver* observer, int pageNumber,
                             int width, int height, qreal dpr, int priority,
                             PixmapRequestFeatures features) :
    d(new PixmapRequestPrivate)
{
    d->mObserver = observer;
    d->mPageNumber = pageNumber;
    d->mWidth = ceil(width * dpr);
    d->mHeight = ceil(height * dpr);
    d->mPriority = priority;
    d->mFeatures = features;
    d->mForce = false;
    d->mTile = false;
    d->mNormalizedRect = NormalizedRect();
    d->mPartialUpdatesWanted = false;
    d->mShouldAbortRender = 0;
}

PixmapRequest::~PixmapRequest()
{
    delete d;
}

DocumentObserver* PixmapRequest::observer() const
{
    return d->mObserver;
}

int PixmapRequest::pageNumber() const
{
    return d->mPageNumber;
}

int PixmapRequest::width() const
{
    return d->mWidth;
}

int PixmapRequest::height() const
{
    return d->mHeight;
}

int PixmapRequest::priority() const
{
    return d->mPriority;
}

bool PixmapRequest::asynchronous() const
{
    return d->mFeatures & Asynchronous;
}

bool PixmapRequest::preload() const
{
    return d->mFeatures & Preload;
}

Page* PixmapRequest::page() const
{
    return d->mPage;
}

void PixmapRequest::setTile(bool tile)
{
    d->mTile = tile;
}

bool PixmapRequest::isTile() const
{
    return d->mTile;
}

void PixmapRequest::setNormalizedRect(const NormalizedRect& rect)
{
    if(d->mNormalizedRect == rect)
    {
        return;
    }

    d->mNormalizedRect = rect;
}

const NormalizedRect& PixmapRequest::normalizedRect() const
{
    return d->mNormalizedRect;
}

void PixmapRequest::setPartialUpdatesWanted(bool partialUpdatesWanted)
{
    d->mPartialUpdatesWanted = partialUpdatesWanted;
}

bool PixmapRequest::partialUpdatesWanted() const
{
    return d->mPartialUpdatesWanted;
}

bool PixmapRequest::shouldAbortRender() const
{
    return d->mShouldAbortRender != 0;
}

Okular::TilesManager* PixmapRequestPrivate::tilesManager() const
{
    return mPage->d->tilesManager(mObserver);
}

PixmapRequestPrivate* PixmapRequestPrivate::get(const PixmapRequest* req)
{
    return req->d;
}

void PixmapRequestPrivate::swap()
{
    qSwap(mWidth, mHeight);
}

class Okular::ExportFormatPrivate : public QSharedData
{
public:
    ExportFormatPrivate(const QString& description, const QMimeType& mimeType,
                        const QIcon& icon = QIcon()) :
        QSharedData(),
        mDescription(description),
        mMimeType(mimeType),
        mIcon(icon)
    {
    }

    ~ExportFormatPrivate()
    {
    }

    QString mDescription;
    QMimeType mMimeType;
    QIcon mIcon;
};

ExportFormat::ExportFormat() :
    d(new ExportFormatPrivate(QString(), QMimeType()))
{
}

ExportFormat::ExportFormat(const QString& description,
                           const QMimeType& mimeType) :
    d(new ExportFormatPrivate(description, mimeType))
{
}

ExportFormat::ExportFormat(const QIcon& icon, const QString& description,
                           const QMimeType& mimeType) :
    d(new ExportFormatPrivate(description, mimeType, icon))
{
}

ExportFormat::~ExportFormat()
{
}

ExportFormat::ExportFormat(const ExportFormat& other) :
    d(other.d)
{
}

ExportFormat& ExportFormat::operator=(const ExportFormat& other)
{
    if(this == &other)
    {
        return *this;
    }

    d = other.d;

    return *this;
}

QString ExportFormat::description() const
{
    return d->mDescription;
}

QMimeType ExportFormat::mimeType() const
{
    return d->mMimeType;
}

QIcon ExportFormat::icon() const
{
    return d->mIcon;
}

bool ExportFormat::isNull() const
{
    return !d->mMimeType.isValid() || d->mDescription.isNull();
}

ExportFormat ExportFormat::standardFormat(StandardExportFormat type)
{
    QMimeDatabase db;
    switch(type)
    {
    case PlainText:
        return ExportFormat(QIcon::fromTheme(QStringLiteral("text-x-generic")),
                            i18n("Plain &Text..."),
                            db.mimeTypeForName(QStringLiteral("text/plain")));
        break;
    case PDF:
        return ExportFormat(
            QIcon::fromTheme(QStringLiteral("application-pdf")), i18n("PDF"),
            db.mimeTypeForName(QStringLiteral("application/pdf")));
        break;
    case OpenDocumentText:
        return ExportFormat(
            QIcon::fromTheme(
                QStringLiteral("application-vnd.oasis.opendocument.text")),
            i18nc("This is the document format", "OpenDocument Text"),
            db.mimeTypeForName(
                QStringLiteral("application/vnd.oasis.opendocument.text")));
        break;
    case HTML:
        return ExportFormat(QIcon::fromTheme(QStringLiteral("text-html")),
                            i18nc("This is the document format", "HTML"),
                            db.mimeTypeForName(QStringLiteral("text/html")));
        break;
    }
    return ExportFormat();
}

bool ExportFormat::operator==(const ExportFormat& other) const
{
    return d == other.d;
}

bool ExportFormat::operator!=(const ExportFormat& other) const
{
    return d != other.d;
}

QDebug operator<<(QDebug str, const Okular::PixmapRequest& req)
{
    PixmapRequestPrivate* reqPriv = PixmapRequestPrivate::get(&req);

    str << "PixmapRequest:" << &req;
    str << "- observer:" << (qulonglong)req.observer();
    str << "- page:" << req.pageNumber();
    str << "- width:" << req.width();
    str << "- height:" << req.height();
    str << "- priority:" << req.priority();
    str << "- async:" << (req.asynchronous() ? "true" : "false");
    str << "- tile:" << (req.isTile() ? "true" : "false");
    str << "- rect:" << req.normalizedRect();
    str << "- preload:" << (req.preload() ? "true" : "false");
    str << "- partialUpdates:"
        << (req.partialUpdatesWanted() ? "true" : "false");
    str << "- shouldAbort:" << (req.shouldAbortRender() ? "true" : "false");
    str << "- force:" << (reqPriv->mForce ? "true" : "false");
    return str;
}

/* kate: replace-tabs on; indent-width 4; */
