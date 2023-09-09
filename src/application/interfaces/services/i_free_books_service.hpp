#pragma once
#include <QObject>
#include <vector>
#include "free_book.hpp"

namespace application
{

class IFreeBooksService : public QObject
{
    Q_OBJECT

public:
    virtual ~IFreeBooksService() noexcept = default;

    virtual void fetchFirstBooksMetadataPageWithFilter(
        const QString& author, const QString& title) = 0;
    virtual void getBookMedia(const int id, const QString& url) = 0;
    virtual void getBookCover(const int id) = 0;
    virtual void deleteBookCover(const int id) = 0;
    virtual const std::vector<domain::value_objects::FreeBook>&
        getFreeBooks() = 0;

public slots:
    virtual void fetchBooksMetadataPage(const QString& url) = 0;
    virtual void setupUserData(const QString& token, const QString& email) = 0;
    virtual void clearUserData() = 0;

signals:
    void fetchingFirstMetadataPageSuccessful(const bool success);
    void apiInfoReady(const int booksTotalCount,
                      const QString& nextMetadataPageUrl,
                      const QString& prevMetadataPageUrl);
    void gettingBookFinished(const QString& filePath);
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
};

}  // namespace application
