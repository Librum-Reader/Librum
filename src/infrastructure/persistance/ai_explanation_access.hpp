#pragma once
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSettings>
#include "i_ai_explanation_access.hpp"

namespace infrastructure::persistence
{

class AiExplanationAccess : public adapters::IAiExplanationAccess
{
    Q_OBJECT

public:
    AiExplanationAccess();

    void getExplanation(const QString& authToken, const QString& query,
                        const QString& mode) override;

private:
    QNetworkRequest createRequest(QUrl url, QString authToken) const;
    QDateTime m_lastRequestStartTime;

    QNetworkAccessManager m_networkAccessManager;
    QString domain;
};

}  // namespace infrastructure::persistence
