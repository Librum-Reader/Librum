#include "book_service.hpp"
#include "settings.hpp"
#include "document.h"


namespace application::services
{

BookService::BookService()
{
    
}

void BookService::uploadBook(QString path)
{
    Q_UNUSED(path)
}

void BookService::openBook(QString path)
{
    Q_UNUSED(path)
    Okular::Settings::instance(QStringLiteral("okularproviderrc"));
    auto doc = new Okular::Document(nullptr);
    delete doc;
}

} // namespace application::services