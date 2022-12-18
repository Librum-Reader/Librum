#include "user_tags_model.hpp"

using domain::models::Tag;

namespace adapters::data_models
{

UserTagsModel::UserTagsModel(const std::vector<domain::models::Tag>& data) :
    m_data(data)
{
}

int UserTagsModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data.size();
}

QVariant UserTagsModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const Tag& tag = m_data.at(index.row());
    switch(role)
    {
    case NameRole:
        return tag.getName();
        break;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> UserTagsModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { NameRole, "name" },
    };

    return roles;
}

void UserTagsModel::startInsertingRow(int row)
{
    beginInsertRows(QModelIndex(), row, row);
}

void UserTagsModel::endInsertingRow()
{
    endInsertRows();
}

void UserTagsModel::startDeletingRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
}

void UserTagsModel::endDeletingRow()
{
    endRemoveRows();
}

void UserTagsModel::refreshRows(int row)
{
    emit dataChanged(index(row, 0), index(row, 0), { NameRole });
}

}  // namespace adapters::data_models