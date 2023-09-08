#pragma once
#include "i_highlight_storage_access.hpp"
#include "i_highlight_storage_gateway.hpp"

namespace adapters::gateways
{

class HighlightStorageGateway : public application::IHighlightStorageGateway
{
public:
    HighlightStorageGateway(IHighlightStorageAccess* highlightStorageAccess);

    void createHighlight(const QString& authToken, const QUuid& bookUuid,
                         const domain::entities::Highlight& highlight) override;
    void removeHighlight(const QString& authToken, const QUuid& bookUuid,
                         const QUuid& highlightUuid) override;

private:
    IHighlightStorageAccess* m_highlightStorageAccess;
};

}  // namespace adapters::gateways