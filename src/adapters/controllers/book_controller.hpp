#pragma once
#include <QImage>
#include <QObject>
#include <QString>
#include "book_dto.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"
#include "i_user_service.hpp"
#include "library_model.hpp"

namespace adapters::controllers
{

class BookController : public IBookController
{
    Q_OBJECT

public:
    BookController(application::IBookService* bookService,
                   application::IUserService* userService);

    int addBook(const QString& path) override;
    int deleteBook(const QString& uuid) override;
    int uninstallBook(const QString& uuid) override;
    int updateBook(const QString& uuid, const QVariant& operations) override;
    int addTag(const QString& uuid, const QString& tagName) override;
    int removeTag(const QString& bookUuid, const QString& tagUuid) override;
    dtos::BookDto getBook(const QString& uuid) override;
    int getBookCount() const override;

    int saveBookToFile(const QString& uuid, const QUrl& path) override;
    data_models::LibraryProxyModel* getLibraryModel() override;

public slots:
    void refreshLastOpenedFlag(const QString& uuid) override;

private:
    void refreshBookChache();
    dtos::BookDto* getBookFromChache(const QString& uuid);
    QImage getCorrectlySizedBookCover(const QString& pathToCover);
    dtos::BookDto getDtoFromBook(const domain::models::Book& book);

    application::IBookService* m_bookService;
    application::IUserService* m_userService;  // For access to user tags
    data_models::LibraryModel m_libraryModel;
    data_models::LibraryProxyModel m_libraryProxyModel;

    // A chache containing BookDto's
    std::vector<dtos::BookDto> m_bookCache;
    bool m_bookChacheChanged;
};

}  // namespace adapters::controllers