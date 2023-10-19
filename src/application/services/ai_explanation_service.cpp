#include "ai_explanation_service.hpp"
#include "error_code.hpp"

namespace application::services
{

AiExplanationService::AiExplanationService(
    IAiExplanationGateway* aiExplanationGateway) :
    m_aiExplanationGateway(aiExplanationGateway)
{
    connect(m_aiExplanationGateway, &IAiExplanationGateway::wordReady, this,
            &AiExplanationService::wordReady);

    connect(
        m_aiExplanationGateway, &IAiExplanationGateway::errorOccured, this,
        [this](int code)
        {
            if(code == static_cast<int>(
                           error_codes::ErrorCode::AiExplanationLimitReached))
            {
                emit limitReached();
            }
            else if(code ==
                    static_cast<int>(error_codes::ErrorCode::AiRequestTooLong))
            {
                emit requestTooLong();
            }
        });
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