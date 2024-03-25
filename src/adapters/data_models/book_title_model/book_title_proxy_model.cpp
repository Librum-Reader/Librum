#include "book_title_proxy_model.hpp"
#include "book_title_model.hpp"
#include "string_utils.hpp"

namespace adapters::data_models
{

BookTitleProxyModel::BookTitleProxyModel(QObject* parent) :
    QSortFilterProxyModel { parent }
{
    sort(0);
}

bool BookTitleProxyModel::lessThan(const QModelIndex& left,
                                   const QModelIndex& right) const
{
    auto result = leftBookIsCloserToSortString(left, right);
    return result;
}

bool BookTitleProxyModel::leftBookIsCloserToSortString(
    const QModelIndex& left, const QModelIndex& right) const
{
    if(m_sortString.isEmpty())
        return false;

    auto leftTitle = sourceModel()->data(left, BookTitleModel::TitleRole);
    auto rightTitle = sourceModel()->data(right, BookTitleModel::TitleRole);

    double leftRatio = string_utils::similarity(
        leftTitle.toString(), m_sortString, m_filterScorer.get());
    double rightRatio = string_utils::similarity(
        rightTitle.toString(), m_sortString, m_filterScorer.get());

    if(leftRatio > rightRatio)
        return true;
    else
        return false;
}

bool BookTitleProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex& source_parent) const
{
    auto index = sourceModel()->index(source_row, 0, source_parent);

    return filterAcceptsDownloaded(index) && filterAcceptsExtension(index);
}

bool BookTitleProxyModel::filterAcceptsDownloaded(
    const QModelIndex& index) const
{
    if(!m_showOnlyDownloaded)
        return true;

    return sourceModel()->data(index, BookTitleModel::DownloadedRole).toBool();
}

bool BookTitleProxyModel::filterAcceptsExtension(const QModelIndex& index) const
{
    if(m_extension.isEmpty())
        return true;

    auto extensionData =
        sourceModel()->data(index, BookTitleModel::ExtensionRole);
    auto extension = extensionData.toString().toLower();

    // Remove the version details from the extension
    int firstSpaceIndex = extension.indexOf(' ');
    if(firstSpaceIndex != -1)
        extension = extension.left(firstSpaceIndex);


    return m_extension.split(",").contains(extension);
}

bool BookTitleProxyModel::getShowOnlyDownloaded() const
{
    return m_showOnlyDownloaded;
}

void BookTitleProxyModel::setShowOnlyDownloaded(bool newShowOnlyDownloaded)
{
    if(m_showOnlyDownloaded == newShowOnlyDownloaded)
        return;

    m_showOnlyDownloaded = newShowOnlyDownloaded;
    emit showOnlyDownloadedUpdated();
    invalidateFilter();
}

QString BookTitleProxyModel::getSortString() const
{
    return m_sortString;
}

void BookTitleProxyModel::setSortString(const QString& newSortString)
{
    if(m_sortString == newSortString)
        return;

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

QString BookTitleProxyModel::getExtension() const
{
    return m_extension;
}

void BookTitleProxyModel::setExtension(const QString& newExtension)
{
    if(m_extension == newExtension)
        return;

    m_extension = newExtension;
    emit extensionUpdated();
    invalidateFilter();
}


}  // namespace adapters::data_models