#include "document_item.hpp"
#include <QUrl>
#include <memory>
#include "document.hpp"

using application::core::Document;

namespace cpp_elements
{

void DocumentItem::setFilePath(const QString& newFilePath)
{
    m_document = std::make_unique<Document>(QUrl(newFilePath).path());
}

const Document* DocumentItem::internal() const
{
    return m_document.get();
}

}  // namespace cpp_elements