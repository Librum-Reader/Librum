#include "icon_proxy_model.hpp"
#include <QDebug>
#include "icon_model.hpp"

namespace adapters::data_models
{

IconProxyModel::IconProxyModel()
{
}

bool IconProxyModel::filterAcceptsRow(int source_row,
                                      const QModelIndex& source_parent) const
{
    if(m_sortString.isEmpty())
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);
    auto keywords =
        sourceModel()->data(index, IconModel::KeywordsRole).toList();

    bool anyKeywordIsSimilar = false;
    for(auto keyword : keywords)
    {
        auto str = keyword.toString().toLower();
        auto containsAim = str.contains(m_sortString);
        auto isSimilarToAim = rapidfuzz::fuzz::ratio(m_sortString.toStdString(),
                                                     str.toStdString()) >= 95;

        if(containsAim || isSimilarToAim)
        {
            anyKeywordIsSimilar = true;
            break;
        }
    }

    return anyKeywordIsSimilar;
}

QString IconProxyModel::getSortString() const
{
    return m_sortString;
}

void IconProxyModel::setSortString(const QString& newSortString)
{
    if(m_sortString == newSortString.toLower())
        return;

    m_sortString = newSortString.toLower();
    emit sortStringUpdated();
    invalidateFilter();
}

}  // namespace adapters::data_models