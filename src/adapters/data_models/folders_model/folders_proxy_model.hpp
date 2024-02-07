#pragma once
#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>
#include <rapidfuzz/fuzz.hpp>
#include "adapters_export.hpp"
#include "folder.hpp"

namespace adapters::data_models
{

class ADAPTERS_EXPORT FoldersProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ getFilterString WRITE setFilterString
                   NOTIFY filterStringUpdated)

public:
    explicit FoldersProxyModel(QObject* parent = nullptr);

    bool filterAcceptsRow(int row, const QModelIndex& parent) const override;
    void setFilterString(QString filterString);
    QString getFilterString();

signals:
    void filterStringUpdated();

private:
    // Recursively check if the item or any of its children match the filter
    bool hasChildrenMatchingTheFilter(
        const domain::entities::Folder* item) const;
    bool itemPassesFilter(const domain::entities::Folder* item) const;

    QString m_filterString;
    std::unique_ptr<rapidfuzz::fuzz::CachedRatio<unsigned int>> m_filterScorer;
};

}  // namespace adapters::data_models