/*
    SPDX-FileCopyrightText: 2004-2005 Enrico Ros <eros.kde@email.it>
    SPDX-FileCopyrightText: 2004-2008 Albert Astals Cid <aacid@kde.org>

    Work sponsored by the LiMux project of the city of Munich:
    SPDX-FileCopyrightText: 2017 Klarälvdalens Datakonsult AB a KDAB Group
   company <info@kdab.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_DOCUMENT_H_
    #define _OKULAR_DOCUMENT_H_

    #include <QDomDocument>
    #include <QMimeType>
    #include <QObject>
    #include <QPrinter>
    #include <QStringList>
    #include <QUrl>
    #include <QVector>
    #include "area.h"
    #include "global.h"
    #include "okularcore_export.h"
    #include "pagesize.h"

class KConfigDialog;
class KPluginMetaData;
class KXMLGUIClient;
class DocumentItem;
class QAbstractItemModel;

namespace Okular
{
class Annotation;
class BookmarkManager;
class CertificateStore;
class DocumentInfoPrivate;
class DocumentObserver;
class DocumentPrivate;
class DocumentSynopsis;
class DocumentViewport;
class EmbeddedFile;
class ExportFormat;
class FontInfo;
class FormField;
class FormFieldText;
class FormFieldButton;
class FormFieldChoice;
class Generator;
class Action;
class MovieAction;
class Page;
class PixmapRequest;
class RenditionAction;
class NewSignatureData;
struct NewSignatureDataPrivate;
class SourceReference;
class View;
class VisiblePageRect;
class SignatureInfo;

    /** IDs for seaches. Globally defined here. **/
    #define PART_SEARCH_ID 1
    #define PAGEVIEW_SEARCH_ID 2
    #define SW_SEARCH_ID 3
    #define PRESENTATION_SEARCH_ID 4

/**
 * The DocumentInfo structure can be filled in by generators to display
 * metadata about the currently opened file.
 */
class OKULARCORE_EXPORT DocumentInfo
{
    friend class Document;

public:
    /**
     * The list of predefined keys.
     */
    enum Key
    {
        Title,  ///< The title of the document
        Subject,  ///< The subject of the document
        Description,  ///< The description of the document
        Author,  ///< The author of the document
        Creator,  ///< The creator of the document (this can be different from
                  ///< the author)
        Producer,  ///< The producer of the document (e.g. some software)
        Copyright,  ///< The copyright of the document
        Pages,  ///< The number of pages of the document
        CreationDate,  ///< The date of creation of the document
        ModificationDate,  ///< The date of last modification of the document
        MimeType,  ///< The mime type of the document
        Category,  ///< The category of the document
        Keywords,  ///< The keywords which describe the content of the document
        FilePath,  ///< The path of the file @since 0.10 (KDE 4.4)
        DocumentSize,  ///< The size of the document @since 0.10 (KDE 4.4)
        PagesSize,  ///< The size of the pages (if all pages have the same size)
                    ///< @since 0.10 (KDE 4.4)
        CustomKeys,  ///< All the custom keys the generator supports @since 0.21
        Invalid  ///< An invalid key @since 0.21. It will always be the last
                 ///< element in the enum
    };

    /**
     * Creates a new document info.
     */
    DocumentInfo();
    DocumentInfo(const DocumentInfo& info);
    DocumentInfo& operator=(const DocumentInfo&);

    ~DocumentInfo();

    /**
     * Returns all the keys present in this DocumentInfo
     *
     * @since 0.21
     */
    QStringList keys() const;

    /**
     * Returns the value for a given key or an null string when the
     * key doesn't exist.
     */
    QString get(Key key) const;

    /**
     * Returns the value for a given key or an null string when the
     * key doesn't exist.
     */
    QString get(const QString& key) const;

    /**
     * Sets a value for a custom key. The title should be an i18n'ed
     * string, since it's used in the document information dialog.
     */
    void set(const QString& key, const QString& value,
             const QString& title = QString());

    /**
     * Sets a value for a special key. The title should be an i18n'ed
     * string, since it's used in the document information dialog.
     */
    void set(Key key, const QString& value);

    /**
     * Returns the user visible string for the given key
     * Takes into account keys added by the set() that takes a QString
     *
     * @since 0.21
     */
    QString getKeyTitle(const QString& key) const;

    /**
     * Returns the internal string for the given key
     * @since 0.10 (KDE 4.4)
     */
    static QString getKeyString(Key key);

    /**
     * Returns the user visible string for the given key
     * @since 0.10 (KDE 4.4)
     */
    static QString getKeyTitle(Key key);

    /**
     * Returns the Key from a string key
     * @since 0.21
     */
    static Key getKeyFromString(const QString& key);

private:
    DocumentInfoPrivate* d;
};

/**
 * @short The Document. Heart of everything. Actions take place here.
 *
 * The Document is the main object in Okular. All views query the Document to
 * get data/properties or even for accessing pages (in a 'const' way).
 *
 * It is designed to keep it detached from the document type (pdf, ps, you
 * name it..) so whenever you want to get some data, it asks its internal
 * generators to do the job and return results in a format-independent way.
 *
 * Apart from the generator (the currently running one) the document stores
 * all the Pages ('Page' class) of the current document in a vector and
 * notifies all the registered DocumentObservers when some content changes.
 *
 * For a better understanding of hierarchies @see README.internals.png
 * @see DocumentObserver, Page
 */
class OKULARCORE_EXPORT Document : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new document with the given @p widget as widget to relay GUI
     * things (messageboxes, ...).
     */
    explicit Document(QWidget* widget);

    /**
     * Destroys the document.
     */
    ~Document() override;

    /**
     * Describes the result of an open document operation.
     * @since 0.20 (KDE 4.14)
     */
    enum OpenResult
    {
        OpenSuccess,  //< The document was opened successfully
        OpenError,  //< The document failed to open
        OpenNeedsPassword  //< The document needs a password to be opened or the
                           // one provided is not the correct
    };

    /**
     * Opens the document.
     * @since 0.20 (KDE 4.14)
     */
    OpenResult openDocument(const QString& docFile, const QUrl& url,
                            const QMimeType& mime,
                            const QString& password = QString());

    /**
     * Closes the document.
     */
    void closeDocument();

    /**
     * Registers a new @p observer for the document.
     */
    void addObserver(DocumentObserver* observer);

    /**
     * Unregisters the given @p observer for the document.
     */
    void removeObserver(DocumentObserver* observer);

    /**
     * Reparses and applies the configuration.
     */
    void reparseConfig();

    /**
     * Returns whether the document is currently opened.
     */
    bool isOpened() const;

    /**
     * Returns the meta data of the document.
     */
    DocumentInfo documentInfo() const;

    /**
     * Returns the asked set of meta data of the document. The result may
     * contain more metadata than the one asked for.
     */
    DocumentInfo documentInfo(const QSet<DocumentInfo::Key>& keys) const;

    /**
     * Returns the table of content of the document or 0 if no
     * table of content is available.
     */
    const DocumentSynopsis* documentSynopsis() const;

    /**
     * Starts the reading of the information about the fonts in the
     * document, if available.
     *
     * The results as well the end of the reading is notified using the
     * signals gotFont(), fontReadingProgress() and fontReadingEnded()
     */
    void startFontReading();

    /**
     * Force the termination of the reading of the information about the
     * fonts in the document, if running.
     */
    void stopFontReading();

    /**
     * Whether the current document can provide information about the
     * fonts used in it.
     */
    bool canProvideFontInformation() const;

    /**
     * Whether the current document can perform digital signing.
     */
    bool canSign() const;

    /**
     * Returns the list of embedded files or 0 if no embedded files
     * are available.
     */
    const QList<EmbeddedFile*>* embeddedFiles() const;

    /**
     * Returns the page object for the given page @p number or 0
     * if the number is out of range.
     */
    const Page* page(int number) const;

    /**
     * Returns the current viewport of the document.
     */
    const DocumentViewport& viewport() const;

    /**
     * Sets the list of visible page rectangles.
     * @see VisiblePageRect
     */
    void setVisiblePageRects(const QVector<VisiblePageRect*>& visiblePageRects,
                             DocumentObserver* excludeObserver = nullptr);

    /**
     * Returns the list of visible page rectangles.
     */
    const QVector<VisiblePageRect*>& visiblePageRects() const;

    /**
     * Returns the number of the current page.
     */
    uint currentPage() const;

    /**
     * Returns the number of pages of the document.
     */
    uint pages() const;

    /**
     * Returns the url of the currently opened document.
     */
    QUrl currentDocument() const;

    /**
     * Returns whether the given @p action is allowed in the document.
     * @see @ref Permission
     */
    bool isAllowed(Permission action) const;

    /**
     * Returns whether the document supports searching.
     */
    bool supportsSearching() const;

    /**
     * Returns whether the document supports the listing of page sizes.
     */
    bool supportsPageSizes() const;

    /**
     * Returns whether the current document supports tiles
     *
     * @since 0.16 (KDE 4.10)
     */
    bool supportsTiles() const;

    /**
     * Returns the list of supported page sizes or an empty list if this
     * feature is not available.
     * @see supportsPageSizes()
     */
    PageSize::List pageSizes() const;

    /**
     * Returns whether the document supports the export to ASCII text.
     */
    bool canExportToText() const;

    /**
     * Exports the document as ASCII text and saves it under @p fileName.
     */
    bool exportToText(const QString& fileName) const;

    /**
     * Returns the list of supported export formats.
     * @see ExportFormat
     */
    QList<ExportFormat> exportFormats() const;

    /**
     * Exports the document in the given @p format and saves it under @p
     * fileName.
     */
    bool exportTo(const QString& fileName, const ExportFormat& format) const;

    /**
     * Returns whether the document history is at the begin.
     */
    bool historyAtBegin() const;

    /**
     * Returns whether the document history is at the end.
     */
    bool historyAtEnd() const;

    /**
     * Returns the meta data for the given @p key and @p option or an empty
     * variant if the key doesn't exists.
     */
    QVariant metaData(const QString& key,
                      const QVariant& option = QVariant()) const;

    /**
     * Returns the current rotation of the document.
     */
    Rotation rotation() const;

    /**
     * If all pages have the same size this method returns it, if the page sizes
     * differ an empty size object is returned.
     */
    QSizeF allPagesSize() const;

    /**
     * Returns the size string for the given @p page or an empty string
     * if the page is out of range.
     */
    QString pageSizeString(int page) const;

    /**
     * Returns the gui client of the generator, if it provides one.
     */
    KXMLGUIClient* guiClient();

    /**
     * Sets the current document viewport to the given @p page.
     *
     * @param page The number of the page.
     * @param excludeObserver The observer ids which shouldn't be effected by
     * this change.
     * @param smoothMove Whether the move shall be animated smoothly.
     */
    void setViewportPage(int page, DocumentObserver* excludeObserver = nullptr,
                         bool smoothMove = false);

    /**
     * Sets the current document viewport to the given @p viewport.
     *
     * @param viewport The document viewport.
     * @param excludeObserver The observer which shouldn't be effected by this
     * change.
     * @param smoothMove Whether the move shall be animated smoothly.
     */
    void setViewport(const DocumentViewport& viewport,
                     DocumentObserver* excludeObserver = nullptr,
                     bool smoothMove = false);

    /**
     * Sets the current document viewport to the given @p viewport.
     * BCI TODO: merge with setViewport, adding a parameter "bool updateHistory
     * = true"
     *
     * @param viewport The document viewport.
     * @param excludeObserver The observer which shouldn't be effected by this
     * change.
     * @param smoothMove Whether the move shall be animated smoothly.
     * @param updateHistory Whether to consider the change of viewport for the
     * history navigation
     */
    void setViewportWithHistory(const DocumentViewport& viewport,
                                DocumentObserver* excludeObserver = nullptr,
                                bool smoothMove = false,
                                bool updateHistory = true);

    /**
     * Sets the current document viewport to the next viewport in the
     * viewport history.
     */
    void setPrevViewport();

    /**
     * Sets the current document viewport to the previous viewport in the
     * viewport history.
     */
    void setNextViewport();

    /**
     * Sets the next @p viewport in the viewport history.
     */
    void setNextDocumentViewport(const DocumentViewport& viewport);

    /**
     * Sets the next @p namedDestination in the viewport history.
     *
     * @since 0.9 (KDE 4.3)
     */
    void setNextDocumentDestination(const QString& namedDestination);

    /**
     * Sets the zoom for the current document.
     */
    void setZoom(int factor, DocumentObserver* excludeObserver = nullptr);

    /**
     * Describes the possible options for the pixmap requests.
     */
    enum PixmapRequestFlag
    {
        NoOption = 0,  ///< No options
        RemoveAllPrevious = 1  ///< Remove all the previous requests, even for
                               ///< non requested page pixmaps
    };
    Q_DECLARE_FLAGS(PixmapRequestFlags, PixmapRequestFlag)

    /**
     * Sends @p requests for pixmap generation.
     *
     * The same as requestPixmaps( requests, RemoveAllPrevious );
     *
     * @since 22.08
     */
    void requestPixmaps(const QList<PixmapRequest*>& requests);

    /**
     * Sends @p requests for pixmap generation.
     *
     * @param requests the linked list of requests
     * @param reqOptions the options for the request
     *
     * @since 22.08
     */
    void requestPixmaps(const QList<PixmapRequest*>& requests,
                        PixmapRequestFlags reqOptions);

    /**
     * Sends a request for text page generation for the given page @p
     * pageNumber.
     */
    void requestTextPage(uint pageNumber);

    /**
     * Adds a new @p annotation to the given @p page.
     */
    void addPageAnnotation(int page, Annotation* annotation);

    /**
     * Tests if the @p annotation can be modified
     *
     * @since 0.15 (KDE 4.9)
     */
    bool canModifyPageAnnotation(const Annotation* annotation) const;

    /**
     *  Prepares to modify the properties of the given @p annotation.
     *  Must be called before the annotation's properties are modified
     *
     * @since 0.17 (KDE 4.11)
     */
    void prepareToModifyAnnotationProperties(Annotation* annotation);

    /**
     * Modifies the given @p annotation on the given @p page.
     * Must be preceded by a call to prepareToModifyAnnotationProperties before
     * the annotation's properties are modified
     *
     * @since 0.17 (KDE 4.11)
     */
    void modifyPageAnnotationProperties(int page, Annotation* annotation);

    /**
     * Translates the position of the given @p annotation on the given @p page
     * by a distance @p delta in normalized coordinates.
     *
     * Consecutive translations applied to the same @p annotation are merged
     * together on the undo stack if the BeingMoved flag is set on the @p
     * annotation.
     *
     * @since 0.17 (KDE 4.11)
     */
    void translatePageAnnotation(int page, Annotation* annotation,
                                 const Okular::NormalizedPoint& delta);

    /**
     * Adjusts the position of the top-left and bottom-right corners of given @p
     * annotation on the given @p page.
     *
     * Can be used to implement resize functionality.
     * @p delta1 in normalized coordinates is added to top-left.
     * @p delta2 in normalized coordinates is added to bottom-right.
     *
     * Consecutive adjustments applied to the same @p annotation are merged
     * together on the undo stack if the BeingResized flag is set on the @p
     * annotation.
     *
     * @since 1.1.0
     */
    void adjustPageAnnotation(int page, Annotation* annotation,
                              const Okular::NormalizedPoint& delta1,
                              const Okular::NormalizedPoint& delta2);

    /**
     * Edits the plain text contents of the given @p annotation on the given @p
     * page.
     *
     * The contents are set to @p newContents with cursor position @p
     * newCursorPos. The previous cursor position @p prevCursorPos and previous
     * anchor position @p prevAnchorPos must also be supplied so that they can
     * be restored if the edit action is undone.
     *
     * The Annotation's internal contents should not be modified prior to
     * calling this method.
     *
     * @since 0.17 (KDE 4.11)
     */
    void editPageAnnotationContents(int page, Annotation* annotation,
                                    const QString& newContents,
                                    int newCursorPos, int prevCursorPos,
                                    int prevAnchorPos);

    /**
     * Tests if the @p annotation can be removed
     *
     * @since 0.15 (KDE 4.9)
     */
    bool canRemovePageAnnotation(const Annotation* annotation) const;

    /**
     * Removes the given @p annotation from the given @p page.
     */
    void removePageAnnotation(int page, Annotation* annotation);

    /**
     * Removes the given @p annotations from the given @p page.
     */
    void removePageAnnotations(int page, const QList<Annotation*>& annotations);

    /**
     * Clears the text selection highlights for the given @p page,
     * creates new ones if @p rect is not nullptr,
     * and deletes @p rect.
     *
     * @param page The number of the page.
     * @param rect The rectangle of the selection.
     * @param color The color of the selection.
     */
    void setPageTextSelection(int page, RegularAreaRect* rect,
                              const QColor& color);

    /**
     * Returns true if there is an undo command available; otherwise returns
     * false.
     * @since 0.17 (KDE 4.11)
     */
    bool canUndo() const;

    /**
     * Returns true if there is a redo command available; otherwise returns
     * false.
     * @since 0.17 (KDE 4.11)
     */
    bool canRedo() const;

    /**
     * Describes the possible search types.
     */
    enum SearchType
    {
        NextMatch,  ///< Search next match
        PreviousMatch,  ///< Search previous match
        AllDocument,  ///< Search complete document
        GoogleAll,  ///< Search complete document (all words in google style)
        GoogleAny  ///< Search complete document (any words in google style)
    };

    /**
     * Describes how search ended
     */
    enum SearchStatus
    {
        MatchFound,  ///< Any match was found
        NoMatchFound,  ///< No match was found
        SearchCancelled  ///< The search was cancelled
    };

    /**
     * Searches the given @p text in the document.
     *
     * @param searchID The unique id for this search request.
     * @param text The text to be searched.
     * @param fromStart Whether the search should be started at begin of the
     * document.
     * @param caseSensitivity Whether the search is case sensitive.
     * @param type The type of the search. @ref SearchType
     * @param moveViewport Whether the viewport shall be moved to the position
     * of the matches.
     * @param color The highlighting color of the matches.
     */
    void searchText(int searchID, const QString& text, bool fromStart,
                    Qt::CaseSensitivity caseSensitivity, SearchType type,
                    bool moveViewport, const QColor& color);

    /**
     * Continues the search for the given @p searchID.
     */
    void continueSearch(int searchID);

    /**
     * Continues the search for the given @p searchID, optionally specifying
     * a new type for the search.
     *
     * @since 0.7 (KDE 4.1)
     */
    void continueSearch(int searchID, SearchType type);

    /**
     * Resets the search for the given @p searchID.
     */
    void resetSearch(int searchID);

    /**
     * Returns the bookmark manager of the document.
     */
    BookmarkManager* bookmarkManager() const;

    /**
     * Processes the given @p action.
     */
    void processAction(const Action* action);

    /**
     * Processes the given format @p action on @p fft.
     *
     * @since 1.9
     */
    void processFormatAction(const Action* action, Okular::FormFieldText* fft);

    /**
     * Processes the given keystroke @p action on @p fft.
     *
     * @since 1.9
     */
    void processKeystrokeAction(const Action* action,
                                Okular::FormFieldText* fft,
                                const QVariant& newValue);

    /**
     * Processes the given keystroke @p action on @p fft.
     * This will set event.willCommit=true
     *
     * @since 22.04
     */
    void processKeystrokeCommitAction(const Action* action,
                                      Okular::FormFieldText* fft);

    /**
     * Processes the given focus action on the field.
     *
     * @since 1.9
     */
    void processFocusAction(const Action* action, Okular::FormField* field);

    /**
     * Processes the given keystroke @p action on @p fft.
     *
     * @since 1.9
     */
    void processValidateAction(const Action* action, Okular::FormFieldText* fft,
                               bool& returnCode);

    /**
     * Returns a list of the bookmarked.pages
     */
    QList<int> bookmarkedPageList() const;

    /**
     * Returns the range of the bookmarked.pages
     */
    QString bookmarkedPageRange() const;

    /**
     * Processes/Executes the given source @p reference.
     */
    void processSourceReference(const SourceReference* reference);

    /**
     * Returns whether the document can configure the printer itself.
     */
    bool canConfigurePrinter() const;

    /**
     * What type of printing a document supports
     */
    enum PrintingType
    {
        NoPrinting,  ///< Printing Not Supported
        NativePrinting,  ///< Native Cross-Platform Printing
        PostscriptPrinting  ///< Postscript file printing
    };

    /**
     * Returns what sort of printing the document supports:
     *   Native, Postscript, None
     */
    PrintingType printingSupport() const;

    /**
     * Returns whether the document supports printing to both PDF and PS files.
     */
    bool supportsPrintToFile() const;

    /// @since 22.04
    enum PrintError
    {
        NoPrintError,  ///< Printing succeeded
        UnknownPrintError,
        TemporaryFileOpenPrintError,
        FileConversionPrintError,
        PrintingProcessCrashPrintError,
        PrintingProcessStartPrintError,
        PrintToFilePrintError,
        InvalidPrinterStatePrintError,
        UnableToFindFilePrintError,
        NoFileToPrintError,
        NoBinaryToPrintError,
        InvalidPageSizePrintError
    };

    /**
     * Prints the document to the given @p printer.
     */
    Document::PrintError print(QPrinter& printer);

    /// @since 22.04
    static QString printErrorString(PrintError error);

    /**
     * Returns a custom printer configuration page or 0 if no
     * custom printer configuration page is available.
     *
     * The returned object should be of a PrintOptionsWidget subclass
     * (which is not officially enforced by the signature for binary
     * compatibility reasons).
     */
    QWidget* printConfigurationWidget() const;

    /**
     * Fill the KConfigDialog @p dialog with the setting pages of the
     * generators.
     */
    void fillConfigDialog(KConfigDialog* dialog);

    /**
     * Returns the number of generators that have a configuration widget.
     */
    int configurableGenerators() const;

    /**
     * Returns the list with the supported MIME types.
     */
    QStringList supportedMimeTypes() const;

    /**
     * Returns the metadata associated with the generator. May be invalid.
     */
    KPluginMetaData generatorInfo() const;

    /**
     * Returns whether the generator supports hot-swapping the current file
     * with another identical file
     *
     * @since 1.3
     */
    bool canSwapBackingFile() const;

    /**
     * Reload the document from a new location, without any visible effect
     * to the user.
     *
     * The new file must be identical to the current one or, if the document
     * has been modified (eg the user edited forms and annotations), the new
     * document must have these changes too. For example, you can call
     * saveChanges first to write changes to a file and then swapBackingFile
     * to switch to the new location.
     *
     * @since 1.3
     */
    bool swapBackingFile(const QString& newFileName, const QUrl& url);

    /**
     * Same as swapBackingFile, but newFileName must be a .okular file.
     *
     * The new file must be identical to the current one or, if the document
     * has been modified (eg the user edited forms and annotations), the new
     * document must have these changes too. For example, you can call
     * saveDocumentArchive first to write changes to a file and then
     * swapBackingFileArchive to switch to the new location.
     *
     * @since 1.3
     */
    bool swapBackingFileArchive(const QString& newFileName, const QUrl& url);

    /**
     * Sets the history to be clean
     *
     * @since 1.3
     */
    void setHistoryClean(bool clean);

    bool isHistoryClean() const;

    /**
     * Saving capabilities. Their availability varies according to the
     * underlying generator and/or the document type.
     *
     * @see canSaveChanges (SaveCapability)
     * @since 0.15 (KDE 4.9)
     */
    enum SaveCapability
    {
        SaveFormsCapability = 1,  ///< Can save form changes
        SaveAnnotationsCapability = 2  ///< Can save annotation changes
    };

    /**
     * Returns whether it's possible to save a given category of changes to
     * another document.
     *
     * @since 0.15 (KDE 4.9)
     */
    bool canSaveChanges(SaveCapability cap) const;

    /**
     * Returns whether the changes to the document (modified annotations,
     * values in form fields, etc) can be saved to another document.
     *
     * Equivalent to the logical OR of canSaveChanges(SaveCapability) for
     * each capability.
     *
     * @since 0.7 (KDE 4.1)
     */
    bool canSaveChanges() const;

    /**
     * Save the document and the optional changes to it to the specified
     * @p fileName.
     *
     * @since 0.7 (KDE 4.1)
     */
    bool saveChanges(const QString& fileName);

    /**
     * Save the document and the optional changes to it to the specified
     * @p fileName and returns a @p errorText if fails.
     *
     * @since 0.10 (KDE 4.4)
     */
    bool saveChanges(const QString& fileName, QString* errorText);

    /**
     * Register the specified @p view for the current document.
     *
     * It is unregistered from the previous document, if any.
     *
     * @since 0.7 (KDE 4.1)
     */
    void registerView(View* view);

    /**
     * Unregister the specified @p view from the current document.
     *
     * @since 0.7 (KDE 4.1)
     */
    void unregisterView(View* view);

    /**
     * Gets the font data for the given font
     *
     * @since 0.8 (KDE 4.2)
     */
    QByteArray fontData(const FontInfo& font) const;

    /**
     * Opens a document archive.
     *
     * @since 0.20 (KDE 4.14)
     */
    OpenResult openDocumentArchive(const QString& docFile, const QUrl& url,
                                   const QString& password = QString());

    /**
     * Saves a document archive.
     *
     * @since 0.8 (KDE 4.2)
     */
    bool saveDocumentArchive(const QString& fileName);

    /**
     * Extract the document file from the current archive.
     *
     * @warning This function only works if the current file is a document
     * archive
     *
     * @since 1.3
     */
    bool extractArchivedFile(const QString& destFileName);

    /**
     * Asks the generator to dynamically generate a SourceReference for a given
     * page number and absolute X and Y position on this page.
     *
     * @attention Ownership of the returned SourceReference is transferred to
     * the caller.
     * @note This method does not call processSourceReference( const
     * SourceReference * )
     *
     * @since 0.10 (KDE 4.4)
     */
    const SourceReference* dynamicSourceReference(int pageNr, double absX,
                                                  double absY);

    /**
     * Returns the orientation of the document (for printing purposes). This
     * is used in the KPart to initialize the print dialog and in the
     * generators to check whether the document needs to be rotated or not.
     *
     * @since 0.14 (KDE 4.8)
     */
    QPrinter::Orientation orientation() const;

    /**
     * Control annotation editing (creation, modification and removal),
     * which is enabled by default.
     *
     * @since 0.15 (KDE 4.9)
     */
    void setAnnotationEditingEnabled(bool enable);

    /**
     * Returns which wallet data to use to read/write the password for the given
     * fileName
     *
     * @since 0.20 (KDE 4.14)
     */
    void walletDataForFile(const QString& fileName, QString* walletName,
                           QString* walletFolder, QString* walletKey) const;

    /**
     * Since version 0.21, okular does not allow editing annotations and
     * form data if they are stored in the docdata directory (like older
     * okular versions did by default).
     * If this flag is set, then annotations and forms cannot be edited.
     *
     * @since 1.3
     */
    bool isDocdataMigrationNeeded() const;

    /**
     * Delete annotations and form data from the docdata folder. Call it if
     * isDocdataMigrationNeeded() was true and you've just saved them to an
     * external file.
     *
     * @since 1.3
     */
    void docdataMigrationDone();

    /**
     * Returns the model for rendering layers (NULL if the document has no
     * layers)
     *
     * @since 0.24
     */
    QAbstractItemModel* layersModel() const;

    /**
     *  Returns the reason why the file opening failed, if any.
     *
     * @since 1.10
     */
    QString openError() const;

    /**
     * Digitally sign document
     *
     * @since 21.04
     */
    bool sign(const NewSignatureData& data, const QString& newPath);

    /**
     * Returns the generator's certificate store (if any)
     *
     * @since 21.04
     */
    CertificateStore* certificateStore() const;

    /** sets the editor command to the command  \p editCmd, as
     * given at the commandline.
     *
     * @since 22.04
     */
    void setEditorCommandOverride(const QString& editCmd);

    /** returns the overriding editor command.
     *
     * If the editor command was not overriden, the string is empty.
     *
     * @since 22.04
     */
    QString editorCommandOverride() const;

public Q_SLOTS:
    /**
     * This slot is called whenever the user changes the @p rotation of
     * the document.
     */
    void setRotation(int rotation);

    /**
     * This slot is called whenever the user changes the page @p size
     * of the document.
     */
    void setPageSize(const Okular::PageSize& size);

    /**
     * Cancels the current search
     */
    void cancelSearch();

    /**
     * Undo last edit command
     * @since 0.17 (KDE 4.11)
     */
    void undo();

    /**
     * Redo last undone edit command
     * @since 0.17 (KDE 4.11)
     */
    void redo();

    /**
     * Edit the text contents of the specified @p form on page @p page to be @p
     * newContents. The new text cursor position (@p newCursorPos), previous
     * text cursor position (@p prevCursorPos), and previous cursor anchor
     * position will be restored by the undo / redo commands.
     * @since 0.17 (KDE 4.11)
     */
    void editFormText(int pageNumber, Okular::FormFieldText* form,
                      const QString& newContents, int newCursorPos,
                      int prevCursorPos, int prevAnchorPos);

    /**
     * Edit the selected list entries in @p form on page @p page to be @p
     * newChoices.
     * @since 0.17 (KDE 4.11)
     */
    void editFormList(int pageNumber, Okular::FormFieldChoice* form,
                      const QList<int>& newChoices);

    /**
     * Set the active choice in the combo box @p form on page @p page to @p
     * newText The new cursor position (@p newCursorPos), previous cursor
     * position
     * (@p prevCursorPos), and previous anchor position (@p prevAnchorPos)
     * will be restored by the undo / redo commands.
     *
     * @since 0.17 (KDE 4.11)
     */
    void editFormCombo(int pageNumber, Okular::FormFieldChoice* form,
                       const QString& newText, int newCursorPos,
                       int prevCursorPos, int prevAnchorPos);

    /**
     * Set the states of the group of form buttons @p formButtons on page @p
     * page to @p newButtonStates. The lists @p formButtons and @p
     * newButtonStates should be the same length and true values in @p
     * newButtonStates indicate that the corresponding entry in @p formButtons
     * should be enabled.
     */
    void editFormButtons(int pageNumber,
                         const QList<Okular::FormFieldButton*>& formButtons,
                         const QList<bool>& newButtonStates);

    /**
     * Reloads the pixmaps for whole document
     *
     * @since 0.24
     */
    void reloadDocument() const;

    /**
     * Returns the part of document covered by the given signature @p info.
     *
     * @since 1.7
     */
    QByteArray requestSignedRevisionData(const Okular::SignatureInfo& info);

    /**
     * Refresh the pixmaps for the given @p pageNumber.
     *
     * @since 1.10
     */
    void refreshPixmaps(int pageNumber);

Q_SIGNALS:
    /**
     * This signal is emitted whenever the document is about to close.
     * @since 1.5.3
     */
    void aboutToClose();

    /**
     * This signal is emitted whenever an action requests a
     * document close operation.
     */
    void close();

    /**
     * This signal is emitted whenever an action requests a
     * document print operation.
     * @since 22.04
     */
    void requestPrint();

    /**
     * This signal is emitted whenever an action requests a
     * document save as operation.
     * @since 22.04
     */
    void requestSaveAs();

    /**
     * This signal is emitted whenever an action requests an
     * application quit operation.
     */
    void quit();

    /**
     * This signal is emitted whenever an action requests a
     * find operation.
     */
    void linkFind();

    /**
     * This signal is emitted whenever an action requests a
     * goto operation.
     */
    void linkGoToPage();

    /**
     * This signal is emitted whenever an action requests a
     * start presentation operation.
     */
    void linkPresentation();

    /**
     * This signal is emitted whenever an action requests an
     * end presentation operation.
     */
    void linkEndPresentation();

    /**
     * This signal is emitted whenever an action requests an
     * open url operation for the given document @p url.
     */
    void openUrl(const QUrl& url);

    /**
     * This signal is emitted whenever an error occurred.
     *
     * @param text The description of the error.
     * @param duration The time in milliseconds the message should be shown to
     * the user.
     */
    void error(const QString& text, int duration);

    /**
     * This signal is emitted to signal a warning.
     *
     * @param text The description of the warning.
     * @param duration The time in milliseconds the message should be shown to
     * the user.
     */
    void warning(const QString& text, int duration);

    /**
     * This signal is emitted to signal a notice.
     *
     * @param text The description of the notice.
     * @param duration The time in milliseconds the message should be shown to
     * the user.
     */
    void notice(const QString& text, int duration);

    /**
     * Emitted when a new font is found during the reading of the fonts of
     * the document.
     */
    void gotFont(const Okular::FontInfo& font);

    /**
     * Reports the progress when reading the fonts in the document.
     *
     * \param page is the page that was just finished to scan for fonts
     */
    void fontReadingProgress(int page);

    /**
     * Reports that the reading of the fonts in the document is finished.
     */
    void fontReadingEnded();

    /**
     * Reports that the current search finished
     */
    void searchFinished(int searchID, Okular::Document::SearchStatus endStatus);

    /**
     * This signal is emitted whenever a source reference with the given
     * parameters has been activated.
     *
     * \param absFileName absolute name of the file.
     * \param line line number.
     * \param col column number.
     * \param handled should be set to 'true' if a slot handles this source
     * reference; the default action to launch the configured editor will then
     * not be performed by the document
     *
     * @since 0.14 (KDE 4.8)
     */
    void sourceReferenceActivated(const QString& absFileName, int line, int col,
                                  bool* handled);

    /**
     * This signal is emitted whenever an movie action is triggered and the UI
     * should process it.
     */
    void processMovieAction(const Okular::MovieAction* action);

    /**
     * This signal is emitted whenever the availability of the undo function
     * changes
     * @since 0.17 (KDE 4.11)
     */
    void canUndoChanged(bool undoAvailable);

    /**
     * This signal is emitted whenever the availability of the redo function
     * changes
     * @since 0.17 (KDE 4.11)
     */
    void canRedoChanged(bool redoAvailable);

    /**
     * This signal is emitted whenever the undo history is clean (i.e. the same
     * status the last time it was saved)
     * @since 1.3
     */
    void undoHistoryCleanChanged(bool clean);

    /**
     * This signal is emitted whenever an rendition action is triggered and the
     * UI should process it.
     *
     * @since 0.16 (KDE 4.10)
     */
    void processRenditionAction(const Okular::RenditionAction* action);

    /**
     * This signal is emitted whenever the contents of the given @p annotation
     * are changed by an undo or redo action.
     *
     * The new contents (@p contents), cursor position (@p cursorPos), and
     * anchor position (@p anchorPos) are included
     * @since 0.17 (KDE 4.11)
     */
    void annotationContentsChangedByUndoRedo(Okular::Annotation* annotation,
                                             const QString& contents,
                                             int cursorPos, int anchorPos);

    /**
     * This signal is emitted whenever the text contents of the given text @p
     * form on the given @p page are changed by an undo or redo action.
     *
     * The new text contents (@p contents), cursor position (@p cursorPos), and
     * anchor position (@p anchorPos) are included
     * @since 0.17 (KDE 4.11)
     */
    void formTextChangedByUndoRedo(int page, Okular::FormFieldText* form,
                                   const QString& contents, int cursorPos,
                                   int anchorPos);

    /**
     * This signal is emitted whenever the selected @p choices for the given
     * list @p form on the given @p page are changed by an undo or redo action.
     * @since 0.17 (KDE 4.11)
     */
    void formListChangedByUndoRedo(int page, Okular::FormFieldChoice* form,
                                   const QList<int>& choices);

    /**
     * This signal is emitted whenever the active @p text for the given combo @p
     * form on the given @p page is changed by an undo or redo action.
     * @since 0.17 (KDE 4.11)
     */
    void formComboChangedByUndoRedo(int page, Okular::FormFieldChoice* form,
                                    const QString& text, int cursorPos,
                                    int anchorPos);

    /**
     * This signal is emitted whenever the state of the specified group of form
     * buttons (@p formButtons) on the given @p page is changed by an undo or
     * redo action.
     * @since 0.17 (KDE 4.11)
     */
    void formButtonsChangedByUndoRedo(
        int page, const QList<Okular::FormFieldButton*>& formButtons);

    /**
     * This signal is emitted whenever a FormField was changed programmatically
     * and the according widget should be updated.
     * @since 1.4
     */
    void refreshFormWidget(Okular::FormField* field);

private:
    /// @cond PRIVATE
    friend class DocumentPrivate;
    friend class ::DocumentItem;
    friend class EditAnnotationContentsCommand;
    friend class EditFormTextCommand;
    friend class EditFormListCommand;
    friend class EditFormComboCommand;
    friend class EditFormButtonsCommand;
    /// @endcond
    DocumentPrivate* const d;

    Q_DISABLE_COPY(Document)
};

/**
 * @short A view on the document.
 *
 * The Viewport structure is the 'current view' over the document. Contained
 * data is broadcasted between observers to synchronize their viewports to get
 * the 'I scroll one view and others scroll too' views.
 */
class OKULARCORE_EXPORT DocumentViewport
{
public:
    /**
     * Creates a new viewport for the given page @p number.
     */
    explicit DocumentViewport(int number = -1);

    /**
     * Creates a new viewport from the given @p xmlDesc.
     */
    explicit DocumentViewport(const QString& xmlDesc);

    /**
     * Returns the viewport as xml description.
     */
    QString toString() const;

    /**
     * Returns whether the viewport is valid.
     */
    bool isValid() const;

    /**
     * @internal
     */
    bool operator==(const DocumentViewport& other) const;
    bool operator<(const DocumentViewport& other) const;

    /**
     * The number of the page nearest the center of the viewport.
     */
    int pageNumber;

    /**
     * Describes the relative position of the viewport.
     */
    enum Position
    {
        Center = 1,  ///< Relative to the center of the page.
        TopLeft = 2  ///< Relative to the top left corner of the page.
    };

    /**
     * If 'rePos.enabled == true' then this structure contains the
     * viewport center or top left depending on the value of pos.
     */
    struct
    {
        bool enabled;
        double normalizedX;
        double normalizedY;
        Position pos;
    } rePos;

    /**
     * If 'autoFit.enabled == true' then the page must be autofit in the
     * viewport.
     */
    struct
    {
        bool enabled;
        bool width;
        bool height;
    } autoFit;
};

/**
 * @short A DOM tree that describes the Table of Contents.
 *
 * The Synopsis (TOC or Table Of Contents for friends) is represented via
 * a dom tree where each node has an internal name (displayed in the TOC)
 * and one or more attributes.
 *
 * In the tree the tag name is the 'screen' name of the entry. A tag can have
 * attributes. Here follows the list of tag attributes with meaning:
 * - Destination: A string description of the referred viewport
 * - DestinationName: A 'named reference' to the viewport that must be converted
 *      using metaData( "NamedViewport", viewport_name )
 * - ExternalFileName: A document to be opened, whose destination is specified
 *      with Destination or DestinationName
 * - Open: a boolean saying whether its TOC branch is open or not (default:
 * false)
 * - URL: a URL to be open as destination; if set, no other Destination* or
 *      ExternalFileName entry is used
 */
class OKULARCORE_EXPORT DocumentSynopsis : public QDomDocument
{
public:
    /**
     * Creates a new document synopsis object.
     */
    DocumentSynopsis();

    /**
     * Creates a new document synopsis object with the given
     * @p document as parent node.
     */
    explicit DocumentSynopsis(const QDomDocument& document);
};

/**
 * @short An embedded file into the document.
 *
 * This class represents a sort of interface of an embedded file in a document.
 *
 * Generators \b must re-implement its members to give the all the information
 * about an embedded file, like its name, its description, the date of creation
 * and modification, and the real data of the file.
 */
class OKULARCORE_EXPORT EmbeddedFile
{
public:
    /**
     * Creates a new embedded file.
     */
    EmbeddedFile();

    /**
     * Destroys the embedded file.
     */
    virtual ~EmbeddedFile();

    EmbeddedFile(const EmbeddedFile&) = delete;
    EmbeddedFile& operator=(const EmbeddedFile&) = delete;

    /**
     * Returns the name of the file
     */
    virtual QString name() const = 0;

    /**
     * Returns the description of the file, or an empty string if not
     * available
     */
    virtual QString description() const = 0;

    /**
     * Returns the real data representing the file contents
     */
    virtual QByteArray data() const = 0;

    /**
     * Returns the size (in bytes) of the file, if available, or -1 otherwise.
     *
     * @note this method should be a fast way to know the size of the file
     * with no need to extract all the data from it
     */
    virtual int size() const = 0;

    /**
     * Returns the modification date of the file, or an invalid date
     * if not available
     */
    virtual QDateTime modificationDate() const = 0;

    /**
     * Returns the creation date of the file, or an invalid date
     * if not available
     */
    virtual QDateTime creationDate() const = 0;
};

/**
 * @short An area of a specified page
 */
class OKULARCORE_EXPORT VisiblePageRect
{
public:
    /**
     * Creates a new visible page rectangle.
     *
     * @param pageNumber The page number where the rectangle is located.
     * @param rectangle The rectangle in normalized coordinates.
     */
    explicit VisiblePageRect(
        int pageNumber = -1,
        const NormalizedRect& rectangle = NormalizedRect());

    /**
     * The page number where the rectangle is located.
     */
    int pageNumber;

    /**
     * The rectangle in normalized coordinates.
     */
    NormalizedRect rect;
};

/**
 * @short Data needed to create a new signature
 *
 * @since 21.04
 */
class OKULARCORE_EXPORT NewSignatureData
{
public:
    NewSignatureData();
    ~NewSignatureData();
    NewSignatureData(const NewSignatureData&) = delete;
    NewSignatureData& operator=(const NewSignatureData&) = delete;

    QString certNickname() const;
    void setCertNickname(const QString& certNickname);

    QString certSubjectCommonName() const;
    void setCertSubjectCommonName(const QString& certSubjectCommonName);

    QString password() const;
    void setPassword(const QString& password);

    int page() const;
    void setPage(int page);

    NormalizedRect boundingRectangle() const;
    void setBoundingRectangle(const NormalizedRect& rect);

    /// @since 22.04
    QString documentPassword() const;

    /// @since 22.04
    void setDocumentPassword(const QString& password);

private:
    NewSignatureDataPrivate* const d;
};

}  // namespace Okular

Q_DECLARE_METATYPE(Okular::DocumentInfo::Key)
Q_DECLARE_OPERATORS_FOR_FLAGS(Okular::Document::PixmapRequestFlags)

#endif

/* kate: replace-tabs on; indent-width 4; */
