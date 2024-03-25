#include "book_title_model.hpp"
#include <iostream>

namespace adapters::data_models
{

using domain::entities::Book;

BookTitleModel::BookTitleModel(
    const std::vector<domain::entities::Book>& data) :
    m_data(data)
{
}

int BookTitleModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data.size();
}

QVariant BookTitleModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const Book& book = m_data.at(index.row());
    switch(role)
    {
    case UuidRole:
        return book.getUuid().toString(QUuid::WithoutBraces);
        break;
    case TitleRole:
        return book.getTitle();
        break;
    case DownloadedRole:
        return book.isDownloaded();
        break;
    case FilePathRole:
        return book.getFilePath();
        break;
    case ExtensionRole:
        return book.getExtension();
        break;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BookTitleModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { UuidRole, "uuid" },
        { TitleRole, "title" },
        { DownloadedRole, "downloaded" },
        { FilePathRole, "filePath" },
        { ExtensionRole, "extension" },
    };

    return roles;
}

void BookTitleModel::startInsertingRow(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void BookTitleModel::endInsertingRow()
{
    endInsertRows();
}

void BookTitleModel::startDeletingBook(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void BookTitleModel::endDeletingBook()
{
    endRemoveRows();
}

void BookTitleModel::refreshBook(int row)
{
    emit dataChanged(index(row, 0), index(row, 0));
}

}  // namespace adapters::data_models