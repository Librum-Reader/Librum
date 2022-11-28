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
    int firstRole = SortRole::RecentlyAdded;
    int lastRole = SortRole::LastOpened;

    bool inRange = firstRole <= newRole <= lastRole;
    if(!inRange)
        return;

    m_sortRole = static_cast<SortRole>(newRole);
    invalidate();
}

int LibraryProxyModel::getSortRole()
{
    return m_sortRole;
}

void LibraryProxyModel::setSortString(QString newSortString)
{
    m_sortString = newSortString;
    invalidate();
}

QString LibraryProxyModel::getSortString()
{
    return m_sortString;
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
        auto leftData = sourceModel()->data(left, LibraryModel::TitleRole);
        auto rightData = sourceModel()->data(right, LibraryModel::TitleRole);

        return stringIsLexicographicallyLess(leftData.toString(),
                                             rightData.toString());
    }
    case SortRole::Authors:
    {
        auto leftData = sourceModel()->data(left, LibraryModel::AuthorsRole);
        auto rightData = sourceModel()->data(right, LibraryModel::AuthorsRole);

        return stringIsLexicographicallyLess(leftData.toString(),
                                             rightData.toString());
    }
    case SortRole::LastOpened:
    {
        return openedAfter(left, right);
    }
    case SortRole::RecentlyAdded:
    {
        return addedToLibraryAfter(left, right);
    }
    default:
    {
        return false;
    }
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

bool LibraryProxyModel::stringIsLexicographicallyLess(
    const QString& left, const QString& right) const
{
    if(left.isEmpty())
        return false;
    else if(right.isEmpty())
        return true;

    return left.toLower() < right.toLower();
}

bool LibraryProxyModel::openedAfter(const QModelIndex& left,
                                    const QModelIndex& right) const
{
    auto lhs = sourceModel()->data(left, LibraryModel::LastOpenedRole);
    auto rhs = sourceModel()->data(right, LibraryModel::LastOpenedRole);

    auto lhsLastOpenedDate =
        QDateTime::fromString(lhs.toString(), m_dateTimeFormat);
    auto rhsLastOpenedDate =
        QDateTime::fromString(rhs.toString(), m_dateTimeFormat);

    if(!lhsLastOpenedDate.isValid())
        return false;
    else if(!rhsLastOpenedDate.isValid())
        return true;

    return lhsLastOpenedDate > rhsLastOpenedDate;
}

bool LibraryProxyModel::addedToLibraryAfter(const QModelIndex& left,
                                            const QModelIndex& right) const
{
    auto lhs = sourceModel()->data(left, LibraryModel::AddedToLibraryRole);
    auto rhs = sourceModel()->data(right, LibraryModel::AddedToLibraryRole);

    auto lhsAddedDate = QDateTime::fromString(lhs.toString(), m_dateTimeFormat);
    auto rhsAddedDate = QDateTime::fromString(rhs.toString(), m_dateTimeFormat);

    if(!lhsAddedDate.isValid())
        return false;
    else if(!rhsAddedDate.isValid())
        return true;

    return lhsAddedDate > rhsAddedDate;
}

}  // namespace adapters::data_models