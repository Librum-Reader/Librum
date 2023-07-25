#include "toc_item.hpp"
#include <QtAlgorithms>

namespace application::core
{

TOCItem::TOCItem(const TOCItemData& data, TOCItem* parentItem) :
    m_data(data),
    m_parentItem(parentItem)
{
}

TOCItem::~TOCItem()
{
    qDeleteAll(m_children);
}

void TOCItem::appendChild(TOCItem* child)
{
    m_children.append(child);
}

TOCItem* TOCItem::child(int row)
{
    if(row < 0 || row >= m_children.size())
        return nullptr;

    return m_children.at(row);
}

int TOCItem::childCount() const
{
    return m_children.count();
}

int TOCItem::columnCount() const
{
    return 0;
}

TOCItemData TOCItem::data() const
{
    return m_data;
}

int TOCItem::row() const
{
    if(m_parentItem != nullptr)
        return m_parentItem->m_children.indexOf(const_cast<TOCItem*>(this));

    return 0;
}

TOCItem* TOCItem::parentItem()
{
    return m_parentItem;
}

void TOCItem::setParent(TOCItem* parent)
{
    m_parentItem = parent;
}

const QList<TOCItem*>& TOCItem::getChildren() const
{
    return m_children;
}

}  // namespace application::core