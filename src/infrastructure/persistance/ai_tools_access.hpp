#pragma once
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSettings>
#include "i_ai_tools_access.hpp"

namespace infrastructure::persistence
{

class AiToolsAccess : public adapters::IAiToolsAccess
{
    Q_OBJECT

public:
    AiToolsAccess();

    void getExplanation(const QString& authToken, const QString& query,
                        const QString& mode) override;
    void getTranslation(const QString& authToken, const QString& text,
                        const QString& sourceLang,
                        const QString& targetLang) override;

private:
    QNetworkRequest createRequest(QUrl url, QString authToken) const;
    QDateTime m_lastRequestStartTime;

    QNetworkAccessManager m_networkAccessManager;
    QString m_serverHost;
};

}  // namespace infrastructure::persistence
