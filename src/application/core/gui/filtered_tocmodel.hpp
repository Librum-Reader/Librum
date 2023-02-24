#pragma once
#include <QSortFilterProxyModel>
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
    
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;
    void setFilterString(QString filterString);
    QString getFilterString();
    
Q_SIGNALS:
    void filterStringUpdated();
    
private:
    double fuzzCompareWithFilterString(QString str) const;

    QString m_filterString;
};

} // namespace Okular