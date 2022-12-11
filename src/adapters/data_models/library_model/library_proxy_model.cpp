#include "library_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include <numeric>
#include <rapidfuzz/fuzz.hpp>
#include "book.hpp"
#include "library_model.hpp"
#include "tag_dto.hpp"

using domain::models::Book;
using domain::models::Tag;

namespace adapters::data_models
{

LibraryProxyModel::LibraryProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
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

    // tags
    auto qTags = sourceModel()
                     ->data(index, LibraryModel::TagsRole)
                     .value<QList<dtos::TagDto>>();
    std::vector<dtos::TagDto> tags;
    for(auto tag : qTags)
    {
        tags.emplace_back(std::move(tag));
    }

    bool v = std::ranges::all_of(m_tags,
                                 [&tags](const QString& tagName)
                                 {
                                     auto pos = std::ranges::find_if(
                                         tags,
                                         [&tagName](const dtos::TagDto& tag)
                                         {
                                             return tag.name == tagName;
                                         });

                                     return pos != std::end(tags);
                                 });

    if(!m_tags.empty() && !v)
        return false;

    auto authorsData = sourceModel()->data(index, LibraryModel::AuthorsRole);
    auto authors = authorsData.toString().toLower();
    bool authorsNotSimilar =
        rapidfuzz::fuzz::ratio(m_filterRequest.authors.toStdString(),
                               authors.toStdString()) < 55;
    if(!m_filterRequest.authors.isEmpty() &&
       (!authors.contains(m_filterRequest.authors) && authorsNotSimilar))
        return false;

    auto formatData = sourceModel()->data(index, LibraryModel::FormatRole);
    auto format = formatData.toString().toLower();
    if(!m_filterRequest.format.isEmpty() && m_filterRequest.format != format)
        return false;

    if(m_filterRequest.onlyFiles && format != "plain")
        return false;

    if(m_filterRequest.onlyBooks && format == "plain")
        return false;

    auto currentPage =
        sourceModel()->data(index, LibraryModel::CurrentPageRole).toInt();
    auto pageCount =
        sourceModel()->data(index, LibraryModel::PageCountRole).toInt();
    if(m_filterRequest.read && currentPage != pageCount)
        return false;

    if(m_filterRequest.unread && currentPage == pageCount)
        return false;

    return true;
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

void LibraryProxyModel::setFilterRequest(QString authors, QString format,
                                         QString date, bool onlyBooks,
                                         bool onlyFiles, bool read, bool unread)
{
    m_filterRequest = FilterRequest {
        .authors = authors.toLower(),
        .format = format.toLower(),
        .date = date.toLower(),
        .onlyBooks = onlyBooks,
        .onlyFiles = onlyFiles,
        .read = read,
        .unread = unread,
    };

    invalidateFilter();
}

void LibraryProxyModel::addFilterTag(QString tag)
{
    m_tags.emplace_back(tag);
    invalidateFilter();
}

void LibraryProxyModel::removeFilterTag(QString tagToRemove)
{
    auto pos = std::ranges::find(m_tags, tagToRemove);
    if(pos == m_tags.end())
        return;

    m_tags.erase(pos);
    invalidateFilter();
}

void LibraryProxyModel::clearFilterTags()
{
    m_tags.clear();
    invalidateFilter();
}

double LibraryProxyModel::fuzzCompareWithSortingString(QString str) const
{
    // If the sorting string is a substring of str, return a high ratio
    auto leftSubstrPos = str.toLower().indexOf(m_sortString.toLower());
    if(leftSubstrPos != -1)
    {
        // The further at the front, the better the ratio should be
        double ratio = 100 - leftSubstrPos;
        // A difference in lentgh of the strings should reduce the score
        ratio -= std::abs(str.length() - m_sortString.length()) * 0.1;

        return ratio;
    }

    return rapidfuzz::fuzz::ratio(m_sortString.toStdString(),
                                  str.toStdString());
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

}  // namespace adapters::data_models