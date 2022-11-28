#include "library_proxy_model.hpp"
#include <QDebug>
#include <rapidfuzz/fuzz.hpp>
#include "library_model.hpp"

namespace adapters::data_models
{

LibraryProxyModel::LibraryProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
}

void LibraryProxyModel::setSortRole(int newRole)
{
    int firstRole = SortRole::None;
    int lastRole = SortRole::RecentlyAdded;
    bool inRange = firstRole <= newRole <= lastRole;

    if(!inRange)
        return;

    m_sortRole = static_cast<SortRole>(newRole);
    invalidate();
}

void LibraryProxyModel::setSortString(QString newSortString)
{
    m_sortString = newSortString;
    invalidate();
}

bool LibraryProxyModel::lessThan(const QModelIndex& left,
                                 const QModelIndex& right) const
{
    if(!m_sortString.isEmpty())
    {
        auto leftData = sourceModel()->data(left, LibraryModel::TitleRole);
        auto rightData = sourceModel()->data(right, LibraryModel::TitleRole);

        double leftRatio = fuzzCompareWithSortingString(leftData.toString());
        double rightRatio = fuzzCompareWithSortingString(rightData.toString());

        if(leftRatio > rightRatio)
            return true;
        if(leftRatio < rightRatio)
            return false;
    }

    switch(m_sortRole)
    {
    case SortRole::Title:
    {
        QVariant leftData = sourceModel()->data(left, LibraryModel::TitleRole);
        QVariant rightData =
            sourceModel()->data(right, LibraryModel::TitleRole);

        if(leftData.toString().isEmpty())
            return false;
        else if(rightData.toString().isEmpty())
            return true;

        return leftData.toString().toLower() < rightData.toString().toLower();
    }
    case SortRole::Authors:
    {
        QVariant leftData =
            sourceModel()->data(left, LibraryModel::AuthorsRole);
        QVariant rightData =
            sourceModel()->data(right, LibraryModel::AuthorsRole);

        if(leftData.toString().isEmpty())
            return false;
        else if(rightData.toString().isEmpty())
            return true;

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

double LibraryProxyModel::fuzzCompareWithSortingString(QString lhs) const
{
    auto leftSubstrPos = lhs.toLower().indexOf(m_sortString.toLower());

    if(leftSubstrPos != -1)
        return 100 - leftSubstrPos;

    return rapidfuzz::fuzz::ratio(m_sortString.toStdString(),
                                  lhs.toStdString());
}

}  // namespace adapters::data_models