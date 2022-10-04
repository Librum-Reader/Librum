#pragma once
#include <vector>
#include <QAbstractListModel>
#include <QByteArray>
#include <QVariant>
#include "book.hpp"


namespace adapters::data_models
{

class LibraryModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    enum Roles
    {
        TitleRole = Qt::UserRole,
        AuthorRole,
        FilePathRole,
        CreatorRole,
        CreationDateRole,
        FormatRole,
        DocumentSizeRole,
        PagesSizeRole,
        PageCountRole,
        AddedToLibraryRole,
        LastModifiedRole,
        CoverRole,
        TagsRole
    };
    
    LibraryModel(const std::vector<domain::models::Book>& data);
    
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
public slots:
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();
    void processBookCover(int index);
    void refreshTags(int row);
    
private:
    const std::vector<domain::models::Book>& m_data;
};

} // namespace adapters::data_models