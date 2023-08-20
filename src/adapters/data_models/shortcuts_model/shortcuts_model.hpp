#pragma once
#include <QAbstractListModel>
#include <QPair>
#include <QQmlPropertyMap>
#include "adapters_export.hpp"

namespace adapters::data_models
{

/**
 * The ShortcutsModel represents the shortcuts to the user
 */
class ADAPTERS_EXPORT ShortcutsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        ShortcutRole = Qt::UserRole,
        ValueRole
    };

    ShortcutsModel(const QQmlPropertyMap& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void refreshRow(QString key);

private:
    const QQmlPropertyMap& m_data;
};

}  // namespace adapters::data_models
