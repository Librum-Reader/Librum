#pragma once
#include <QObject>
#include <QString>
#include "adapters_export.hpp"
#include "i_book_controller.hpp"
#include "i_book_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT BookController : public IBookController
{
    Q_OBJECT

public:
    BookController(application::IBookService* bookService);

    void setUp(QString uuid) override;
    mupdf::FzDocument* getFzDocument() override;

private:
    application::IBookService* m_bookService;
};

}  // namespace adapters::controllers