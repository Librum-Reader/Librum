#include "icon_proxy_model.hpp"
#include <QDebug>
#include "icon_model.hpp"

namespace adapters::data_models
{

IconProxyModel::IconProxyModel()
{
    sort(0);
}

bool IconProxyModel::lessThan(const QModelIndex& left,
                              const QModelIndex& right) const
{
    Q_UNUSED(right)

    auto rawName = sourceModel()->data(left, IconModel::NameRole);
    auto name = rawName.toString().toLower();

    // We want to ensure that the folder is always the first item, we don't care
    // about the ordering of the rest.
    if(name == "folder")
        return true;

    return false;
}

bool IconProxyModel::filterAcceptsRow(int source_row,
                                      const QModelIndex& source_parent) const
{
    if(m_sortString.isEmpty())
        return true;

    auto index = sourceModel()->index(source_row, 0, source_parent);

    // The folder icon should always be visible
    auto name =
        sourceModel()->data(index, IconModel::NameRole).toString().toLower();
    if(name == "folder")
        return true;

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
    sort(0);
}

}  // namespace adapters::data_models