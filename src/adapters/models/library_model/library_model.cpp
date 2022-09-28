#include "library_model.hpp"
#include <QDebug>
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
    
    
    switch(role)
    {
    case TitleRole:
        return book.title();
        break;
    case CoverRole:
        return book.cover();
        break;
    case FilePathRole:
        return book.filePath();
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
        {CoverRole, "cover"},
        {FilePathRole, "filePath"}
    };
    
    return roles;
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