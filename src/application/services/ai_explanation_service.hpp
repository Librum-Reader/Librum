#pragma once
#include <QDir>
#include <QObject>
#include "i_ai_explanation_gateway.hpp"
#include "i_ai_explanation_service.hpp"

namespace application::services
{

class AiExplanationService : public IAiExplanationService
{
    Q_OBJECT

public:
    AiExplanationService(IAiExplanationGateway* aiExplanationGateway);

    void getExplanation(const QString& text, const QString& mode) override;

public slots:
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;

private:
    IAiExplanationGateway* m_aiExplanationGateway;

    QString m_authenticationToken;
};

}  // namespace application::services