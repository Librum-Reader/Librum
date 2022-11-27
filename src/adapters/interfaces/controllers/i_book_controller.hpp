#pragma once
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QVariantMap>
#include "book_dto.hpp"
#include "library_sort_filter_model.hpp"

namespace adapters
{

/**
 * The BookController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for book operations. It acts as a
 * layer of abstraction which maps the user data to a format usable for the
 * application.
 */
class IBookController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::LibrarySortFilterModel* libraryModel READ
                   getLibraryModel CONSTANT)
    Q_PROPERTY(int bookCount READ getBookCount NOTIFY bookCountChanged)

public:
    virtual ~IBookController() noexcept = default;

    enum class MetaProperty
    {
        Title = 0,
        Authors,
        FilePath,
        Creator,
        CreationDate,
        Format,
        Language,
        DocumentSize,
        PagesSize,
        PageCount,
        AddedToLibrary,
        LastModified,
        Cover,
        Invalid
    };
    Q_ENUM(MetaProperty);


    Q_INVOKABLE virtual int addBook(const QString& path) = 0;
    Q_INVOKABLE virtual int deleteBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int uninstallBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int updateBook(const QString& uuid,
                                       const QVariant& operations) = 0;
    Q_INVOKABLE virtual int addTag(const QString& uuid,
                                   const QString& tagName) = 0;
    Q_INVOKABLE virtual int removeTag(const QString& uuid,
                                      const QString& tagName) = 0;
    Q_INVOKABLE virtual adapters::dtos::BookDto getBook(
        const QString& uuid) = 0;
    Q_INVOKABLE virtual int getBookCount() const = 0;

    Q_INVOKABLE virtual int saveBookToFile(const QString& uuid,
                                           const QUrl& path) = 0;
    virtual data_models::LibrarySortFilterModel* getLibraryModel() = 0;

public slots:
    Q_INVOKABLE virtual void refreshLastOpenedFlag(const QString& uuid) = 0;

signals:
    void bookCountChanged();
};

}  // namespace adapters