#pragma once
#include <QObject>
#include <QUrl>
#include <QUuid>
#include <vector>
#include "book.hpp"
#include "book_operation_status.hpp"
#include "tag.hpp"

namespace application
{

/**
 *  The BookService handles all the operations on books.
 */
class IBookService : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookService() noexcept = default;

    virtual BookOperationStatus addBook(const QString& filePath) = 0;
    virtual BookOperationStatus deleteBook(const QUuid& uuid) = 0;
    virtual BookOperationStatus uninstallBook(const QUuid& uuid) = 0;
    virtual BookOperationStatus downloadBookMedia(const QUuid& uuid) = 0;
    virtual BookOperationStatus updateBook(
        const domain::entities::Book& newBook) = 0;
    virtual BookOperationStatus changeBookCover(const QUuid& uuid,
                                                const QString& filePath) = 0;

    virtual const std::vector<domain::entities::Book>& getBooks() const = 0;
    virtual const domain::entities::Book* getBook(const QUuid& uuid) const = 0;
    virtual domain::entities::Book* getBook(const QUuid& uuid) = 0;
    virtual int getBookIndex(const QUuid& uuid) const = 0;
    virtual int getBookCount() const = 0;

    virtual BookOperationStatus addTagToBook(
        const QUuid& uuid, const domain::entities::Tag& tag) = 0;
    virtual BookOperationStatus removeTagFromBook(const QUuid& bookUuid,
                                                  const QUuid& tagUuid) = 0;
    virtual BookOperationStatus renameTagOfBook(const QUuid& bookUuid,
                                                const QUuid& tagUuid,
                                                const QString& newName) = 0;

    virtual BookOperationStatus saveBookToFile(const QUuid& uuid,
                                               const QUrl& path) = 0;

public slots:
    virtual bool refreshLastOpenedDateOfBook(const QUuid& uuid) = 0;
    virtual void updateUsedBookStorage(long usedStorage,
                                       long bookStorageLimit) = 0;
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;

signals:
    void bookCoverGenerated(int index);
    void bookInsertionStarted(int index);
    void bookInsertionEnded();
    void bookDeletionStarted(int index);
    void bookDeletionEnded();
    void bookClearingStarted();
    void bookClearingEnded();
    void tagsChanged(int index);
    void dataChanged(int index);
    void storageLimitExceeded();
};

}  // namespace application