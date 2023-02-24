#include "filtered_tocmodel.hpp"
#include <rapidfuzz/fuzz.hpp>

namespace Okular
{

FilteredTOCModel::FilteredTOCModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
}

bool FilteredTOCModel::filterAcceptsRow(int source_row,
                                        const QModelIndex& source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);
    auto rawAuthors = sourceModel()->data(index, Qt::ToolTipRole);

    auto similarity = fuzzCompareWithFilterString(rawAuthors.toString());
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