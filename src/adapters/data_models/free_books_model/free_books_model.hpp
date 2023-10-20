#pragma once
#include <QAbstractListModel>
#include <QByteArray>
#include <QVariant>
#include <vector>
#include "adapters_export.hpp"
#include "free_book.hpp"

namespace adapters::data_models
{

class ADAPTERS_EXPORT FreeBooksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        AuthorsRole,
        LanguagesRole,
        FormatsRole,
        DownloadCountRole,
        CoverRole,
        MediaDownloadLink,
        MediaDownloadProgressRole,
        isDownloadedRole,
        Invalid
    };

    FreeBooksModel(const std::vector<domain::value_objects::FreeBook>* data);
    FreeBooksModel() = default;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void setApiInfo(const int booksTotalCount,
                    const QString& nextMetadataPageUrl,
                    const QString& prevMetadataPageUrl);
    void refreshBook(int row);
    void startBookClearing();
    void endBookClearing();
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();
    void downloadingBookMediaProgressChanged(int row);
    void bookIsDownloadedChanged(int row);
    void clear();

signals:
    void fetchBooksMetadataPage(const QString& url);

protected:
    bool canFetchMore(const QModelIndex& parent) const override;
    void fetchMore(const QModelIndex& parent) override;

private:
    const std::vector<domain::value_objects::FreeBook>* m_data;
    int m_booksLoadedCount = 0;
    int m_booksTotalCount = 0;
    QString m_prevMetadataPageUrl;
    QString m_nextMetadataPageUrl;

    QVector<int> getAllRoles();
};

}  // namespace adapters::data_models
