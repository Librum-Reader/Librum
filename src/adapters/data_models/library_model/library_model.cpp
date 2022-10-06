#include "library_model.hpp"
#include <QByteArray>
#include <QBuffer>
#include <QString>
#include <QList>
#include "book.hpp"
#include "tag_dto.hpp"


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
    
    QList<dtos::TagDto> tagDtos;
    
    switch(role)
    {
    case TitleRole:
        return book.getTitle();
        break;
    case AuthorRole:
        if(book.getAuthor() == "")
            return "Unknown Author";
        return book.getAuthor();
        break;
    case FilePathRole:
        return book.getFilePath();
        break;
    case CreatorRole:
        return book.getCreator();
        break;
    case CreationDateRole:
        return book.getCreationDate();
        break;
    case FormatRole:
        return book.getFormat();
        break;
    case DocumentSizeRole:
        return book.getDocumentSize();
        break;
    case PagesSizeRole:
        return book.getPagesSize();
        break;
    case PageCountRole:
        return book.getPageCount();
        break;
    case AddedToLibraryRole:
        return book.getAddedToLibrary();
        break;
    case LastOpenedRole:
        return book.getLastOpened();
        break;
    case CoverRole:
        buffer.open(QIODevice::WriteOnly);
        book.getCover().save(&buffer, "png");
        base64 = QString::fromUtf8(byteArray.toBase64());
        return QString("data:image/png;base64,") + base64;
        break;
    case TagsRole:
        for(const auto& tag : book.getTags())
            tagDtos.push_back(dtos::TagDto{ .name = tag.getName() });
        
        return QVariant::fromValue(tagDtos);
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
        {CreatorRole, "creator"},
        {CreationDateRole, "creationDate"},
        {FormatRole, "format"},
        {DocumentSizeRole, "documentSize"},
        {PagesSizeRole, "pagesSize"},
        {PageCountRole, "pageCount"},
        {AddedToLibraryRole, "addedToLibrary"},
        {LastOpenedRole, "lastOpened"},
        {CoverRole, "cover"},
        {TagsRole, "tags"}
    };
    
    return roles;
}

void LibraryModel::processBookCover(int row)
{
    auto modelIndex = index(row, 0);
    
    emit dataChanged(modelIndex, modelIndex, {CoverRole});
}

void LibraryModel::refreshTags(int row)
{
    emit dataChanged(index(row, 0), index(row, 0), {TagsRole});
}

void LibraryModel::refreshBook(int row)
{
    QVector<int> allRoles;
    int lastRole = Invalid;
    for(int i = TitleRole; i < lastRole; ++i)
    {
        allRoles.push_back(i);
    }
    
    emit dataChanged(index(row, 0), index(row, 0), allRoles);
}

void LibraryModel::startInsertingRow(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void LibraryModel::endInsertingRow()
{
    endInsertRows();
}

void LibraryModel::startDeletingBook(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
}

void LibraryModel::endDeletingBook()
{
    endRemoveRows();
}

} // namespace adapters::data_models