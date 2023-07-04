#pragma once
#include <QAbstractListModel>
#include <QByteArray>
#include <QVariant>
#include <vector>
#include "free_book.hpp"

namespace adapters::data_models
{

class FreeBooksModel : public QAbstractListModel
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
        Invalid
    };

    FreeBooksModel(const std::vector<domain::value_objects::FreeBook>* data);
    FreeBooksModel() = default;

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void refreshBook(int row);
    void startBookClearing();
    void endBookClearing();
    void startInsertingRow(int index);
    void endInsertingRow();
    void startDeletingBook(int index);
    void endDeletingBook();

private:
    QVector<int> getAllRoles();

    const std::vector<domain::value_objects::FreeBook>* m_data;
};

}  // namespace adapters::data_models
