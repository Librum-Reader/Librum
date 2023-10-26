#pragma once
#include <QObject>
#include <set>
#include <vector>
#include "application_export.hpp"
#include "free_book.hpp"

namespace application
{

class APPLICATION_EXPORT IFreeBooksService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksService() noexcept = default;

    virtual void fetchFirstBooksMetadataPageWithFilter(
        const QString& authorsAndTitle) = 0;
    virtual void getBookMedia(const int id, const QString& url) = 0;
    virtual void getBookCover(const int id) = 0;
    virtual void deleteBookCover(const int id) = 0;
    virtual const std::vector<domain::value_objects::FreeBook>&
        getFreeBooks() = 0;
    virtual void deleteAllBooks() = 0;
    virtual bool isBookDownloaded(int id) = 0;
    virtual void proccessDownloadedIds(const std::set<int>& newIds) = 0;
    virtual void markBookAsDownloaded(int id) = 0;
    virtual void unmarkBookAsDownloaded(int id) = 0;

public slots:
    virtual void fetchBooksMetadataPage(const QString& url) = 0;
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;

signals:
    void fetchingFirstMetadataPageSuccessful(const bool success);
    void receivedNoMetadata();
    void apiInfoReady(const int booksTotalCount,
                      const QString& nextMetadataPageUrl,
                      const QString& prevMetadataPageUrl);
    void gettingBookFinished(const QString& filePath, int id);
    void bookCoverDownloadFinished();

    void bookInsertionStarted(int index);
    void bookInsertionEnded();
    void bookDeletionStarted(int index);
    void bookDeletionEnded();
    void bookClearingStarted();
    void bookClearingEnded();
    void tagsChanged(int index);
    void dataChanged(int index);
    void downloadingBookMediaProgressChanged(int index);
    void bookIsDownloadedChanged(int row);
};

}  // namespace application
