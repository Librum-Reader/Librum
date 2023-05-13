#include "shortcuts_model.hpp"

namespace adapters::data_models
{

ShortcutsModel::ShortcutsModel(const QQmlPropertyMap& data) :
    m_data(data)
{
}

int ShortcutsModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data.size();
}

QVariant ShortcutsModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    const auto shortcuts = m_data.keys();
    const QString& shortcut = shortcuts.at(index.row());
    const QString value = m_data[shortcut].toString();

    switch(role)
    {
    case ShorcutRole:
        return shortcut;
    case ValueRole:
        return value;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> ShortcutsModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { ShorcutRole, "shortcut" },
        { ValueRole, "value" },
    };

    return roles;
}

void ShortcutsModel::startInsertingRow(int row)
{
    beginInsertRows(QModelIndex(), row, row);
}

void ShortcutsModel::endInsertingRow()
{
    endInsertRows();
}

void ShortcutsModel::startDeletingRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
}

void ShortcutsModel::endDeletingRow()
{
    endRemoveRows();
}

void ShortcutsModel::refreshRow(QString key)
{
    int row = 0;
    for(const QString& item : m_data.keys())
    {
        if(item == key)
        {
            emit dataChanged(index(row, 0), index(row, 0),
                             { ShorcutRole, ValueRole });
            return;
        }

        ++row;
    }
}

}  // namespace adapters::data_models