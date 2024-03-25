#pragma once
#include <QSortFilterProxyModel>
#include <memory>
#include <rapidfuzz/fuzz.hpp>
#include "adapters_export.hpp"

namespace adapters::data_models
{

/**
 * The BookTitleProxyModel class is a proxy class which acts as a wrapper around
 * the book title model that is handling the sorting and filtering of elements.
 */
class ADAPTERS_EXPORT BookTitleProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool showOnlyDownloaded READ getShowOnlyDownloaded WRITE
                   setShowOnlyDownloaded NOTIFY showOnlyDownloadedUpdated)
    Q_PROPERTY(QString sortString READ getSortString WRITE setSortString NOTIFY
                   sortStringUpdated)
    Q_PROPERTY(QString extension READ getExtension WRITE setExtension NOTIFY
                   extensionUpdated)

public:
    explicit BookTitleProxyModel(QObject* parent = nullptr);

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;

    bool getShowOnlyDownloaded() const;
    void setShowOnlyDownloaded(bool newShowOnlyDownloaded);

    QString getSortString() const;
    void setSortString(const QString& newSortString);

    QString getExtension() const;
    void setExtension(const QString& newExtension);

signals:
    void showOnlyDownloadedUpdated();
    void sortStringUpdated();

    void extensionUpdated();

private:
    bool leftBookIsCloserToSortString(const QModelIndex& left,
                                      const QModelIndex& right) const;
    bool filterAcceptsDownloaded(const QModelIndex& index) const;
    bool filterAcceptsExtension(const QModelIndex& index) const;

    QString m_sortString;
    std::unique_ptr<rapidfuzz::fuzz::CachedRatio<unsigned int>> m_filterScorer;
    bool m_showOnlyDownloaded = false;
    QString m_extension;
};

}  // namespace adapters::data_models
