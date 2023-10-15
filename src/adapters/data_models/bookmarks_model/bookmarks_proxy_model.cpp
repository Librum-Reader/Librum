#include "bookmarks_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include <numeric>
#include "book.hpp"
#include "bookmarks_model.hpp"
#include "string_utils.hpp"
#include "tag_dto.hpp"

using adapters::data_models::BookmarksModel::NameRole;
using adapters::data_models::BookmarksModel::PageNumberRole;
using adapters::dtos::TagDto;
using domain::entities::Book;
using domain::entities::Tag;

namespace adapters::data_models
{

BookmarksProxyModel::BookmarksProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
}

bool BookmarksProxyModel::lessThan(const QModelIndex& left,
                                   const QModelIndex& right) const
{
    QString leftName = sourceModel()->data(left, NameRole).toString();
    QString rightName = sourceModel()->data(right, NameRole).toString();

    QString leftPageNr = sourceModel()->data(left, PageNumberRole).toString();
    QString rightPageNr = sourceModel()->data(right, PageNumberRole).toString();

    // Sort alphabetically if no filter string is set
    if(m_filterString.isEmpty())
        return leftPageNr < rightPageNr;

    // Sort by the highest similarity if filter string is set
    auto lSimilarity = string_utils::fuzzCompare(leftName, m_filterString);
    auto rSimilarity = string_utils::fuzzCompare(rightName, m_filterString);
    return lSimilarity >= rSimilarity;
}

bool BookmarksProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex& source_parent) const
{
    if(m_filterString.isEmpty())
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    auto shortcut = sourceModel()->data(index, NameRole).toString();
    auto bookmarkSimilarityToFilterString =
        string_utils::fuzzCompare(shortcut, m_filterString);
    if(bookmarkSimilarityToFilterString >= 60)
        return true;

    return false;
}

void BookmarksProxyModel::setFilterString(QString newFilterString)
{
    m_filterString = newFilterString;
    emit filterStringUpdated();
    invalidate();
}

QString BookmarksProxyModel::getFilterString()
{
    return m_filterString;
}

}  // namespace adapters::data_models