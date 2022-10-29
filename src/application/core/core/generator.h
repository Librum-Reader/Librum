/*
    SPDX-FileCopyrightText: 2004-5 Enrico Ros <eros.kde@email.it>
    SPDX-FileCopyrightText: 2005 Piotr Szymanski <niedakh@gmail.com>
    SPDX-FileCopyrightText: 2008 Albert Astals Cid <aacid@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_GENERATOR_H_
    #define _OKULAR_GENERATOR_H_

    #include <KPluginFactory>
    #include <QList>
    #include <QMimeType>
    #include <QObject>
    #include <QSharedDataPointer>
    #include <QSizeF>
    #include <QString>
    #include <QVariant>
    #include <QVector>
    #include "document.h"
    #include "fontinfo.h"
    #include "global.h"
    #include "okularcore_export.h"
    #include "pagesize.h"
    #include "signatureutils.h"

    #define OKULAR_EXPORT_PLUGIN(classname, json)                        \
        static_assert(json[0] != '\0', "arg2 must be a string literal"); \
        K_PLUGIN_CLASS_WITH_JSON(classname, json)

class QByteArray;
class QMutex;
class QPrinter;
class QIcon;

namespace Okular
{
class BackendOpaqueAction;
class DocumentFonts;
class DocumentInfo;
class DocumentObserver;
class DocumentSynopsis;
class EmbeddedFile;
class ExportFormatPrivate;
class FontInfo;
class GeneratorPrivate;
class Page;
class PixmapRequest;
class PixmapRequestPrivate;
class TextPage;
class TextRequest;
class TextRequestPrivate;
class NormalizedRect;

/* Note: on contents generation and asynchronous queries.
 * Many observers may want to request data synchronously or asynchronously.
 * - Sync requests. These should be done in-place.
 * - Async request must be done in real background. That usually means a
 *   thread, such as QThread derived classes.
 * Once contents are available, they must be immediately stored in the
 * Page they refer to, and a signal is emitted as soon as storing
 * (even for sync or async queries) has been done.
 */

/**
 * @short Defines an entry for the export menu
 *
 * This class encapsulates information about an export format.
 * Every Generator can support 0 or more export formats which can be
 * queried with @ref Generator::exportFormats().
 */
class OKULARCORE_EXPORT ExportFormat
{
public:
    typedef QList<ExportFormat> List;

    /**
     * Creates an empty export format.
     *
     * @see isNull()
     */
    ExportFormat();

    /**
     * Creates a new export format.
     *
     * @param description The i18n'ed description of the format.
     * @param mimeType The supported mime type of the format.
     */
    ExportFormat(const QString& description, const QMimeType& mimeType);

    /**
     * Creates a new export format.
     *
     * @param icon The icon used in the GUI for this format.
     * @param description The i18n'ed description of the format.
     * @param mimeType The supported mime type of the format.
     */
    ExportFormat(const QIcon& icon, const QString& description,
                 const QMimeType& mimeType);

    /**
     * Destroys the export format.
     */
    ~ExportFormat();

    /**
     * @internal
     */
    ExportFormat(const ExportFormat& other);

    /**
     * @internal
     */
    ExportFormat& operator=(const ExportFormat& other);

    /**
     * Returns the description of the format.
     */
    QString description() const;

    /**
     * Returns the mime type of the format.
     */
    QMimeType mimeType() const;

    /**
     * Returns the icon for GUI representations of the format.
     */
    QIcon icon() const;

    /**
     * Returns whether the export format is null/valid.
     *
     * An ExportFormat is null if the mimetype is not valid or the
     * description is empty, or both.
     */
    bool isNull() const;

    /**
     * Type of standard export format.
     */
    enum StandardExportFormat
    {
        PlainText,  ///< Plain text
        PDF,  ///< PDF, aka Portable Document Format
        OpenDocumentText,  ///< OpenDocument Text format @since 0.8 (KDE 4.2)
        HTML  ///< OpenDocument Text format @since 0.8 (KDE 4.2)
    };

    /**
     * Builds a standard format for the specified @p type .
     */
    static ExportFormat standardFormat(StandardExportFormat type);

    bool operator==(const ExportFormat& other) const;

    bool operator!=(const ExportFormat& other) const;

private:
    /// @cond PRIVATE
    friend class ExportFormatPrivate;
    /// @endcond
    QSharedDataPointer<ExportFormatPrivate> d;
};

/**
 * @short [Abstract Class] The information generator.
 *
 * Most of class members are virtuals and some of them pure virtual. The pure
 * virtuals provide the minimal functionalities for a Generator, that is being
 * able to generate QPixmap for the Page 's of the Document.
 *
 * Implementing the other functions will make the Generator able to provide
 * more contents and/or functionalities (like text extraction).
 *
 * Generation/query is requested by the Document class only, and that
 * class stores the resulting data into Page s. The data will then be
 * displayed by the GUI components (PageView, ThumbnailList, etc..).
 *
 * @see PrintInterface, ConfigInterface, GuiInterface
 */
class OKULARCORE_EXPORT Generator : public QObject
{
    /// @cond PRIVATE
    friend class PixmapGenerationThread;
    friend class TextPageGenerationThread;
    /// @endcond

    Q_OBJECT

public:
    /**
     * Describe the possible optional features that a Generator can
     * provide.
     */
    enum GeneratorFeature
    {
        Threaded,  ///< Whether the Generator supports asynchronous generation
                   ///< of pictures or text pages
        TextExtraction,  ///< Whether the Generator can extract text from the
                         ///< document in the form of TextPage's
        ReadRawData,  ///< Whether the Generator can read a document directly
                      ///< from its raw data.
        FontInfo,  ///< Whether the Generator can provide information about the
                   ///< fonts used in the document
        PageSizes,  ///< Whether the Generator can change the size of the
                    ///< document pages.
        PrintNative,  ///< Whether the Generator supports native cross-platform
                      ///< printing (QPainter-based).
        PrintPostscript,  ///< Whether the Generator supports postscript-based
                          ///< file printing.
        PrintToFile,  ///< Whether the Generator supports export to PDF & PS
                      ///< through the Print Dialog
        TiledRendering,  ///< Whether the Generator can render tiles @since 0.16
                         ///< (KDE 4.10)
        SwapBackingFile,  ///< Whether the Generator can hot-swap the file it's
                          ///< reading from @since 1.3
        SupportsCancelling  ///< Whether the Generator can cancel requests
                            ///< @since 1.4
    };

    /**
     * Creates a new generator.
     */
    explicit Generator(QObject* parent = nullptr,
                       const QVariantList& args = QVariantList());

    /**
     * Destroys the generator.
     */
    ~Generator() override;

    /**
     * Loads the document with the given @p fileName and fills the
     * @p pagesVector with the parsed pages.
     *
     * @note If you implement the WithPassword variants you don't need to
     * implement this one
     *
     * @returns true on success, false otherwise.
     */
    virtual bool loadDocument(const QString& fileName,
                              QVector<Page*>& pagesVector);

    /**
     * Loads the document from the raw data @p fileData and fills the
     * @p pagesVector with the parsed pages.
     *
     * @note If you implement the WithPassword variants you don't need to
     * implement this one
     *
     * @note the Generator has to have the feature @ref ReadRawData enabled
     *
     * @returns true on success, false otherwise.
     */
    virtual bool loadDocumentFromData(const QByteArray& fileData,
                                      QVector<Page*>& pagesVector);

    /**
     * Loads the document with the given @p fileName and @p password and fills
     * the
     * @p pagesVector with the parsed pages.
     *
     * @note Do not implement this if your format doesn't support passwords,
     * it'll cleanly call loadDocument()
     *
     * @since 0.20 (KDE 4.14)
     *
     * @returns a LoadResult defining the result of the operation
     */
    virtual Document::OpenResult loadDocumentWithPassword(
        const QString& fileName, QVector<Page*>& pagesVector,
        const QString& password);

    /**
     * Loads the document from the raw data @p fileData and @p password and
     * fills the
     * @p pagesVector with the parsed pages.
     *
     * @note Do not implement this if your format doesn't support passwords,
     * it'll cleanly call loadDocumentFromData()
     *
     * @note the Generator has to have the feature @ref ReadRawData enabled
     *
     * @since 0.20 (KDE 4.14)
     *
     * @returns a LoadResult defining the result of the operation
     */
    virtual Document::OpenResult loadDocumentFromDataWithPassword(
        const QByteArray& fileData, QVector<Page*>& pagesVector,
        const QString& password);

    /**
     * Describes the result of an swap file operation.
     *
     * @since 1.3
     */
    enum SwapBackingFileResult
    {
        SwapBackingFileError,  //< The document could not be swapped
        SwapBackingFileNoOp,  //< The document was swapped and nothing needs to
                              // be done
        SwapBackingFileReloadInternalData  //< The document was swapped and
                                           // internal data (forms, annotations,
                                           // etc) needs to be reloaded
    };

    /**
     * Changes the path of the file we are reading from. The new path must
     * point to a copy of the same document.
     *
     * @note the Generator has to have the feature @ref SwapBackingFile enabled
     *
     * @since 1.3
     */
    virtual SwapBackingFileResult swapBackingFile(
        const QString& newFileName, QVector<Okular::Page*>& newPagesVector);

    /**
     * This method is called when the document is closed and not used
     * any longer.
     *
     * @returns true on success, false otherwise.
     */
    bool closeDocument();

    /**
     * This method returns whether the generator is ready to
     * handle a new pixmap request.
     */
    virtual bool canGeneratePixmap() const;

    virtual bool canSign() const;

    virtual bool sign(const NewSignatureData& data, const QString& rFilename);

    virtual CertificateStore* certificateStore() const;

    /**
     * This method can be called to trigger the generation of
     * a new pixmap as described by @p request.
     */
    virtual void generatePixmap(PixmapRequest* request);

    /**
     * This method returns whether the generator is ready to
     * handle a new text page request.
     */
    virtual bool canGenerateTextPage() const;

    /**
     * This method can be called to trigger the generation of
     * a text page for the given @p page.
     *
     * The generation is done in the calling thread.
     *
     * @see TextPage
     */
    void generateTextPage(Page* page);

    /**
     * Returns the general information object of the document.
     *
     * Changed signature in okular version 0.21
     */
    virtual DocumentInfo generateDocumentInfo(
        const QSet<DocumentInfo::Key>& keys) const;

    /**
     * Returns the 'table of content' object of the document or 0 if
     * no table of content is available.
     */
    virtual const DocumentSynopsis* generateDocumentSynopsis();

    /**
     * Returns the 'list of embedded fonts' object of the specified \p page
     * of the document.
     *
     * \param page a page of the document, starting from 0 - -1 indicates all
     * the other fonts
     */
    virtual FontInfo::List fontsForPage(int page);

    /**
     * Returns the 'list of embedded files' object of the document or 0 if
     * no list of embedded files is available.
     */
    virtual const QList<EmbeddedFile*>* embeddedFiles() const;

    /**
     * This enum identifies the metric of the page size.
     */
    enum PageSizeMetric
    {
        None,  ///< The page size is not defined in a physical metric.
        Points,  ///< The page size is given in 1/72 inches.
        Pixels  ///< The page size is given in screen pixels @since 0.19
                ///< (KDE 4.13)
    };

    /**
     * This method returns the metric of the page size. Default is @ref None.
     */
    virtual PageSizeMetric pagesSizeMetric() const;

    /**
     * Returns whether the given @p action is allowed in the document.
     * @see @ref Okular::Permission
     */
    virtual bool isAllowed(Permission action) const;

    /**
     * This method is called when the orientation has been changed by the user.
     */
    virtual void rotationChanged(Rotation orientation, Rotation oldOrientation);

    /**
     * Returns the list of supported page sizes.
     */
    virtual PageSize::List pageSizes() const;

    /**
     * This method is called when the page size has been changed by the user.
     */
    virtual void pageSizeChanged(const PageSize& pageSize,
                                 const PageSize& oldPageSize);

    /**
     * This method is called to print the document to the given @p printer.
     */
    virtual Document::PrintError print(QPrinter& printer);

    /**
     * This method returns the meta data of the given @p key with the given @p
     * option of the document.
     */
    virtual QVariant metaData(const QString& key, const QVariant& option) const;

    /**
     * Returns the list of additional supported export formats.
     */
    virtual ExportFormat::List exportFormats() const;

    /**
     * This method is called to export the document in the given @p format and
     * save it under the given @p fileName. The format must be one of the
     * supported export formats.
     */
    virtual bool exportTo(const QString& fileName, const ExportFormat& format);

    /**
     * This method is called to know which wallet data should be used for the
     * given file name. Unless you have very special requirements to where
     * wallet data should be stored you don't need to reimplement this method.
     */
    virtual void walletDataForFile(const QString& fileName, QString* walletName,
                                   QString* walletFolder,
                                   QString* walletKey) const;

    /**
     * Query for the specified @p feature.
     */
    bool hasFeature(GeneratorFeature feature) const;

    /**
     * Update DPI of the generator
     *
     * @since 0.19 (old signature)
     * @since 22.04 (new signature)
     */
    void setDPI(const QSizeF dpi);

    /**
     * Returns the 'layers model' object of the document or NULL if
     * layers model is not available.
     *
     * @since 0.24
     */
    virtual QAbstractItemModel* layersModel() const;

    /**
     * Calls the backend to execute an BackendOpaqueAction
     */
    virtual void opaqueAction(const BackendOpaqueAction* action);

    /**
     * Frees the contents of the opaque action (if any);
     *
     * @since 22.04
     */
    virtual void freeOpaqueActionContents(const BackendOpaqueAction& action);

Q_SIGNALS:
    /**
     * This signal should be emitted whenever an error occurred in the
     * generator.
     *
     * @param message The message which should be shown to the user.
     * @param duration The time that the message should be shown to the user.
     */
    void error(const QString& message, int duration);

    /**
     * This signal should be emitted whenever the user should be warned.
     *
     * @param message The message which should be shown to the user.
     * @param duration The time that the message should be shown to the user.
     */
    void warning(const QString& message, int duration);

    /**
     * This signal should be emitted whenever the user should be noticed.
     *
     * @param message The message which should be shown to the user.
     * @param duration The time that the message should be shown to the user.
     */
    void notice(const QString& message, int duration);

protected:
    /**
     * This method must be called when the pixmap request triggered by
     * generatePixmap() has been finished.
     */
    void signalPixmapRequestDone(PixmapRequest* request);

    /**
     * This method must be called when a text generation has been finished.
     */
    void signalTextGenerationDone(Page* page, TextPage* textPage);

    /**
     * This method is called when the document is closed and not used
     * any longer.
     *
     * @returns true on success, false otherwise.
     */
    virtual bool doCloseDocument() = 0;

    /**
     * Returns the image of the page as specified in
     * the passed pixmap @p request.
     *
     * Must return a null image if the request was cancelled and the generator
     * supports cancelling
     *
     * @warning this method may be executed in its own separated thread if the
     * @ref Threaded is enabled!
     */
    virtual QImage image(PixmapRequest* request);

    /**
     * Returns the text page for the given @p request.
     *
     * Must return a null pointer if the request was cancelled and the generator
     * supports cancelling
     *
     * @warning this method may be executed in its own separated thread if the
     * @ref Threaded is enabled!
     *
     * @since 1.4
     */
    virtual TextPage* textPage(TextRequest* request);

    /**
     * Returns a pointer to the document.
     */
    const Document* document() const;

    /**
     * Toggle the @p feature .
     */
    void setFeature(GeneratorFeature feature, bool on = true);

    /**
     * Internal document setting
     */
    enum DocumentMetaDataKey
    {
        PaperColorMetaData,  ///< Returns (QColor) the paper color if set in
                             ///< Settings or the default color (white) if
                             ///< option is true (otherwise returns a non
                             ///< initialized QColor)
        TextAntialiasMetaData,  ///< Returns (bool) text antialias from Settings
                                ///< (option is not used)
        GraphicsAntialiasMetaData,  ///< Returns (bool)graphic antialias from
                                    ///< Settings (option is not used)
        TextHintingMetaData  ///< Returns (bool)text hinting from Settings
                             ///< (option is not used)
    };

    /**
     * Request a meta data of the Document, if available, like an internal
     * setting.
     *
     * @since 1.1
     */
    QVariant documentMetaData(const DocumentMetaDataKey key,
                              const QVariant& option = QVariant()) const;

    /**
     * Return the pointer to a mutex the generator can use freely.
     */
    QMutex* userMutex() const;

    /**
     * Set the bounding box of a page after the page has already been handed
     * to the Document. Call this instead of Page::setBoundingBox() to ensure
     * that all observers are notified.
     *
     * @since 0.7 (KDE 4.1)
     */
    void updatePageBoundingBox(int page, const NormalizedRect& boundingBox);

    /**
     * Returns DPI, previously set via setDPI()
     * @since 0.19 (KDE 4.13)
     */
    QSizeF dpi() const;

    /**
     * Gets the font data for the given font
     *
     * @since 0.8 (old signature)
     * @since 22.04 (new signature)
     */
    virtual QByteArray requestFontData(const Okular::FontInfo& font);

protected Q_SLOTS:
    /**
     * This method can be called to trigger a partial pixmap update for the
     * given request Make sure you call it in a way it's executed in the main
     * thread.
     * @since 1.3
     */
    void signalPartialPixmapRequest(Okular::PixmapRequest* request,
                                    const QImage& image);

protected:
    /// @cond PRIVATE
    Generator(GeneratorPrivate& dd, QObject* parent, const QVariantList& args);
    Q_DECLARE_PRIVATE(Generator)
    GeneratorPrivate* d_ptr;

    friend class Document;
    friend class DocumentPrivate;
    /// @endcond PRIVATE

private:
    Q_DISABLE_COPY(Generator)
};

/**
 * @short Describes a pixmap type request.
 */
class OKULARCORE_EXPORT PixmapRequest
{
    friend class Document;
    friend class DocumentPrivate;

public:
    enum PixmapRequestFeature
    {
        NoFeature = 0,
        Asynchronous = 1,
        Preload = 2
    };
    Q_DECLARE_FLAGS(PixmapRequestFeatures, PixmapRequestFeature)

    /**
     * Creates a new pixmap request.
     *
     * @param observer The observer.
     * @param pageNumber The page number.
     * @param width The width of the page in logical pixels.
     * @param height The height of the page in logical pixels.
     * @param priority The priority of the request.
     * @param features The features of generation.
     */
    [[deprecated(
        "This PixmapRequest constructor is deprecated, use the one "
        "including the device pixel ratio")]] PixmapRequest(DocumentObserver*
                                                                observer,
                                                            int pageNumber,
                                                            int width,
                                                            int height,
                                                            int priority,
                                                            PixmapRequestFeatures
                                                                features);

    /**
     * Creates a new pixmap request.
     *
     * @param observer The observer.
     * @param pageNumber The page number.
     * @param width The width of the page in logical pixels.
     * @param height The height of the page in logical pixels.
     * @param dpr Device pixel ratio of the screen that the pixmap is intended
     * for.
     * @param priority The priority of the request.
     * @param features The features of generation.
     */
    PixmapRequest(DocumentObserver* observer, int pageNumber, int width,
                  int height, qreal dpr, int priority,
                  PixmapRequestFeatures features);

    /**
     * Destroys the pixmap request.
     */
    ~PixmapRequest();

    /**
     * Returns the observer of the request.
     */
    DocumentObserver* observer() const;

    /**
     * Returns the page number of the request.
     */
    int pageNumber() const;

    /**
     * Returns the page width of the requested pixmap.
     */
    int width() const;

    /**
     * Returns the page height of the requested pixmap.
     */
    int height() const;

    /**
     * Returns the priority (less it better, 0 is maximum) of the
     * request.
     */
    int priority() const;

    /**
     * Returns whether the generation should be done synchronous or
     * asynchronous.
     *
     * If asynchronous, the pixmap is created in a thread and the observer
     * is notified when the job is done.
     */
    bool asynchronous() const;

    /**
     * Returns whether the generation request is for a page that is not
     * important i.e. it's just for speeding up future rendering
     */
    bool preload() const;

    /**
     * Returns a pointer to the page where the pixmap shall be generated for.
     */
    Page* page() const;

    /**
     * Sets whether the generator should render only the given normalized
     * rect or the entire page
     *
     * @since 0.16 (KDE 4.10)
     */
    void setTile(bool tile);

    /**
     * Returns whether the generator should render just the region given by
     * normalizedRect() or the entire page.
     *
     * @since 0.16 (KDE 4.10)
     */
    bool isTile() const;

    /**
     * Sets the region of the page to request.
     *
     * @since 0.16 (KDE 4.10)
     */
    void setNormalizedRect(const NormalizedRect& rect);

    /**
     * Returns the normalized region of the page to request.
     *
     * @since 0.16 (KDE 4.10)
     */
    const NormalizedRect& normalizedRect() const;

    /**
     * Sets whether the request should report back updates if possible
     *
     * @since 1.3
     */
    void setPartialUpdatesWanted(bool partialUpdatesWanted);

    /**
     * Should the request report back updates if possible?
     *
     * @since 1.3
     */
    bool partialUpdatesWanted() const;

    /**
     * Should the request be aborted if possible?
     *
     * @since 1.4
     */
    bool shouldAbortRender() const;

private:
    Q_DISABLE_COPY(PixmapRequest)

    friend class PixmapRequestPrivate;
    PixmapRequestPrivate* const d;
};

/**
 * @short Describes a text request.
 *
 * @since 1.4
 */
class OKULARCORE_EXPORT TextRequest
{
public:
    /**
     * Creates a new text request.
     */
    explicit TextRequest(Page* page);

    TextRequest();

    /**
     * Destroys the pixmap request.
     */
    ~TextRequest();

    /**
     * Returns a pointer to the page where the pixmap shall be generated for.
     */
    Page* page() const;

    /**
     * Should the request be aborted if possible?
     */
    bool shouldAbortExtraction() const;

private:
    Q_DISABLE_COPY(TextRequest)

    friend TextRequestPrivate;
    TextRequestPrivate* const d;
};

}  // namespace Okular

Q_DECLARE_METATYPE(Okular::PixmapRequest*)

    #define OkularGeneratorInterface_iid "org.kde.okular.Generator"
Q_DECLARE_INTERFACE(Okular::Generator, OkularGeneratorInterface_iid)

    #ifndef QT_NO_DEBUG_STREAM
OKULARCORE_EXPORT QDebug operator<<(QDebug str,
                                    const Okular::PixmapRequest& req);
    #endif

#endif

/* kate: replace-tabs on; indent-width 4; */
