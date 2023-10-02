#include "ai_explanation_controller.hpp"

namespace adapters::controllers
{

AiExplanationController::AiExplanationController(
    application::IAiExplanationService* m_aiExplanationService) :
    m_aiExplanationService(m_aiExplanationService)
{
    connect(m_aiExplanationService,
            &application::IAiExplanationService::explanationReady, this,
            &AiExplanationController::explanationReady);
}

void AiExplanationController::getExplanation(const QString& text,
                                             const QString& mode)
{
    m_aiExplanationService->getExplanation(text, mode);
}

}  // namespace adapters::controllers