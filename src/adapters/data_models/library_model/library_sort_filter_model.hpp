#pragma once
#include <QSortFilterProxyModel>

namespace adapters::data_models
{

class LibrarySortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum SortRole
    {
        None = Qt::UserRole + 1,
        Title,
        LastOpened,
        RecentlyAdded
    };

    Q_ENUM(SortRole);

    explicit LibrarySortFilterModel(QObject* parent = nullptr);

    Q_INVOKABLE void setSortRole(int newRole);

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;

private:
    const QString m_dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";
    SortRole m_sortRole;
};

}  // namespace adapters::data_models