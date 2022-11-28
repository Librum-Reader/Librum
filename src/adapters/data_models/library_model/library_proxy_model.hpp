#pragma once
#include <QSortFilterProxyModel>

namespace adapters::data_models
{

/**
 * The LibraryProxyModel class is a proxy class on the library model that is
 * handling the sorting and filtering of elements
 */
class LibraryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum SortRole
    {
        None = Qt::UserRole + 1,
        Title,
        Authors,
        LastOpened,
        RecentlyAdded
    };

    Q_ENUM(SortRole);

    explicit LibraryProxyModel(QObject* parent = nullptr);

    Q_INVOKABLE void setSortRole(int newRole);
    Q_INVOKABLE void setSortString(QString newSortString);
    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;

private:
    double fuzzCompareWithSortingString(QString lhs) const;

    const QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
    QString m_sortString;
    SortRole m_sortRole;
};

}  // namespace adapters::data_models