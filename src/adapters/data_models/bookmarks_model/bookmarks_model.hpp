#pragma once
#include <QAbstractListModel>
#include <QList>
#include <bookmark.hpp>
#include "adapters_export.hpp"

namespace adapters::data_models
{

/**
 * The BookmarksModel represents the Bookmarks to the UI
 */
class ADAPTERS_EXPORT BookmarksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        UuidRole = Qt::UserRole,
        NameRole,
        PageNumberRole,
        YOffsetRole
    };

    BookmarksModel(const QList<domain::entities::Bookmark>& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingBookmark(int index);
    void endInsertingBookmark();
    void startDeletingBookmark(int index);
    void endDeletingBookmark();
    void bookmarkNameChanged(int row);

private:
    const QList<domain::entities::Bookmark> m_data;
};

}  // namespace adapters::data_models