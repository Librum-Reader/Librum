#include "folders_model.hpp"

namespace adapters::data_models
{

using domain::entities::Folder;

FoldersModel::FoldersModel(Folder* root, QObject* parent) :
    QAbstractItemModel(parent),
    m_root(root)
{
}

QVariant FoldersModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    auto item = static_cast<Folder*>(index.internalPointer());
    switch(role)
    {
    case NameRole:
        return item->getName();
    case UuidRole:
        return item->getUuid().toString(QUuid::WithoutBraces);
    case ColorRole:
        return item->getColor();
    case IconRole:
        return item->getIcon();
    case DescriptionRole:
        return item->getDescription();
    }

    return QVariant();
}

Qt::ItemFlags FoldersModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QModelIndex FoldersModel::index(int row, int column,
                                const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    Folder* parentItem;
    if(!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<Folder*>(parent.internalPointer());

    auto thisItem = parentItem->getChildAtIndex(row);
    if(thisItem != nullptr)
        return createIndex(row, column, thisItem);
    return QModelIndex();
}

QModelIndex FoldersModel::parent(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    auto thisItem = static_cast<Folder*>(index.internalPointer());
    auto parentItem = thisItem->getParent();

    if(parentItem == m_root)
        return QModelIndex();

    return createIndex(parentItem->getIndexInParent(), 0, parentItem);
}

int FoldersModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    Folder* parentItem;
    if(!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<Folder*>(parent.internalPointer());

    return parentItem->childCount();
}

QHash<int, QByteArray> FoldersModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { NameRole, "name" },
        { UuidRole, "uuid" },
        { ColorRole, "color" },
        { IconRole, "icon" },
        { DescriptionRole, "description" },
    };

    return roles;
}

int FoldersModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return roleNames().count();
}

void FoldersModel::beginInsertFolder(Folder* parent, int row)
{
    auto parentIndex = createModelIndexFromFolder(parent);
    beginInsertRows(parentIndex, row, row);
}

void FoldersModel::endInsertFolder()
{
    endInsertRows();
}

void FoldersModel::beginRemoveFolder(Folder* parent, int row)
{
    auto parentIndex = createModelIndexFromFolder(parent);
    beginRemoveRows(parentIndex, row, row);
}

void FoldersModel::endRemoveFolder()
{
    endRemoveRows();
}

void FoldersModel::refreshFolder(Folder* parent, int row)
{
    auto parentIndex = createModelIndexFromFolder(parent);

    auto thisIndex = index(row, 0, parentIndex);
    emit dataChanged(thisIndex, thisIndex);
}

void FoldersModel::beginModelReset()
{
    emit beginResetModel();
}

void FoldersModel::endModelReset()
{
    emit endResetModel();
}

QModelIndex FoldersModel::createModelIndexFromFolder(Folder* folder)
{
    // This occurs when the this item is the root item
    if(folder == nullptr || folder->getParent() == nullptr)
        return QModelIndex();

    return createIndex(folder->getIndexInParent(), 0, folder);
}

}  // namespace adapters::data_models