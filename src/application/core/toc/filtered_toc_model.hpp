#pragma once
#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>
#include "toc_item.hpp"
#include "toc_model.hpp"
#include "application_export.hpp"

namespace application::core
{

class APPLICATION_EXPORT FilteredTOCModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ getFilterString WRITE setFilterString
                   NOTIFY filterStringUpdated)

public:
    explicit FilteredTOCModel(QObject* parent = nullptr);

    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
    void setFilterString(QString filterString);
    QString getFilterString();

signals:
    void filterStringUpdated();

private:
    // Recursively check if the item or any of its children match the filter
    bool hasChildrenMatchingTheFilter(const TOCItem* item) const;
    bool itemPassesFilter(const TOCItem* item) const;

    QString m_filterString;
};

}  // namespace application::core
