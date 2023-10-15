#pragma once
#include <QSortFilterProxyModel>
#include <optional>
#include <vector>
#include "adapters_export.hpp"

namespace adapters::data_models
{

/**
 * The BookmarksProxyModel class is a proxy class which acts as a wrapper around
 * the Bookmarks model that is handling the sorting and filtering of elements.
 */
class ADAPTERS_EXPORT BookmarksProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ getFilterString WRITE setFilterString
                   NOTIFY filterStringUpdated)

public:
    explicit BookmarksProxyModel(QObject* parent = nullptr);

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;

    void setFilterString(QString newFilterString);
    QString getFilterString();

signals:
    void filterStringUpdated();

private:
    QString m_filterString;
};

}  // namespace adapters::data_models
