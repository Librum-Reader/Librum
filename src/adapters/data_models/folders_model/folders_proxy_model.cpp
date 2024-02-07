#include "folders_proxy_model.hpp"
#include <memory>
#include "folders_model.hpp"
#include "string_utils.hpp"

namespace adapters::data_models
{

using domain::entities::Folder;

FoldersProxyModel::FoldersProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
}

bool FoldersProxyModel::filterAcceptsRow(int row,
                                         const QModelIndex& parent) const
{
    auto index = sourceModel()->index(row, 0, parent);
    auto name = sourceModel()->data(index, FoldersModel::Roles::NameRole);

    auto item = static_cast<Folder*>(index.internalPointer());
    if(item != nullptr && hasChildrenMatchingTheFilter(item))
        return true;

    return false;
}

void FoldersProxyModel::setFilterString(QString filterString)
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

QString FoldersProxyModel::getFilterString()
{
    return m_filterString;
}

bool FoldersProxyModel::hasChildrenMatchingTheFilter(const Folder* item) const
{
    if(itemPassesFilter(item))
        return true;

    return std::any_of(item->getChildren().begin(), item->getChildren().end(),
                       [this](const std::unique_ptr<Folder>& child)
                       {
                           return hasChildrenMatchingTheFilter(child.get());
                       });
}

bool FoldersProxyModel::itemPassesFilter(const Folder* item) const
{
    auto similarity = string_utils::similarity(item->getName(), m_filterString,
                                               m_filterScorer.get());
    double minSimilarity = 70;

    return similarity >= minSimilarity;
}

}  // namespace adapters::data_models