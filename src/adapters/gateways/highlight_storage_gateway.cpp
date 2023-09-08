#include "highlight_storage_gateway.hpp"
#include "highlight_dto.hpp"

namespace adapters::gateways
{

HighlightStorageGateway::HighlightStorageGateway(
    IHighlightStorageAccess* highlightStorageAccess) :
    m_highlightStorageAccess(highlightStorageAccess)
{
}

void HighlightStorageGateway::createHighlight(
    const QString& authToken, const QUuid& bookUuid,
    const domain::entities::Highlight& highlight)
{
    dtos::HighlightDto highlightDto {
        .uuid = highlight.getUuid().toString(QUuid::WithoutBraces),
        .pageNumber = highlight.getPageNumber(),
        .color = highlight.getColor().name(QColor::HexArgb),
        .rects = highlight.getRects(),
    };

    auto bookUuidString = bookUuid.toString(QUuid::WithoutBraces);
    m_highlightStorageAccess->createHighlight(authToken, bookUuidString,
                                              highlightDto);
}

void HighlightStorageGateway::removeHighlight(const QString& authToken,
                                              const QUuid& bookUuid,
                                              const QUuid& highlightUuid)
{
    auto bookUuidString = bookUuid.toString(QUuid::WithoutBraces);
    auto highlightUuidString = highlightUuid.toString(QUuid::WithoutBraces);
    m_highlightStorageAccess->removeHighlight(authToken, bookUuidString,
                                              highlightUuidString);
}

}  // namespace adapters::gateways