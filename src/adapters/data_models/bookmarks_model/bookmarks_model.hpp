#pragma once
#include <QAbstractListModel>
#include <QList>
#include <bookmark.hpp>
#include "adapters_export.hpp"
#include "i_book_service.hpp"

namespace adapters::data_models
{

/**
 * The BookmarksModel represents the Bookmarks to the UI
 */
class ADAPTERS_EXPORT BookmarksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        UuidRole = Qt::UserRole,
        NameRole,
        PageNumberRole,
        YOffsetRole
    };

    BookmarksModel(const QUuid bookUuid,
                   const application::IBookService* bookService);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingBookmark(int index);
    void endInsertingBookmark();
    void startDeletingBookmark(int index);
    void endDeletingBookmark();
    void bookmarkNameChanged(int row);

private:
    // We can not keep a reference to the book services books, since they
    // are inside of a vector which can be relocated and thus lead to
    // invalid pointers. Instead we keep a reference to the book service.
    const application::IBookService* m_bookService;
    QUuid m_bookUuid;
};

}  // namespace adapters::data_models