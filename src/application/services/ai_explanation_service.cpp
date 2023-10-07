#include "ai_explanation_service.hpp"

namespace application::services
{

AiExplanationService::AiExplanationService(
    IAiExplanationGateway* aiExplanationGateway) :
    m_aiExplanationGateway(aiExplanationGateway)
{
    connect(m_aiExplanationGateway, &IAiExplanationGateway::wordReady,
            this, &AiExplanationService::wordReady);
}

void AiExplanationService::getExplanation(const QString& text,
                                          const QString& mode)
{
    m_aiExplanationGateway->getExplanation(m_authenticationToken, text, mode);
}

void AiExplanationService::setupUserData(const QString& token,
                                         const QString& email)
{
    Q_UNUSED(email);

    m_authenticationToken = token;
}

void AiExplanationService::clearUserData()
{
    m_authenticationToken.clear();
}

}  // namespace application::services