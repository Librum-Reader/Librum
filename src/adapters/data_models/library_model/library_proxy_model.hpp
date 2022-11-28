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
    Q_PROPERTY(
        QString sortString READ getSortString WRITE setSortString CONSTANT)
    Q_PROPERTY(int sortRole READ getSortRole WRITE setSortRole CONSTANT)

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

    void setSortRole(int newRole);
    int getSortRole();

    void setSortString(QString newSortString);
    QString getSortString();

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;

signals:
    void sortStringUpdated();

private:
    double fuzzCompareWithSortingString(QString lhs) const;

    const QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
    QString m_sortString;
    SortRole m_sortRole;
};

}  // namespace adapters::data_models