#pragma once
#include <QObject>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QPixmap>
#include <QUrl>
#include <QUuid>
#include "book_dto.hpp"
#include "library_model.hpp"


namespace adapters
{

class IBookController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::LibraryModel* libraryModel READ getLibraryModel CONSTANT)
    Q_PROPERTY(int bookCount READ getBookCount NOTIFY bookCountChanged)
    
public:
    virtual ~IBookController() noexcept = default;
    
    enum class MetaProperties
    {
        Title = 0,
        Author,
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
    Q_ENUM(MetaProperties);
    
    
    Q_INVOKABLE virtual int addBook(const QString& path) = 0;
    Q_INVOKABLE virtual int deleteBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int updateBook(const QString& uuid, 
                                       const QVariant& operations) = 0;
    Q_INVOKABLE virtual int addTag(const QString& uuid, 
                                   const QString& tagName) = 0;
    Q_INVOKABLE virtual int removeTag(const QString& uuid,
                                      const QString& tagName) = 0;
    Q_INVOKABLE virtual adapters::dtos::BookDto getBook(const QString& uuid) = 0;
    Q_INVOKABLE virtual int getBookCount() const = 0;
    
    Q_INVOKABLE virtual int saveBookToPath(const QString& uuid, 
                                           const QUrl& path) = 0;
    virtual data_models::LibraryModel* getLibraryModel() = 0;
    
public slots:
    Q_INVOKABLE virtual void refreshLastOpenedFlag(const QString& uuid) = 0;

signals:
    void bookCountChanged();
};

} // namespace adapters