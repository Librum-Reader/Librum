#include "free_books_model.hpp"
#include <QBuffer>
#include <QDebug>

using namespace domain::value_objects;

namespace adapters::data_models
{

FreeBooksModel::FreeBooksModel(const std::vector<FreeBook>* data) :
    m_data(data)
{
}

int FreeBooksModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data->size();
}

QVariant FreeBooksModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const auto& freeBook = m_data->at(index.row());
    switch(role)
    {
    case IdRole:
        return QVariant::fromValue(freeBook.id);
    case TitleRole:
        return freeBook.title;
    case AuthorsRole:
        return freeBook.authors;
    case LanguagesRole:
        return freeBook.languages;
    case FormatsRole:
        return freeBook.formats;
    case DownloadCountRole:
        return QVariant::fromValue(freeBook.downloadCount);
    case CoverRole:
    {
        auto& cover = freeBook.cover;
        if(cover.isNull())
            return "";

        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        cover.save(&buffer, "jpeg");
        QString base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/jpeg;base64,") + base64;
    }
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FreeBooksModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { IdRole, "id" },           { TitleRole, "title" },
        { AuthorsRole, "authors" }, { LanguagesRole, "languages" },
        { FormatsRole, "formats" }, { DownloadCountRole, "downloadCount" },
        { CoverRole, "cover" },
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
