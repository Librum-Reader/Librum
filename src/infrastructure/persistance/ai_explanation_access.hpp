#pragma once
#include <QByteArray>
#include <QDateTime>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
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
	AiExplanationAccess();					
		
private:
    QNetworkRequest createRequest(QUrl url, QString authToken) const;
    QDateTime m_lastRequestStartTime;

    QNetworkAccessManager m_networkAccessManager;
	QString domain; // server domain
};

}  // namespace infrastructure::persistence