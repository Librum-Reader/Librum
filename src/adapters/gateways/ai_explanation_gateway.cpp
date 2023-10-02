#include "ai_explanation_gateway.hpp"

namespace adapters::gateways
{

AiExplanationGateway::AiExplanationGateway(
    IAiExplanationAccess* aiExplanationService) :
    m_aiExplanationAccess(aiExplanationService)
{
    connect(m_aiExplanationAccess, &IAiExplanationAccess::explanationReceived,
            this, &AiExplanationGateway::explanationReady);
}

void AiExplanationGateway::getExplanation(const QString& authToken,
                                          const QString& text,
                                          const QString& mode)
{
    m_aiExplanationAccess->getExplanation(authToken, text, mode);
}

}  // namespace adapters::gateways