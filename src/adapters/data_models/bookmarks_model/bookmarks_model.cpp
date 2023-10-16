#include "bookmarks_model.hpp"

using domain::entities::Bookmark;

namespace adapters::data_models
{

BookmarksModel::BookmarksModel(const QUuid bookUuid,
                               const application::IBookService* bookService) :
    m_bookService(bookService),
    m_bookUuid(bookUuid)
{
}

int BookmarksModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_bookService->getBookmarks().size();
}

QVariant BookmarksModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    auto bookmarks = m_bookService->getBookmarks();
    const Bookmark& bookmark = bookmarks.at(index.row());
    switch(role)
    {
    case UuidRole:
        return bookmark.getUuid().toString(QUuid::WithoutBraces);
    case NameRole:
        return bookmark.getName();
    case PageNumberRole:
        return bookmark.getPageNumber();
    case YOffsetRole:
        return bookmark.getYOffset();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BookmarksModel::roleNames() const
{
    static QHash<int, QByteArray> roles {

        { UuidRole, "uuid" },
        { NameRole, "name" },
        { PageNumberRole, "pageNumber" },
        { YOffsetRole, "yOffset" },
    };

    return roles;
}

void BookmarksModel::startInsertingBookmark(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void BookmarksModel::endInsertingBookmark()
{
    endInsertRows();
}

void BookmarksModel::startDeletingBookmark(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void BookmarksModel::endDeletingBookmark()
{
    endRemoveRows();
}

void BookmarksModel::bookmarkNameChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0), { NameRole });
}

}  // namespace adapters::data_models