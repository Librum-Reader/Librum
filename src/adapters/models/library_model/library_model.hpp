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
        CoverRole,
        FilePathRole
    };
    
    LibraryModel(const std::vector<domain::models::Book>& data);
    
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
public slots:
    void beginInsertingRow(int index);
    void endInsertingRow();
    
private:
    const std::vector<domain::models::Book>& m_data;
};

} // namespace adapters::data_models