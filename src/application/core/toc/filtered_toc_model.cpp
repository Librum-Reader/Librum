#include "filtered_toc_model.hpp"
#include <algorithm>
#include "string_utils.hpp"
#include "toc_model.hpp"

namespace application::core
{

FilteredTOCModel::FilteredTOCModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
}

bool FilteredTOCModel::filterAcceptsRow(int row,
                                        const QModelIndex& parent) const
{
    auto index = sourceModel()->index(row, 0, parent);
    auto title = sourceModel()->data(index, TOCModel::Roles::TitleRole);

    auto temp = sourceModel()->index(row, 0, parent);
    TOCItem* item = static_cast<TOCItem*>(temp.internalPointer());
    if(item != nullptr && hasChildrenMatchingTheFilter(item))
        return true;

    return false;
}

void FilteredTOCModel::setFilterString(QString filterString)
{
    m_filterString = filterString;
    if(!filterString.isEmpty())
    {
        m_filterScorer =
            std::make_unique<rapidfuzz::fuzz::CachedRatio<unsigned int>>(
                filterString.toUtf8());
    }

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

    return std::any_of(item->getChildren().begin(), item->getChildren().end(),
                       [this](const TOCItem* child)
                       {
                           return hasChildrenMatchingTheFilter(child);
                       });
}

bool FilteredTOCModel::itemPassesFilter(const TOCItem* item) const
{
    auto similarity = string_utils::similarity(
        item->data().title, m_filterString, m_filterScorer.get());
    double minSimilarity = 70;

    return similarity >= minSimilarity;
}

}  // namespace application::core
