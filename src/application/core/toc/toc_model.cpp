#include "toc/toc_model.hpp"
#include <QDebug>
#include <QStack>

namespace application::core
{

TOCModel::TOCModel(fz_outline* outline, mupdf::FzDocument& document,
                   QObject* parent) :
    QAbstractItemModel(parent),
    m_document(document)
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
        return item->data().title.isEmpty() ? "-" : item->data().title;
    case PageNumberRole:
        return item->data().pageNumber;
    case YOffsetRole:
        return item->data().yOffset;
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
        { YOffsetRole, "yOffset" },
    };

    return roles;
}

int TOCModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)

    return roleNames().count();
}

void TOCModel::setupModelData(fz_outline* outline)
{
    if(outline == nullptr)
        return;

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
    auto location = outline->page;
    float yOffset = outline->y;

    // Outlines come with the page number set to -1 if they are reflowable
    // documents (e.g. epub), because its quite expensive to get absolute page
    // numbers for them. Since we need absolute page numbers, we need to resolve
    // their links manually and then convert the location to page numbers.
    if(location.page == -1)
    {
        location = *m_document.fz_resolve_link(outline->uri, nullptr, &yOffset)
                        .internal();
    }

    auto pageNumber = m_document.fz_page_number_from_location(location);

    TOCItemData data {
        .title = QString(outline->title),
        .pageNumber = pageNumber,
        .yOffset = yOffset,
        .internal = outline,
    };

    return new TOCItem(data);
}

}  // namespace application::core