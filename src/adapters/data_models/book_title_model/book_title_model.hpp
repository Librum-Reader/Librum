#pragma once
#include <QAbstractListModel>
#include <QByteArray>
#include <QList>
#include <QUuid>
#include <QVariant>
#include <vector>
#include "adapters_export.hpp"
#include "book.hpp"

namespace adapters::data_models
{

/**
 * The BookTitleModel class provides access to the titles of books in the user's
 * library.
 */
class ADAPTERS_EXPORT BookTitleModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        UuidRole = Qt::UserRole,
        TitleRole,
        DownloadedRole,
        FilePathRole,
        ExtensionRole,
    };

    explicit BookTitleModel(const std::vector<domain::entities::Book>& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();
    void refreshBook(int row);

private:
    const std::vector<domain::entities::Book>& m_data;
};

}  // namespace adapters::data_models
