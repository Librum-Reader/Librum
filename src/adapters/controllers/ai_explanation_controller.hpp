#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_ai_explanation_controller.hpp"
#include "i_ai_explanation_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT AiExplanationController : public IAiExplanationController
{
    Q_OBJECT

public:
    AiExplanationController(
        application::IAiExplanationService* m_aiExplanationService);

    void getExplanation(const QString& text) override;

private:
    application::IAiExplanationService* m_aiExplanationService;
};

}  // namespace adapters::controllers