#pragma once
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QVariantMap>
#include <set>
#include "adapters_export.hpp"
#include "book_dto.hpp"
#include "book_title_proxy_model.hpp"
#include "library_proxy_model.hpp"

namespace adapters
{

/**
 * The LibraryController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for book operations. It acts as a
 * layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class ADAPTERS_EXPORT ILibraryController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::LibraryProxyModel* libraryModel READ
                   getLibraryModel CONSTANT)
    Q_PROPERTY(adapters::data_models::BookTitleProxyModel* bookTitleModel READ
                   getBookTitleModel CONSTANT)
    Q_PROPERTY(int bookCount READ getBookCount NOTIFY bookCountChanged)
    Q_PROPERTY(bool isSyncing READ isSyncing NOTIFY isSyncingChanged)

public:
    virtual ~ILibraryController() noexcept = default;

    enum class MetaProperty
    {
        Title = 0,
        Authors,
        FilePath,
        Creator,
        CreationDate,
        Format,
        Language,
        DocumentSize,
        PagesSize,
        PageCount,
        CurrentPage,
        AddedToLibrary,
        LastModified,
        ParentFolderId,
        Invalid
    };
    Q_ENUM(MetaProperty);

    Q_INVOKABLE virtual void syncWithServer() = 0;
    Q_INVOKABLE virtual int addBook(const QString& path,
                                    bool allowDuplicates = false,
                                    int projectGutenbergId = 0) = 0;
    Q_INVOKABLE virtual int deleteBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int deleteAllBooks() = 0;
    Q_INVOKABLE virtual int uninstallBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int downloadBookMedia(const QString& uuid) = 0;
    Q_INVOKABLE virtual int updateBook(const QString& uuid,
                                       const QVariant& operations) = 0;
    Q_INVOKABLE virtual int changeBookCover(const QString& uuid,
                                            const QString& path) = 0;
    Q_INVOKABLE virtual int addTag(const QString& bookUuid,
                                   const QString& tagName,
                                   const QString& tagUuid) = 0;
    Q_INVOKABLE virtual int removeTag(const QString& bookUuid,
                                      const QString& tagUuid) = 0;
    Q_INVOKABLE virtual void removeAllTagsWithUuid(const QString& tagUuid) = 0;
    Q_INVOKABLE virtual void renameTags(const QString& oldName,
                                        const QString& newName) = 0;
    Q_INVOKABLE virtual adapters::dtos::BookDto getBook(
        const QString& uuid) = 0;
    Q_INVOKABLE virtual int getBookCount() const = 0;
    Q_INVOKABLE virtual bool isSyncing() const = 0;
    Q_INVOKABLE virtual void removeAllBooksFromFolderWithId(
        const QString& folderId) = 0;

    Q_INVOKABLE virtual int saveBookToFile(const QString& uuid,
                                           const QUrl& path) = 0;
    virtual data_models::LibraryProxyModel* getLibraryModel() = 0;
    virtual data_models::BookTitleProxyModel* getBookTitleModel() = 0;

public slots:
    Q_INVOKABLE virtual void refreshLastOpenedFlag(const QString& uuid) = 0;

signals:
    void bookCountChanged();
    void isSyncingChanged();
    void storageLimitExceeded();
    void addingBookFinished(int projectGutenbergId, bool result);
    void downloadedProjectGutenbergIdsReady(const std::set<int>& ids);
};

}  // namespace adapters
