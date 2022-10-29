/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TEXTDOCUMENTGENERATOR_H_
#define _OKULAR_TEXTDOCUMENTGENERATOR_H_

#include "../interfaces/configinterface.h"
#include "document.h"
#include "generator.h"
#include "okularcore_export.h"
#include "textdocumentsettings.h"

class QTextBlock;
class QTextDocument;

namespace Okular
{
class TextDocumentConverterPrivate;
class TextDocumentGenerator;
class TextDocumentGeneratorPrivate;

class OKULARCORE_EXPORT TextDocumentConverter : public QObject
{
    Q_OBJECT

    friend class TextDocumentGenerator;
    friend class TextDocumentGeneratorPrivate;

public:
    /**
     * Creates a new generic converter.
     */
    TextDocumentConverter();

    /**
     * Destroys the generic converter.
     */
    ~TextDocumentConverter() override;

    /**
     * Returns the generated QTextDocument object. The caller takes ownership of
     * the QTextDocument
     *
     * @note there is no need to implement this one if you implement
     * convertWithPassword
     */
    virtual QTextDocument* convert(const QString& fileName);

    /**
     * Returns the generated QTextDocument object.
     */
    virtual Document::OpenResult convertWithPassword(const QString& fileName,
                                                     const QString& password);

    /**
     * Returns the generated QTextDocument object. Will be null if convert
     * didn't succeed
     */
    QTextDocument* document();

Q_SIGNALS:
    /**
     * Adds a new link object which is located between cursorBegin and
     * cursorEnd to the generator.
     */
    void addAction(Okular::Action* link, int cursorBegin, int cursorEnd);

    /**
     * Adds a new annotation object which is located between cursorBegin and
     * cursorEnd to the generator.
     */
    void addAnnotation(Okular::Annotation* annotation, int cursorBegin,
                       int cursorEnd);

    /**
     * Adds a new title at the given level which is located as position to the
     * generator.
     */
    void addTitle(int level, const QString& title, const QTextBlock& position);

    /**
     * Adds a set of meta data to the generator.
     *
     * @since 0.7 (KDE 4.1)
     */
    void addMetaData(DocumentInfo::Key key, const QString& value);

    /**
     * This signal should be emitted whenever an error occurred in the
     * converter.
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
     * Sets the converted QTextDocument object.
     */
    void setDocument(QTextDocument* document);

    /**
     * This method can be used to calculate the viewport for a given text block.
     *
     * @note This method should be called at the end of the conversion, because
     * it triggers QTextDocument to do the layout calculation.
     */
    DocumentViewport calculateViewport(QTextDocument* document,
                                       const QTextBlock& block);

    /**
     * Returns the generator that owns this converter.
     *
     * @note May be null if the converter was not created for a generator.
     *
     * @since 0.7 (KDE 4.1)
     */
    TextDocumentGenerator* generator() const;

private:
    TextDocumentConverterPrivate* d_ptr;
    Q_DECLARE_PRIVATE(TextDocumentConverter)
    Q_DISABLE_COPY(TextDocumentConverter)
};

/**
 * @brief QTextDocument-based Generator
 *
 * This generator provides a document in the form of a QTextDocument object,
 * parsed using a specialized TextDocumentConverter.
 */
class OKULARCORE_EXPORT TextDocumentGenerator : public Generator,
                                                public Okular::ConfigInterface
{
    /// @cond PRIVATE
    friend class TextDocumentConverter;
    /// @endcond

    Q_OBJECT
    Q_INTERFACES(Okular::ConfigInterface)

public:
    /**
     * Creates a new generator that uses the specified @p converter.
     *
     * @param converter The text document converter.
     * @param configName - see Okular::TextDocumentSettings
     * @param parent The parent object.
     * @param args The arguments.
     *
     * @note the generator will take ownership of the converter, so you
     *       don't have to delete it yourself
     * @since 0.17 (KDE 4.11)
     */
    TextDocumentGenerator(TextDocumentConverter* converter,
                          const QString& configName, QObject* parent,
                          const QVariantList& args);

    ~TextDocumentGenerator() override;

    // [INHERITED] load a document and fill up the pagesVector
    Document::OpenResult loadDocumentWithPassword(
        const QString& fileName, QVector<Okular::Page*>& pagesVector,
        const QString& password) override;

    // [INHERITED] perform actions on document / pages
    bool canGeneratePixmap() const override;
    void generatePixmap(Okular::PixmapRequest* request) override;

    // [INHERITED] print document using already configured QPrinter
    Document::PrintError print(QPrinter& printer) override;

    // [INHERITED] text exporting
    Okular::ExportFormat::List exportFormats() const override;
    bool exportTo(const QString& fileName,
                  const Okular::ExportFormat& format) override;

    // [INHERITED] config interface
    /// By default checks if the default font has changed or not
    bool reparseConfig() override;
    /// Does nothing by default. You need to reimplement it in your generator
    void addPages(KConfigDialog* dlg) override;

    /**
     * Config skeleton for TextDocumentSettingsWidget
     *
     * You must use new construtor to initialize TextDocumentSettings,
     * that contain @p configName .
     *
     * @since 0.17 (KDE 4.11)
     */
    TextDocumentSettings* generalSettings();

    Okular::DocumentInfo generateDocumentInfo(
        const QSet<DocumentInfo::Key>& keys) const override;
    const Okular::DocumentSynopsis* generateDocumentSynopsis() override;

protected:
    bool doCloseDocument() override;
    Okular::TextPage* textPage(Okular::TextRequest* request) override;

    /* @since 1.8 */
    TextDocumentConverter* converter();

    /* @since 1.8 */
    void setTextDocument(QTextDocument* textDocument);

private:
    Q_DECLARE_PRIVATE(TextDocumentGenerator)
    Q_DISABLE_COPY(TextDocumentGenerator)
};

}  // namespace Okular

#endif
