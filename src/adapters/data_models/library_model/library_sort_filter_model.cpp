#include "library_sort_filter_model.hpp"
#include <QDebug>
#include "library_model.hpp"

namespace adapters::data_models
{

LibrarySortFilterModel::LibrarySortFilterModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
}

void LibrarySortFilterModel::setSortRole(int newRole)
{
    int firstRole = SortRole::None;
    int lastRole = SortRole::RecentlyAdded;
    bool inRange = firstRole <= newRole <= lastRole;

    if(!inRange)
        return;

    m_sortRole = static_cast<SortRole>(newRole);
    if(m_sortRole != SortRole::None)
        invalidate();
}

bool LibrarySortFilterModel::lessThan(const QModelIndex& left,
                                      const QModelIndex& right) const
{
    switch(m_sortRole)
    {
    case SortRole::Title:
    {
        QVariant leftData = sourceModel()->data(left, LibraryModel::TitleRole);
        QVariant rightData =
            sourceModel()->data(right, LibraryModel::TitleRole);

        return leftData.toString().toLower() < rightData.toString().toLower();
    }
    case SortRole::LastOpened:
    {
        QVariant leftData =
            sourceModel()->data(left, LibraryModel::LastOpenedRole);
        QVariant rightData =
            sourceModel()->data(right, LibraryModel::LastOpenedRole);

        auto leftLastOpenedDate =
            QDateTime::fromString(leftData.toString(), m_dateTimeFormat);
        auto rightLastOpenedDate =
            QDateTime::fromString(rightData.toString(), m_dateTimeFormat);

        if(!leftLastOpenedDate.isValid())
            return false;
        else if(!rightLastOpenedDate.isValid())
            return true;

        return leftLastOpenedDate > rightLastOpenedDate;
    }
    case SortRole::RecentlyAdded:
    {
        QVariant leftData =
            sourceModel()->data(left, LibraryModel::AddedToLibraryRole);
        QVariant rightData =
            sourceModel()->data(right, LibraryModel::AddedToLibraryRole);

        auto leftRecentlyAddedDate =
            QDateTime::fromString(leftData.toString(), m_dateTimeFormat);
        auto rightRecentlyAddedDate =
            QDateTime::fromString(rightData.toString(), m_dateTimeFormat);

        if(!leftRecentlyAddedDate.isValid())
            return false;
        else if(!rightRecentlyAddedDate.isValid())
            return true;

        return leftRecentlyAddedDate > rightRecentlyAddedDate;
    }
    default:
        return false;
    }
}

}  // namespace adapters::data_models