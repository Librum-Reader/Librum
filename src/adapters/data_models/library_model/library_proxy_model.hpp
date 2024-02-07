#pragma once
#include <QSortFilterProxyModel>
#include <optional>
#include <rapidfuzz/fuzz.hpp>
#include <vector>
#include "adapters_export.hpp"
#include "filter_request.hpp"
#include "tag_dto.hpp"

namespace adapters::data_models
{

/**
 * The LibraryProxyModel class is a proxy class which acts as a wrapper around
 * the library model that is handling the sorting and filtering of elements
 */
class ADAPTERS_EXPORT LibraryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString sortString READ getSortString WRITE setSortString NOTIFY
                   sortStringUpdated)
    Q_PROPERTY(
        int sortRole READ getSortRole WRITE setSortRole NOTIFY sortRoleUpdated)
    Q_PROPERTY(bool isFiltering READ getIsFiltering NOTIFY filterUpdated)
    Q_PROPERTY(QString folder READ getFolderFilter WRITE setFolderFilter NOTIFY
                   folderFilterChanged)

public:
    enum SortRole
    {
        RecentlyAdded = Qt::UserRole + 1,
        Title,
        Authors,
        LastOpened,
        Progress,
        SortRole_END
    };

    Q_ENUM(SortRole);

    explicit LibraryProxyModel(QObject* parent = nullptr);

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;

    Q_INVOKABLE void setFilterRequest(QString authors, QString format,
                                      QString language, bool onlyBooks,
                                      bool onlyFiles, bool read, bool unread);
    Q_INVOKABLE bool getIsFiltering();
    Q_INVOKABLE void addFilterTag(QString tag);
    Q_INVOKABLE void removeFilterTag(QString tag);
    Q_INVOKABLE void clearFilterTags();

    void setSortRole(int newRole);
    int getSortRole();

    void setSortString(QString newSortString);
    QString getSortString();

    QString getFolderFilter() const;
    void setFolderFilter(const QString& newFolder);

signals:
    void sortStringUpdated();
    void sortRoleUpdated();
    void filterUpdated();
    void folderFilterChanged();

private:
    std::optional<bool> leftBookIsCloserToSortString(
        const QModelIndex& left, const QModelIndex& right) const;
    bool openedAfter(const QModelIndex& left, const QModelIndex& right) const;
    bool addedToLibraryAfter(const QModelIndex& left,
                             const QModelIndex& right) const;
    bool hasHigherReadingProgress(const QModelIndex& left,
                                  const QModelIndex& right) const;
    bool filterAcceptsTags(const QModelIndex& bookIndex) const;
    std::vector<adapters::dtos::TagDto> getTags(const QModelIndex& index) const;
    bool bookContainsAllTags(std::vector<adapters::dtos::TagDto> tags) const;
    bool filterAcceptsAuthors(const QModelIndex& bookIndex) const;
    bool filterAcceptsFormat(const QModelIndex& bookIndex) const;
    bool filterAcceptsStatus(const QModelIndex& bookIndex) const;
    bool filterAcceptsLanguage(const QModelIndex& bookIndex) const;
    bool filterAcceptsFolder(const QModelIndex& bookIndex) const;

    FilterRequest m_filterRequest;
    QString m_folder = "all";
    QString m_sortString = "";
    std::unique_ptr<rapidfuzz::fuzz::CachedRatio<unsigned int>> m_filterScorer;
    std::vector<QString> m_tags;
    SortRole m_sortRole = SortRole::RecentlyAdded;
};

}  // namespace adapters::data_models
