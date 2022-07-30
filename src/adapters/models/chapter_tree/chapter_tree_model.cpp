#include <QVariant>
#include <QAbstractItemModel>
#include "chapter_tree_model.hpp"
#include "chapter_tree_node.hpp"


namespace adapters::models
{

ChapterTreeModel::ChapterTreeModel(QObject* parent)
    : QAbstractItemModel(parent), m_rootNode(new ChapterTreeNode("Title"))
{
    // Test data
    auto firstLevel1 = new ChapterTreeNode("Table of Contents", m_rootNode);
    auto firstLevel2 = new ChapterTreeNode("From the Publisher", m_rootNode);
    auto firstLevel3 = new ChapterTreeNode("Introduction", m_rootNode);
    auto firstLevel4 = new ChapterTreeNode("Introduction", m_rootNode);
    auto firstLevel0 = new ChapterTreeNode("Introduction", m_rootNode);
    
    auto secondLevel1 = new ChapterTreeNode("Using Code Examples", firstLevel2);
    auto secondLevel2 = new ChapterTreeNode("@Safari Book Online", firstLevel2);
    auto secondLevel3 = new ChapterTreeNode("How to Contact Us", firstLevel2);
    
    firstLevel2->appendChild(secondLevel1);
    firstLevel2->appendChild(secondLevel2);
    firstLevel2->appendChild(secondLevel3);
    
    auto secondLevel4 = new ChapterTreeNode("Custom one", firstLevel3);
    auto secondLevel5 = new ChapterTreeNode("Another one", firstLevel3);
    auto secondLevel6 = new ChapterTreeNode("Something else", firstLevel3);
    
    firstLevel3->appendChild(secondLevel4);
    firstLevel3->appendChild(secondLevel5);
    firstLevel3->appendChild(secondLevel6);
    
    auto thirdLevel1 = new ChapterTreeNode("Third level", secondLevel3);
    auto thirdLevel2 = new ChapterTreeNode("Even a 3rd", secondLevel3);
    auto thirdLevel3 = new ChapterTreeNode("Another text", secondLevel3);
    
    secondLevel3->appendChild(thirdLevel1);
    secondLevel3->appendChild(thirdLevel2);
    secondLevel3->appendChild(thirdLevel3);
    
    
    m_rootNode->appendChild(firstLevel1);
    m_rootNode->appendChild(firstLevel2);
    m_rootNode->appendChild(firstLevel0);
    m_rootNode->appendChild(firstLevel3);
    m_rootNode->appendChild(firstLevel4);
    
    
    for(int i = 0; i < 40; ++i)
    {
        QString value = "Filler";
        value.append(QString::number(i));
        m_rootNode->appendChild(new ChapterTreeNode(value, m_rootNode));
    }
}

ChapterTreeModel::~ChapterTreeModel()
{
    delete m_rootNode;
}


QModelIndex ChapterTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();
    
    ChapterTreeNode* parentNode;
    if(!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<ChapterTreeNode*>(parent.internalPointer());
    
    auto childNode = parentNode->child(row);
    if(childNode)
        return createIndex(row, column, childNode);
    
    return QModelIndex();
}

QModelIndex ChapterTreeModel::parent(const QModelIndex& child) const
{
    if(!child.isValid())
        return QModelIndex();
    
    auto childNode = static_cast<ChapterTreeNode*>(child.internalPointer());
    auto parentNode = childNode->parentNode();
    
    if(parentNode == m_rootNode)
        return QModelIndex();
    
    return createIndex(parentNode->row(), 0, parentNode);
}

int ChapterTreeModel::rowCount(const QModelIndex& parent) const
{
    if(parent.column() > 0)
        return 0;
    
    ChapterTreeNode* parentNode;
    if(!parent.isValid())
        parentNode = m_rootNode;
    else
        parentNode = static_cast<ChapterTreeNode*>(parent.internalPointer());
    
    return parentNode->childCount();
}

int ChapterTreeModel::columnCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return static_cast<ChapterTreeNode*>(parent.internalPointer())->columnCount();
    
    return m_rootNode->columnCount();
}

QVariant ChapterTreeModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();
    
    if(role != Qt::DisplayRole)
        return QVariant();
    
    auto node = static_cast<ChapterTreeNode*>(index.internalPointer());
    return node->data();
}

Qt::ItemFlags ChapterTreeModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;
    
    return QAbstractItemModel::flags(index);
}

QVariant ChapterTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_rootNode->data();
    
    return QVariant();
}

} // namespace adapters::models