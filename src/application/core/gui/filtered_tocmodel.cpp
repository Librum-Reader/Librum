#include "filtered_tocmodel.hpp"
#include <algorithm>
#include <rapidfuzz/fuzz.hpp>

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

    auto similarity = fuzzCompareWithFilterString(name.toString());
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
    auto similarity = fuzzCompareWithFilterString(item->text);
    double minSimilarity = 70;

    return similarity >= minSimilarity;
}

double FilteredTOCModel::fuzzCompareWithFilterString(QString str) const
{
    // If the sorting string is a sub-string of str, return a high ratio
    auto leftSubstrPos = str.toLower().indexOf(m_filterString.toLower());
    if(leftSubstrPos != -1)
    {
        // The further at the front, the better the ratio should be
        double ratio = 100 - leftSubstrPos;
        // A difference in length of the strings should reduce the score
        ratio -= std::abs(str.length() - m_filterString.length()) * 0.1;

        return ratio;
    }

    return rapidfuzz::fuzz::ratio(m_filterString.toStdString(),
                                  str.toStdString());
}

}  // namespace Okular