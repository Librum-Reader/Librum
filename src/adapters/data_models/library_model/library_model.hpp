#pragma once
#include <QAbstractListModel>
#include <QByteArray>
#include <QUuid>
#include <QVariant>
#include <vector>
#include "book.hpp"
#include "tag.hpp"
#include "tag_dto.hpp"

namespace adapters::data_models
{

/**
 * The LibraryModel class provides access to the library for the UI
 */
class LibraryModel : public QAbstractListModel
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
        AddedToLibraryRole,
        LastOpenedRole,
        CoverRole,
        TagsRole,
        DownloadedRole,
        Invalid
    };

    LibraryModel(const std::vector<domain::models::Book>& data);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();
    void processBookCover(int index);
    void refreshTags(int row);
    void refreshBook(int row);

private:
    QList<dtos::TagDto> convertTagsToDtos(
        const std::vector<domain::models::Tag>& tags) const;
    const QString dateTimeFormat = "hh:mm:ss - dd.MM.yyyy";

    const std::vector<domain::models::Book>& m_data;
};

}  // namespace adapters::data_models