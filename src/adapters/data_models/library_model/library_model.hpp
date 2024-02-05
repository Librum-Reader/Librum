#pragma once
#include <QAbstractListModel>
#include <QByteArray>
#include <QList>
#include <QUuid>
#include <QVariant>
#include <vector>
#include "adapters_export.hpp"
#include "book.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"

namespace adapters::data_models
{

/**
 * The LibraryModel class provides access to the library for the UI
 */
class ADAPTERS_EXPORT LibraryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles
    {
        UuidRole = Qt::UserRole,
        TitleRole,
        AuthorsRole,
        FilePathRole,
        CreatorRole,
        CreationDateRole,
        FormatRole,
        LanguageRole,
        DocumentSizeRole,
        PagesSizeRole,
        PageCountRole,
        BookReadingProgressRole,
        CurrentPageRole,
        AddedToLibraryRole,
        LastOpenedRole,
        ParentFolderIdRole,
        CoverRole,
        TagsRole,
        DownloadedRole,
        MediaDownloadProgressRole,
        ExistsOnlyOnClientRole
    };

    explicit LibraryModel(const std::vector<domain::entities::Book>& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();
    void processBookCover(int row);
    void refreshTags(int row);
    void refreshBook(int row);
    void startBookClearing();
    void endBookClearing();
    void downloadingBookMediaProgressChanged(int row);

private:
    QList<dtos::TagDto> convertTagsToDtos(
        const QList<domain::entities::Tag>& tags) const;

    const std::vector<domain::entities::Book>& m_data;
};

}  // namespace adapters::data_models
