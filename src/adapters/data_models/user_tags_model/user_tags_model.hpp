#include <QAbstractListModel>
#include <vector>
#include "tag.hpp"
#pragma once

namespace adapters::data_models
{

/**
 * The UserTagsModel represents the user's tags to the UI
 */
class UserTagsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        NameRole = Qt::UserRole,
    };

    UserTagsModel(const std::vector<domain::models::Tag>& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingRow(int row);
    void endInsertingRow();
    void startDeletingRow(int row);
    void endDeletingRow();
    void refreshRows(int row);

private:
    const std::vector<domain::models::Tag>& m_data;
};

}  // namespace adapters::data_models