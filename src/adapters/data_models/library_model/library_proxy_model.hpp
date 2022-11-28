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
    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;

private:
    const QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
    SortRole m_sortRole;
};

}  // namespace adapters::data_models