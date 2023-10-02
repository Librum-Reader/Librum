#pragma once
#include <QByteArray>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include "i_ai_explanation_access.hpp"

namespace infrastructure::persistence
{

class AiExplanationAccess : public adapters::IAiExplanationAccess
{
    Q_OBJECT

public:
    void getExplanation(const QString& authToken, const QString& query,
                        const QString& mode) override;

private:
    QNetworkRequest createRequest(QUrl url, QString authToken) const;

    QNetworkAccessManager m_networkAccessManager;
};

}  // namespace infrastructure::persistence