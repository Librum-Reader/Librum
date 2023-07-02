#include "free_books_model.hpp"

using namespace domain::value_objects;

namespace adapters::data_models
{

FreeBooksModel::FreeBooksModel(
    const std::vector<domain::value_objects::FreeBook>& data) :
    m_data(data)
{
}

int FreeBooksModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data.size();
}

QVariant FreeBooksModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const auto& book = m_data.at(index.row());
    switch(role)
    {
    case IdRole:
        return QString::number(book.id);
        break;
    case TitleRole:
        return book.title;
        break;
    case AuthorsRole:
        return book.authors;
        break;
    case LanguagesRole:
        return book.languages;
        break;
    case MediaTypeRole:
        return book.mediaType;
        break;
    case FormatsRole:
        return book.formats;
        break;
    case DownloadCountRole:
        return QString::number(book.downloadCount);
        break;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FreeBooksModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { IdRole, "id" },
        { TitleRole, "title" },
        { AuthorsRole, "authors" },
        { LanguagesRole, "languages" },
        { MediaTypeRole, "mediaType" },
        { FormatsRole, "formats" },
        { DownloadCountRole, "downloadCount" },
    };

    return roles;
}

void FreeBooksModel::refreshBook(int row)
{
    auto allRoles = getAllRoles();

    emit dataChanged(index(row, 0), index(row, 0), allRoles);
}

void FreeBooksModel::startBookClearing()
{
    beginResetModel();
}

void FreeBooksModel::endBookClearing()
{
    endResetModel();
}

void FreeBooksModel::startInsertingRow(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void FreeBooksModel::endInsertingRow()
{
    endInsertRows();
}

void FreeBooksModel::startDeletingBook(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void FreeBooksModel::endDeletingBook()
{
    endRemoveRows();
}

QVector<int> FreeBooksModel::getAllRoles()
{
    QVector<int> allRoles;
    int lastRole = Invalid;
    for(int i = TitleRole; i < lastRole; ++i)
    {
        allRoles.push_back(i);
    }

    return allRoles;
}

}  // namespace adapters::data_models
