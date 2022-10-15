#pragma once
#include <QObject>
#include <QString>
#include <QImage>
#include "book_dto.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"


namespace adapters::controllers
{

class BookController : public IBookController
{
    Q_OBJECT
    
public:
    BookController(application::IBookService* bookService);
    
    int addBook(const QString& path) override;
    int deleteBook(const QString& uuid) override;
    int updateBook(const QString& uuid,
                   const QVariant& operations) override;
    int addTag(const QString& uuid, const QString& tagName) override;
    int removeTag(const QString& uuid, const QString& tagName) override;
    dtos::BookDto getBook(const QString& uuid) override;
    int getBookCount() const override;
    
    int saveBookToPath(const QString& uuid, const QUrl& path) override;
    data_models::LibraryModel* getLibraryModel() override;
    
public slots:
    void refreshLastOpenedFlag(const QString& uuid) override;
    
private:
    void refreshBookChache();
    dtos::BookDto* getBookFromChache(const QString& uuid);
    QImage getCorrectlySizedBookCover(const QString& pathToCover);
    
    bool m_bookChacheChanged;
    std::vector<dtos::BookDto> m_bookCache;
    application::IBookService* m_bookService;
    data_models::LibraryModel m_libraryModel;
};

} // namespace adapters::controllers