#pragma once
#include <QList>
#include <QObject>
#include <QPixmap>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QVariantMap>
#include "book_dto.hpp"
#include "library_proxy_model.hpp"

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
    Q_PROPERTY(adapters::data_models::LibraryProxyModel* libraryModel READ
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
        CurrentPage,
        AddedToLibrary,
        LastModified,
        Invalid
    };
    Q_ENUM(MetaProperty);

    Q_INVOKABLE virtual void refresh() = 0;
    Q_INVOKABLE virtual int addBook(const QString& path) = 0;
    Q_INVOKABLE virtual int deleteBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int uninstallBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int downloadBookMedia(const QString& uuid) = 0;
    Q_INVOKABLE virtual int updateBook(const QString& uuid,
                                       const QVariant& operations) = 0;
    Q_INVOKABLE virtual int changeBookCover(const QString& uuid,
                                            const QString& path) = 0;
    Q_INVOKABLE virtual int addTag(const QString& bookUuid,
                                   const QString& tagName,
                                   const QString& tagUuid) = 0;
    Q_INVOKABLE virtual int removeTag(const QString& bookUuid,
                                      const QString& tagUuid) = 0;
    Q_INVOKABLE virtual void removeAllTagsWithUuid(const QString& tagUuid) = 0;
    Q_INVOKABLE virtual void renameTags(const QString& oldName,
                                        const QString& newName) = 0;
    Q_INVOKABLE virtual adapters::dtos::BookDto getBook(
        const QString& uuid) = 0;
    Q_INVOKABLE virtual int getBookCount() const = 0;

    Q_INVOKABLE virtual int saveBookToFile(const QString& uuid,
                                           const QUrl& path) = 0;
    virtual data_models::LibraryProxyModel* getLibraryModel() = 0;

public slots:
    Q_INVOKABLE virtual void refreshLastOpenedFlag(const QString& uuid) = 0;

signals:
    void bookCountChanged();
    void storageLimitExceeded();
};

}  // namespace adapters
