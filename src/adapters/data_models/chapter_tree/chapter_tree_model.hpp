#pragma once
#include <qnamespace.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <memory>
#include "chapter_tree_node.hpp"

namespace adapters::data_models
{

class ChapterTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ChapterTreeModel(QObject* parent = nullptr);
    ~ChapterTreeModel();

    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;


private:
    ChapterTreeNode* m_rootNode;
};

}  // namespace adapters::data_models