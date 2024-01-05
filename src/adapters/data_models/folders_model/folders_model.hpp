#pragma once
#include <QAbstractItemModel>
#include <QString>
#include "adapters_export.hpp"
#include "folder.hpp"

namespace adapters::data_models
{

class ADAPTERS_EXPORT FoldersModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FoldersModel(domain::entities::Folder* root,
                          QObject* parent = nullptr);

    enum Roles
    {
        NameRole = Qt::DisplayRole,
        UuidRole,
        ChildCountRole,
        Invalid
    };

    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    int columnCount(const QModelIndex& parent) const override;

public slots:
    void beginInsertFolder(domain::entities::Folder* parent, int row);
    void endInsertFolder();
    void beginRemoveFolder(domain::entities::Folder* parent, int row);
    void endRemoveFolder();
    void refreshFolder(domain::entities::Folder* parent, int row);

private:
    QList<int> getAllRoles();
    QModelIndex createModelIndexFromFolder(domain::entities::Folder* folder);

    domain::entities::Folder* m_root;
};

}  // namespace adapters::data_models