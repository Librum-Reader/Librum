#include "toc_model.hpp"
#include <QStack>
#include "toc/toc_item.hpp"

namespace application::core
{

TOCModel::TOCModel(fz_outline* outline, QObject* parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new TOCItem(TOCItemData());

    setupModelData(outline);
}

TOCModel::~TOCModel()
{
    delete m_rootItem;
}

QVariant TOCModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    auto item = static_cast<TOCItem*>(index.internalPointer());
    switch(role)
    {
    case TitleRole:
        return item->data().title;
        break;
    case PageNumberRole:
        return item->data().pageNumber;
        break;
    }

    return QVariant();
}

Qt::ItemFlags TOCModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QModelIndex TOCModel::index(int row, int column,
                            const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    TOCItem* parentItem;
    if(!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TOCItem*>(parent.internalPointer());

    auto childItem = parentItem->child(row);
    if(childItem != nullptr)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TOCModel::parent(const QModelIndex& index) const
{
    if(!index.isValid())
        return QModelIndex();

    auto childItem = static_cast<TOCItem*>(index.internalPointer());
    auto parentItem = childItem->parentItem();

    if(parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TOCModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;

    TOCItem* parentItem;
    if(!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<TOCItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QHash<int, QByteArray> TOCModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { TitleRole, "title" },
        { PageNumberRole, "pageNumber" },
    };

    return roles;
}

int TOCModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

void TOCModel::setupModelData(fz_outline* outline)
{
    // No chapter exists
    //    if(outline->page.page == -1)
    //        return;

    QStack<TOCItem*> stack;
    stack.push(m_rootItem);

    auto curr = getTOCItemFromOutline(outline);
    while(true)
    {
        curr->setParent(stack.top());
        curr->parentItem()->appendChild(curr);

        while(outline->down != nullptr)
        {
            stack.push(curr);

            auto child = getTOCItemFromOutline(outline->down);

            curr->appendChild(child);
            child->setParent(curr);

            curr = child;
            outline = outline->down;
        }

        while(outline->next == nullptr)
        {
            if(stack.top() == m_rootItem)
                return;

            curr = stack.pop();
            outline = curr->data().internal;
        }

        curr = getTOCItemFromOutline(outline->next);
        outline = outline->next;
    }
}

TOCItem* TOCModel::getTOCItemFromOutline(fz_outline* outline)
{
    TOCItemData data {
        .title = QString(outline->title),
        .pageNumber = outline->page.page,
        .internal = outline,
    };

    return new TOCItem(data);
}

}  // namespace application::core