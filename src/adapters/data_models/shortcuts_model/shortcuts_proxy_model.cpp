#include "shortcuts_proxy_model.hpp"
#include <QAbstractItemModel>
#include <QDebug>
#include <algorithm>
#include <numeric>
#include "book.hpp"
#include "tag_dto.hpp"

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
}

bool ShortcutsProxyModel::filterAcceptsRow(
    int source_row, const QModelIndex& source_parent) const
{
}

void ShortcutsProxyModel::setFilterString(QString newSortString)
{
    m_filterString = newSortString;
    emit filterStringUpdated();
    invalidate();
}

QString ShortcutsProxyModel::getFilterString()
{
    return m_filterString;
}

}  // namespace adapters::data_models