#pragma once
#include <QList>
#include <QString>


namespace adapters::models
{

class ChapterTreeNode
{
public:
    ChapterTreeNode(const QString& data, ChapterTreeNode* parent = nullptr);
    ~ChapterTreeNode();
    
    void appendChild(ChapterTreeNode* child);
    ChapterTreeNode* child(int row) const;
    int childCount() const;
    int columnCount() const;
    QString data() const;
    int row();
    ChapterTreeNode* parentNode();
    
    
private:
    ChapterTreeNode* m_parentNode;
    QList<ChapterTreeNode*> m_childNodes;
    QString m_data;
};

} // namespace adapters::models