#pragma once
#include <QSortFilterProxyModel>
#include "tocmodel.h"
#include <QString>
#include <QObject>

namespace Okular
{

class Q_DECL_EXPORT FilteredTOCModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ getFilterString WRITE setFilterString
                   NOTIFY filterStringUpdated)

public:
    explicit FilteredTOCModel(QObject* parent = nullptr);
    
    bool filterAcceptsRow(int row,
                          const QModelIndex& parent) const override;
    void setFilterString(QString filterString);
    QString getFilterString();
    
Q_SIGNALS:
    void filterStringUpdated();
    
private:
    // Recursively check if the item or any of its children match the filter
    bool hasChildrenMatchingTheFilter(const TOCItem* item) const;
    bool itemPassesFilter(const TOCItem* item) const;

    QString m_filterString;
};

} // namespace Okular