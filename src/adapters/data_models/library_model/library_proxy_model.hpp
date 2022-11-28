#pragma once
#include <QSortFilterProxyModel>

namespace adapters::data_models
{

/**
 * The LibraryProxyModel class is a proxy class which acts as a wrapper around
 * the library model that is handling the sorting and filtering of elements
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
        RecentlyAdded = Qt::UserRole + 1,
        Title,
        Authors,
        LastOpened
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
    bool stringIsLexicographicallyLess(const QString& left,
                                       const QString& right) const;
    bool openedAfter(const QModelIndex& left, const QModelIndex& right) const;
    bool addedToLibraryAfter(const QModelIndex& left,
                             const QModelIndex& right) const;

    const QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
    QString m_sortString;
    SortRole m_sortRole = SortRole::RecentlyAdded;
};

}  // namespace adapters::data_models