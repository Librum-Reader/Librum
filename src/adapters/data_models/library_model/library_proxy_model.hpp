#pragma once
#include <QSortFilterProxyModel>
#include "library_model.hpp"

namespace adapters::data_models
{

/**
 * The LibraryProxyModel class is a proxy class which acts as a wrapper around
 * the library model that is handling the sorting and filtering of elements
 */
class LibraryProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int bookCount READ getBookCount NOTIFY bookCountChanged)
    Q_PROPERTY(
        QString sortString READ getSortString WRITE setSortString CONSTANT)
    Q_PROPERTY(int sortRole READ getSortRole WRITE setSortRole CONSTANT)
    Q_PROPERTY(QString filterAuthors READ getFilterAuthors WRITE
                   setFilterAuthors CONSTANT)
    Q_PROPERTY(QString filterFormat READ getFilterFormat WRITE setFilterFormat
                   CONSTANT)
    Q_PROPERTY(bool filterForOnlyFiles READ getFilterForOnlyFiles WRITE
                   setFilterForOnlyFiles CONSTANT)
    Q_PROPERTY(bool filterForOnlyBooks READ getFilterForOnlyBooks WRITE
                   setFilterForOnlyBooks CONSTANT)

public:
    enum SortRole
    {
        RecentlyAdded = Qt::UserRole + 1,
        Title,
        Authors,
        LastOpened
    };

    Q_ENUM(SortRole);

    explicit LibraryProxyModel(QObject* parent = nullptr);

    bool lessThan(const QModelIndex& left,
                  const QModelIndex& right) const override;
    bool filterAcceptsRow(int source_row,
                          const QModelIndex& source_parent) const override;

    int getBookCount() const;

    void setSortRole(int newRole);
    int getSortRole();

    void setSortString(QString newSortString);
    QString getSortString();

    QString getFilterAuthors() const;
    void setFilterAuthors(const QString& newFilterAuthor);

    QString getFilterFormat() const;
    void setFilterFormat(const QString& newFilterFormat);

    bool getFilterForOnlyFiles() const;
    void setFilterForOnlyFiles(bool newFilterForOnlyFiles);

    bool getFilterForOnlyBooks() const;
    void setFilterForOnlyBooks(bool newFilterForOnlyBooks);

signals:
    void sortStringUpdated();
    void bookCountChanged();

private:
    double fuzzCompareWithSortingString(QString lhs) const;
    bool stringIsLexicographicallyLess(const QString& left,
                                       const QString& right) const;
    bool openedAfter(const QModelIndex& left, const QModelIndex& right) const;
    bool addedToLibraryAfter(const QModelIndex& left,
                             const QModelIndex& right) const;

    LibraryModel* m_libraryModel;
    QString m_sortString = "";
    SortRole m_sortRole = SortRole::RecentlyAdded;
    QString m_filterAuthors = "";
    QString m_filterFormat = "";
    bool m_filterForOnlyFiles = false;
    bool m_filterForOnlyBooks = false;
};

}  // namespace adapters::data_models