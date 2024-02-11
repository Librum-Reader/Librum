#include "library_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include "book.hpp"
#include "library_model.hpp"
#include "string_utils.hpp"
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
    auto result = leftBookIsCloserToSortString(left, right);
    if(result.has_value())
        return result.value();


    // If no sort string is set, compare by sort role
    switch(m_sortRole)
    {
    case SortRole::Title:
    {
        auto leftTitle = sourceModel()->data(left, LibraryModel::TitleRole);
        auto rightTitle = sourceModel()->data(right, LibraryModel::TitleRole);
        return string_utils::lexicographicallyLess(leftTitle.toString(),
                                                   rightTitle.toString());
    }
    case SortRole::Authors:
    {
        auto leftData = sourceModel()->data(left, LibraryModel::AuthorsRole);
        auto rightData = sourceModel()->data(right, LibraryModel::AuthorsRole);

        return string_utils::lexicographicallyLess(leftData.toString(),
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
    case SortRole::Progress:
    {
        return hasHigherReadingProgress(left, right);
    }
    case SortRole::SortRole_END:
    {
        qCritical() << QString("Sorting called with role: 'SortRole_END'");
        return false;
    }
    default:
        return false;
    }
}

std::optional<bool> LibraryProxyModel::leftBookIsCloserToSortString(
    const QModelIndex& left, const QModelIndex& right) const
{
    // If no sort string is set, abort
    if(m_sortString.isEmpty())
        return std::nullopt;

    auto leftTitle = sourceModel()->data(left, LibraryModel::TitleRole);
    auto rightTitle = sourceModel()->data(right, LibraryModel::TitleRole);

    double leftRatio = string_utils::similarity(
        leftTitle.toString(), m_sortString, m_filterScorer.get());
    double rightRatio = string_utils::similarity(
        rightTitle.toString(), m_sortString, m_filterScorer.get());

    if(leftRatio > rightRatio)
        return true;
    if(leftRatio < rightRatio)
        return false;

    return std::nullopt;
}

bool LibraryProxyModel::filterAcceptsRow(int source_row,
                                         const QModelIndex& source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);

    if(!getIsFiltering())
        return filterAcceptsFolder(index) && filterAcceptsTags(index);

    return filterAcceptsTags(index) && filterAcceptsAuthors(index) &&
           filterAcceptsFormat(index) && filterAcceptsStatus(index) &&
           filterAcceptsLanguage(index) && filterAcceptsFolder(index);
}

void LibraryProxyModel::setFilterRequest(QString authors, QString format,
                                         QString language, bool onlyBooks,
                                         bool onlyFiles, bool read, bool unread)
{
    m_filterRequest = FilterRequest {
        .authors = authors.toLower(),
        .format = format.toLower(),
        .language = language.toLower(),
        .onlyBooks = onlyBooks,
        .onlyFiles = onlyFiles,
        .read = read,
        .unread = unread,
    };

    emit filterUpdated();
    invalidateFilter();
}

bool LibraryProxyModel::getIsFiltering() const
{
    return !m_filterRequest.authors.isEmpty() ||
           !m_filterRequest.format.isEmpty() ||
           !m_filterRequest.language.isEmpty() || m_filterRequest.onlyBooks ||
           m_filterRequest.onlyFiles || m_filterRequest.read ||
           m_filterRequest.unread;
}

void LibraryProxyModel::setSortRole(int newRole)
{
    int firstRole = SortRole::RecentlyAdded;

    bool inRange = firstRole <= newRole && newRole <= SortRole::SortRole_END;
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
    if(!newSortString.isEmpty())
    {
        m_filterScorer =
            std::make_unique<rapidfuzz::fuzz::CachedRatio<unsigned int>>(
                newSortString.toUtf8());
    }

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

    emit filterUpdated();
    invalidateFilter();
}

void LibraryProxyModel::removeFilterTag(QString tagToRemove)
{
    auto pos = std::ranges::find(m_tags, tagToRemove);
    if(pos == m_tags.end())
        return;

    m_tags.erase(pos);

    emit filterUpdated();
    invalidateFilter();
}

void LibraryProxyModel::clearFilterTags()
{
    m_tags.clear();

    emit filterUpdated();
    invalidateFilter();
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

    if(!rhsLastOpenedDate.isValid())
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

    if(!rhsAddedDate.isValid())
        return true;

    return lhsAddedDate > rhsAddedDate;
}

bool LibraryProxyModel::hasHigherReadingProgress(const QModelIndex& left,
                                                 const QModelIndex& right) const
{
    auto lhs = sourceModel()->data(left, LibraryModel::BookReadingProgressRole);
    auto rhs =
        sourceModel()->data(right, LibraryModel::BookReadingProgressRole);

    return lhs.toInt() >= rhs.toInt();
}

bool LibraryProxyModel::filterAcceptsTags(const QModelIndex& bookIndex) const
{
    auto tags = getTags(bookIndex);

    return m_tags.empty() || bookContainsAllTags(tags);
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

    return authorsContainsRequest || authorsAndRequestAreSimilar;
}

bool LibraryProxyModel::filterAcceptsFormat(const QModelIndex& bookIndex) const
{
    auto formatData = sourceModel()->data(bookIndex, LibraryModel::FormatRole);
    auto format = formatData.toString().toLower();
    // Remove the version details from the format
    int firstSpaceIndex = format.indexOf(' ');
    if(firstSpaceIndex != -1)
        format = format.left(firstSpaceIndex);


    if(m_filterRequest.format.isEmpty() && !m_filterRequest.onlyFiles &&
       !m_filterRequest.onlyBooks)
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

bool LibraryProxyModel::filterAcceptsLanguage(
    const QModelIndex& bookIndex) const
{
    bool requestIsEmpty = m_filterRequest.language.isEmpty();
    if(requestIsEmpty)
        return true;

    auto rawLanguage =
        sourceModel()->data(bookIndex, LibraryModel::LanguageRole);
    auto language = rawLanguage.toString().toLower();

    return language == m_filterRequest.language;
}

bool LibraryProxyModel::filterAcceptsFolder(const QModelIndex& bookIndex) const
{
    if(m_folder == "all")
        return true;

    auto uuid = sourceModel()
                    ->data(bookIndex, LibraryModel::ParentFolderIdRole)
                    .toString();

    auto emptyUuidString = QUuid().toString(QUuid::WithoutBraces);
    if(m_folder == "unsorted" && uuid == emptyUuidString)
        return true;

    return uuid == m_folder;
}

QString LibraryProxyModel::getFolderFilter() const
{
    return m_folder;
}

void LibraryProxyModel::setFolderFilter(const QString& newFolder)
{
    if(m_folder == newFolder)
        return;

    m_folder = newFolder;

    emit folderFilterChanged();
    invalidateFilter();
}

}  // namespace adapters::data_models
