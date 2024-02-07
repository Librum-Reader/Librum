#pragma once
#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>
#include <rapidfuzz/fuzz.hpp>
#include "adapters_export.hpp"

namespace adapters::data_models
{

class ADAPTERS_EXPORT IconProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString sortString READ getSortString WRITE setSortString NOTIFY
                   sortStringUpdated)

public:
    IconProxyModel();

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;

    QString getSortString() const;
    void setSortString(const QString& newSortString);

signals:
    void sortStringUpdated();

private:
    QString m_sortString;
};

}  // namespace adapters::data_models