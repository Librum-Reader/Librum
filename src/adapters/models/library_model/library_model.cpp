#include "library_model.hpp"


using namespace domain::models;

namespace adapters::models
{

LibraryModel::LibraryModel(const std::vector<domain::models::Book>& data)
    : m_data(data)
{
}

int LibraryModel::rowCount(const QModelIndex& parent) const
{
    if(!parent.isValid())
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

} // namespace adapters::models