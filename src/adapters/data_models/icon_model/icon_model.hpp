#pragma once
#include <QAbstractListModel>
#include <QObject>
#include "adapters_export.hpp"
#include "icon_item.hpp"

namespace adapters::data_models
{

class ADAPTERS_EXPORT IconModel : public QAbstractListModel
{
    Q_OBJECT

public:
    IconModel();

    enum Roles
    {
        NameRole = Qt::DisplayRole,
        KeywordsRole
    };

    QVariant data(const QModelIndex& index, int role) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    const QString m_jsonFilePath = ":/resources/data/folder_icons.json";
    QList<IconItem> m_data;
};

}  // namespace adapters::data_models