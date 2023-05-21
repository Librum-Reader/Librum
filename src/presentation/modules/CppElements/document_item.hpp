#pragma once

#include <core/document.h>
#include <core/observer.h>
#include <QObject>
#include "filtered_tocmodel.hpp"
#include "settings.hpp"


class SignatureModel;
class TOCModel;

namespace Okular
{
class Document;
}

namespace cpp_elements
{

class Observer;

class DocumentItem : public QObject
{
    Q_OBJECT

    /**
     * Absolute URI to document file to open
     */
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

    /**
     * Suggested window title if a window represents this document. may be
     * pathname or document title, depending on Okular settings.
     */
    Q_PROPERTY(QString windowTitleForDocument READ windowTitleForDocument NOTIFY
                   windowTitleForDocumentChanged)

    /**
     * Current displaying page for the document
     */
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY
                   currentPageChanged)

    /**
     * True if this DocumentItem instance has a document file opened
     */
    Q_PROPERTY(bool opened READ isOpened NOTIFY openedChanged)

    /**
     * True if this DocumentItem instance needs a password to open the document
     */
    Q_PROPERTY(
        bool needsPassword READ needsPassword NOTIFY needsPasswordChanged)

    /**
     * How many pages there are in the document
     */
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

    /**
     * True if the document is able to perform full text searches in its
     * contents
     */
    Q_PROPERTY(bool supportsSearching READ supportsSearching NOTIFY
                   supportsSearchingChanged)

    /**
     * True if a search is currently in progress and results didn't arrive yet
     */
    Q_PROPERTY(bool searchInProgress READ isSearchInProgress NOTIFY
                   searchInProgressChanged)

    /**
     * A list of all pages that contain a match for the search terms. If no text
     * has been searched, all pages are returned.
     */
    Q_PROPERTY(QVariantList matchingPages READ matchingPages NOTIFY
                   matchingPagesChanged)

    /**
     * Table of contents for the document, if available
     */
    Q_PROPERTY(
        Okular::FilteredTOCModel* tableOfContents READ tableOfContents CONSTANT)

    /**
     * Signatures model, if available
     */
    Q_PROPERTY(SignatureModel* signaturesModel READ signaturesModel CONSTANT)

    /**
     * List of pages that contain a bookmark
     */
    Q_PROPERTY(QVariantList bookmarkedPages READ bookmarkedPages NOTIFY
                   bookmarkedPagesChanged)

    /**
     * list of bookmarks urls valid on this page
     */
    Q_PROPERTY(QStringList bookmarks READ bookmarks NOTIFY bookmarksChanged)

public:
    explicit DocumentItem(QObject* parent = nullptr);
    ~DocumentItem() override;

    void setUrl(const QUrl& url);
    QUrl url() const;

    QString windowTitleForDocument() const;

    void setCurrentPage(int page);
    int currentPage() const;

    bool isOpened() const;

    bool needsPassword() const
    {
        return m_needsPassword;
    }

    int pageCount() const;

    bool supportsSearching() const;

    bool isSearchInProgress() const;

    QVariantList matchingPages() const;

    Okular::FilteredTOCModel* tableOfContents();

    SignatureModel* signaturesModel() const;

    QVariantList bookmarkedPages() const;

    QStringList bookmarks() const;

    // This could be a property, but maybe we want to have parameter for
    // searchText
    /**
     * Performs a search in the document
     *
     * @param text the string to search in the document
     */
    Q_INVOKABLE void searchText(const QString& text);

    /**
     * Reset the search over the document.
     */
    Q_INVOKABLE void resetSearch();

    /**
     * Tries to reopen the document with the given password.
     */
    Q_INVOKABLE void setPassword(const QString& password);

    Q_INVOKABLE void expand(const QModelIndex&)
    {
        // This method just exists for fixing a warning of something trying to
        // call a non-existent "expand" method
        ;
    };

    // Internal, not binded to qml
    Okular::Document* document();
    Observer* pageviewObserver();
    Observer* thumbnailObserver();

Q_SIGNALS:
    void urlChanged();
    void pageCountChanged();
    void openedChanged();
    void needsPasswordChanged();
    void searchInProgressChanged();
    void matchingPagesChanged();
    void currentPageChanged();
    void supportsSearchingChanged();
    void bookmarkedPagesChanged();
    void bookmarksChanged();
    void windowTitleForDocumentChanged();

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

private Q_SLOTS:
    void searchFinished(int id, Okular::Document::SearchStatus endStatus);

private:
    void openUrl(const QUrl& url, const QString& password);

    Okular::Document* m_document;
    TOCModel* m_tocModel;
    Okular::FilteredTOCModel m_filteredTocModel;
    SignatureModel* m_signaturesModel;
    Observer* m_thumbnailObserver;
    Observer* m_pageviewObserver;
    QVariantList m_matchingPages;
    bool m_searchInProgress;
    bool m_needsPassword = false;
};

class Observer : public QObject,
                 public Okular::DocumentObserver
{
    Q_OBJECT

public:
    explicit Observer(DocumentItem* parent);
    ~Observer() override;

    // inherited from DocumentObserver
    void notifyPageChanged(int page, int flags) override;

Q_SIGNALS:
    void pageChanged(int page, int flags);

private:
    DocumentItem* m_document;
};
}  // namespace cpp_elements