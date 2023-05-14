#include "shortcuts_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include <numeric>
#include "book.hpp"
#include "shortcuts_model.hpp"
#include "string_utils.hpp"
#include "tag_dto.hpp"

using adapters::data_models::ShortcutsModel::ShortcutRole;
using adapters::data_models::ShortcutsModel::ValueRole;
using adapters::dtos::TagDto;
using domain::entities::Book;
using domain::entities::Tag;

namespace adapters::data_models
{

ShortcutsProxyModel::ShortcutsProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
}

bool ShortcutsProxyModel::lessThan(const QModelIndex& left,
                                   const QModelIndex& right) const
{
    QString leftName = sourceModel()->data(left, ShortcutRole).toString();
    QString rightName = sourceModel()->data(right, ShortcutRole).toString();

    // Sort alphabetically if no filter string is set
    if(m_filterString.isEmpty())
        return leftName < rightName;

    // Sort by the highest similarity if filter string is set
    auto lSimilarity = string_utils::fuzzCompare(leftName, m_filterString);
    auto rSimilarity = string_utils::fuzzCompare(rightName, m_filterString);
    return lSimilarity >= rSimilarity;
}

bool ShortcutsProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex& source_parent) const
{
    if(m_filterString.isEmpty())
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    auto shortcut = sourceModel()->data(index, ShortcutRole).toString();
    auto shortcutSimilarityToFilterString =
        string_utils::fuzzCompare(shortcut, m_filterString);
    if(shortcutSimilarityToFilterString >= 60)
        return true;

    return false;
}

void ShortcutsProxyModel::setFilterString(QString newFilterString)
{
    m_filterString = newFilterString;
    emit filterStringUpdated();
    invalidate();
}

QString ShortcutsProxyModel::getFilterString()
{
    return m_filterString;
}

}  // namespace adapters::data_models