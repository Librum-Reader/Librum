#include "library_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <rapidfuzz/fuzz.hpp>
#include "book.hpp"
#include "library_model.hpp"

using domain::models::Book;

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
        return false;
    }
}

bool LibraryProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex& source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);

    auto authorsData = sourceModel()->data(index, LibraryModel::AuthorsRole);
    auto authors = authorsData.toString().toLower();
    if(!m_filterAuthors.isEmpty() && m_filterAuthors != authors)
        return false;

    auto formatData = sourceModel()->data(index, LibraryModel::FormatRole);
    auto format = formatData.toString().toLower();
    if(!m_filterFormat.isEmpty() && m_filterFormat != format)
        return false;

    if(m_filterForOnlyFiles && format != "plain")
        return false;

    if(m_filterForOnlyBooks && format == "plain")
        return false;

    return true;
}

double LibraryProxyModel::fuzzCompareWithSortingString(QString lhs) const
{
    // If the sorting string is a substring of the title, return a high ratio
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
        QDateTime::fromString(lhs.toString(), Book::dateTimeStringFormat);
    auto rhsLastOpenedDate =
        QDateTime::fromString(rhs.toString(), Book::dateTimeStringFormat);

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

    auto lhsAddedDate =
        QDateTime::fromString(lhs.toString(), Book::dateTimeStringFormat);
    auto rhsAddedDate =
        QDateTime::fromString(rhs.toString(), Book::dateTimeStringFormat);

    if(!lhsAddedDate.isValid())
        return false;
    else if(!rhsAddedDate.isValid())
        return true;

    return lhsAddedDate > rhsAddedDate;
}

QString LibraryProxyModel::getFilterAuthors() const
{
    return m_filterAuthors;
}

void LibraryProxyModel::setFilterAuthors(const QString& newFilterAuthor)
{
    m_filterAuthors = newFilterAuthor.toLower();
    invalidateFilter();
}

QString LibraryProxyModel::getFilterFormat() const
{
    return m_filterFormat;
}

void LibraryProxyModel::setFilterFormat(const QString& newFilterFormat)
{
    m_filterFormat = newFilterFormat.toLower();
    qDebug() << newFilterFormat;
    invalidateFilter();
}

bool LibraryProxyModel::getFilterForOnlyFiles() const
{
    return m_filterForOnlyFiles;
}

void LibraryProxyModel::setFilterForOnlyFiles(bool newFilterForOnlyFiles)
{
    m_filterForOnlyFiles = newFilterForOnlyFiles;
    invalidateFilter();
}

bool LibraryProxyModel::getFilterForOnlyBooks() const
{
    return m_filterForOnlyBooks;
}

void LibraryProxyModel::setFilterForOnlyBooks(bool newFilterForOnlyBooks)
{
    m_filterForOnlyBooks = newFilterForOnlyBooks;
    invalidateFilter();
}

}  // namespace adapters::data_models