#include "library_model.hpp"
#include "book.hpp"
#include <QDebug>


using namespace domain::models;

namespace adapters::data_models
{


LibraryModel::LibraryModel(const std::vector<domain::models::Book>& data)
    : m_data(data)
{
//    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
//    m_data.emplace_back("SomeTitle", "Some/Path.qml", "SomeCover");
//    endInsertRows();
//    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
//    m_data.emplace_back("SomeOtherTitle", "Some/Path.qml", "SomeCover");
//    endInsertRows();
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
    if(role == TitleRole)
        return book.title();
    else if(role == CoverRole)
        return book.cover();
    else if(role == FilePathRole)
        return book.filePath();
    else
        return QVariant();
}

QHash<int, QByteArray> LibraryModel::roleNames() const
{
    return QHash<int, QByteArray>
    {
        {TitleRole, "title"},
        {CoverRole, "cover"},
        {FilePathRole, "filePath"}
    };
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