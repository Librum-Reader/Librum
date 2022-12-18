#include "library_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include <numeric>
#include <rapidfuzz/fuzz.hpp>
#include "book.hpp"
#include "library_model.hpp"
#include "tag_dto.hpp"

using adapters::dtos::TagDto;
using domain::entities::Book;
using domain::entities::Tag;

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
    auto fuzzResult = fuzzCompareBooks(left, right);
    if(fuzzResult.has_value())
        return fuzzResult.value();

    switch(m_sortRole)
    {
    case SortRole::Title:
    {
        auto leftTitle = sourceModel()->data(left, LibraryModel::TitleRole);
        auto rightTitle = sourceModel()->data(right, LibraryModel::TitleRole);
        return stringIsLexicographicallyLess(leftTitle.toString(),
                                             rightTitle.toString());
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

std::optional<bool> LibraryProxyModel::fuzzCompareBooks(
    const QModelIndex& left, const QModelIndex& right) const
{
    if(m_sortString.isEmpty())
        return std::nullopt;

    auto leftTitle = sourceModel()->data(left, LibraryModel::TitleRole);
    auto rightTitle = sourceModel()->data(right, LibraryModel::TitleRole);

    double leftRatio = fuzzCompareWithSortingString(leftTitle.toString());
    double rightRatio = fuzzCompareWithSortingString(rightTitle.toString());

    if(leftRatio > rightRatio)
        return true;
    if(leftRatio < rightRatio)
        return false;
    else
        return std::nullopt;
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

bool LibraryProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex& source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);

    if(filterAcceptsTags(index) && filterAcceptsAuthors(index) &&
       filterAcceptsFormat(index) && filterAcceptsStatus(index))
        return true;

    return false;
}

void LibraryProxyModel::setSortRole(int newRole)
{
    int firstRole = SortRole::RecentlyAdded;
    int lastRole = SortRole::LastOpened;

    bool inRange = firstRole <= newRole && newRole <= lastRole;
    if(!inRange)
        return;

    m_sortRole = static_cast<SortRole>(newRole);
    emit sortRoleUpdated();
    invalidate();
}

int LibraryProxyModel::getSortRole()
{
    return m_sortRole;
}

void LibraryProxyModel::setSortString(QString newSortString)
{
    m_sortString = newSortString;
    emit sortStringUpdated();
    invalidate();
}

QString LibraryProxyModel::getSortString()
{
    return m_sortString;
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

bool LibraryProxyModel::filterAcceptsTags(const QModelIndex& bookIndex) const
{
    auto tags = getTags(bookIndex);
    if(m_tags.empty() || bookContainsAllTags(tags))
        return true;

    return false;
}

std::vector<dtos::TagDto> LibraryProxyModel::getTags(
    const QModelIndex& index) const
{
    QList<TagDto> qTagList = sourceModel()
                                 ->data(index, LibraryModel::TagsRole)
                                 .value<QList<TagDto>>();

    std::vector<TagDto> tags;
    for(auto tag : qTagList)
    {
        tags.emplace_back(std::move(tag));
    }

    return tags;
}

bool LibraryProxyModel::bookContainsAllTags(std::vector<TagDto> tags) const
{
    for(const auto& tagName : m_tags)
    {
        auto pos = std::ranges::find_if(tags,
                                        [&tagName](const TagDto& tag)
                                        {
                                            return tag.name == tagName;
                                        });
        if(pos == tags.end())
            return false;
    }

    return true;
}

bool LibraryProxyModel::filterAcceptsAuthors(const QModelIndex& bookIndex) const
{
    bool requestIsEmpty = m_filterRequest.authors.isEmpty();
    if(requestIsEmpty)
        return true;

    auto rawAuthors = sourceModel()->data(bookIndex, LibraryModel::AuthorsRole);
    auto authors = rawAuthors.toString().toLower();

    bool authorsAndRequestAreSimilar =
        rapidfuzz::fuzz::ratio(m_filterRequest.authors.toStdString(),
                               authors.toStdString()) >= 55;
    bool authorsContainsRequest = authors.contains(m_filterRequest.authors);

    if(authorsContainsRequest || authorsAndRequestAreSimilar)
        return true;

    return false;
}

bool LibraryProxyModel::filterAcceptsFormat(const QModelIndex& bookIndex) const
{
    auto formatData = sourceModel()->data(bookIndex, LibraryModel::FormatRole);
    auto format = formatData.toString().toLower();
    if(m_filterRequest.format.isEmpty())
        return true;

    if(m_filterRequest.format == format)
        return true;

    if(m_filterRequest.onlyFiles && format == "plain")
        return true;

    if(m_filterRequest.onlyBooks && format != "plain")
        return true;

    return false;
}

bool LibraryProxyModel::filterAcceptsStatus(const QModelIndex& bookIndex) const
{
    auto currentPage =
        sourceModel()->data(bookIndex, LibraryModel::CurrentPageRole).toInt();
    auto pageCount =
        sourceModel()->data(bookIndex, LibraryModel::PageCountRole).toInt();

    if(m_filterRequest.read && currentPage != pageCount)
        return false;

    if(m_filterRequest.unread && currentPage == pageCount)
        return false;

    return true;
}

}  // namespace adapters::data_models