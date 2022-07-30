#include <QtAlgorithms>
#include "chapter_tree_node.hpp"


namespace adapters::models
{

ChapterTreeNode::ChapterTreeNode(const QString& data, ChapterTreeNode* parent)
    : m_parentNode(parent), m_data(data)
{
}

ChapterTreeNode::~ChapterTreeNode()
{
    qDeleteAll(m_childNodes);
}


void ChapterTreeNode::appendChild(ChapterTreeNode* child)
{
    m_childNodes.append(child);
}

ChapterTreeNode* ChapterTreeNode::child(int row) const
{
    if(row < 0 || row >= m_childNodes.size())
        return nullptr;
    
    return m_childNodes.at(row);
}

int ChapterTreeNode::childCount() const
{
    return m_childNodes.size();
}

int ChapterTreeNode::columnCount() const
{
    return 1;
}

QString ChapterTreeNode::data() const
{
    return m_data;
}

int ChapterTreeNode::row() const
{
    if(m_parentNode)
        return m_parentNode->m_childNodes.indexOf(this);
    
    return 0;
}

ChapterTreeNode* ChapterTreeNode::parentNode()
{
    return m_parentNode;
}

} // namespace adapters::models