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

    virtual void getBooksMetadata(const QString& author,
                                  const QString& title) = 0;
    virtual void getBookMedia(const QString& url) = 0;
    virtual const std::vector<domain::value_objects::FreeBook>&
        getFreeBooks() = 0;

signals:
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
};

}  // namespace application
