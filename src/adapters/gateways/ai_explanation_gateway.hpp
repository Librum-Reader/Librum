#pragma once
#include <QObject>
#include "i_ai_explanation_access.hpp"
#include "i_ai_explanation_gateway.hpp"

namespace adapters::gateways
{

class AiExplanationGateway : public application::IAiExplanationGateway
{
    Q_OBJECT

public:
    AiExplanationGateway(IAiExplanationAccess* aiExplanationService);

    void getExplanation(const QString& authToken, const QString& text,
                        const QString& mode) override;

private:
    IAiExplanationAccess* m_aiExplanationAccess;
};

}  // namespace adapters::gateways