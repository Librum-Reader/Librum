#include "filtered_tocmodel.hpp"
#include <algorithm>
#include "string_utils.hpp"

namespace Okular
{

FilteredTOCModel::FilteredTOCModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
}

bool FilteredTOCModel::filterAcceptsRow(int row,
                                        const QModelIndex& parent) const
{
    auto index = sourceModel()->index(row, 0, parent);
    auto name = sourceModel()->data(index, Qt::ToolTipRole);

    auto x = sourceModel()->index(row, 0, parent);
    TOCItem* item = static_cast<TOCItem*>(x.internalPointer());
    if(item != nullptr && hasChildrenMatchingTheFilter(item))
        return true;

    auto similarity =
        string_utils::fuzzCompare(name.toString(), m_filterString);
    double minSimilarity = 70;

    return similarity >= minSimilarity;
}

void FilteredTOCModel::setFilterString(QString filterString)
{
    m_filterString = filterString;
    invalidateFilter();
}

QString FilteredTOCModel::getFilterString()
{
    return m_filterString;
}

bool FilteredTOCModel::hasChildrenMatchingTheFilter(const TOCItem* item) const
{
    if(itemPassesFilter(item))
        return true;

    return std::any_of(item->children.begin(), item->children.end(),
                       [this](const TOCItem* child)
                       {
                           return hasChildrenMatchingTheFilter(child);
                       });
}

bool FilteredTOCModel::itemPassesFilter(const TOCItem* item) const
{
    auto similarity = string_utils::fuzzCompare(item->text, m_filterString);
    double minSimilarity = 70;

    return similarity >= minSimilarity;
}

}  // namespace Okular