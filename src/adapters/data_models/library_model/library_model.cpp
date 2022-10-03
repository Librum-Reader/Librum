#include "library_model.hpp"
#include <QDebug>
#include <QByteArray>
#include <QBuffer>
#include <strings.h>
#include "book.hpp"


using namespace domain::models;

namespace adapters::data_models
{


LibraryModel::LibraryModel(const std::vector<domain::models::Book>& data)
    : m_data(data)
{
}


int LibraryModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;
    
    return m_data.size();
}

QVariant LibraryModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();
    
    const Book& book = m_data.at(index.row());
    
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    QString base64;
    
    switch(role)
    {
    case TitleRole:
        return book.title();
        break;
    case AuthorRole:
        return book.author();
        break;
    case FilePathRole:
        return book.filePath();
        break;
    case CoverRole:
        buffer.open(QIODevice::WriteOnly);
        book.cover().save(&buffer, "png");
        base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/png;base64,") + base64;
        break;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> LibraryModel::roleNames() const
{
    static QHash<int, QByteArray> roles
    {
        {TitleRole, "title"},
        {AuthorRole, "author"},
        {FilePathRole, "filePath"},
        {CoverRole, "cover"}
    };
    
    return roles;
}

void LibraryModel::processBookCover(int row)
{
    auto modelIndex = index(row, 0);
    
    emit dataChanged(modelIndex, modelIndex, {CoverRole});
}


void LibraryModel::beginInsertingRow(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void LibraryModel::endInsertingRow()
{
    endInsertRows();
}

} // namespace adapters::data_models