#pragma once
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>

// A group of utility functions to make handling API errors easy.
namespace infrastructure::persistence::api_error_helper
{

// Private namespace
namespace
{

QString getJsonMessageFromReply(const QByteArray& content)
{
    auto jsonObj = QJsonDocument::fromJson(content).object();

    return jsonObj["message"].toString();
}

QString getStatusCodeFromReply(const QNetworkReply* reply)
{
    auto statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    return QString::number(statusCode);
}

int getErrorCode(const QByteArray& content)
{
    auto jsonObj = QJsonDocument::fromJson(content).object();

    return jsonObj["code"].toInt();
}

}  // namespace

// Public interface
struct ApiError
{
    int errorCode;
    QString errorMessage;
};

inline bool apiRequestFailed(const QNetworkReply* reply, int expectedStatusCode)
{
    bool apiError = reply->error() != QNetworkReply::NoError;
    auto statusCode =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    return apiError || statusCode != expectedStatusCode;
}

inline int logErrorMessage(QNetworkReply* reply, QString actionName)
{
    QString statusCode = getStatusCodeFromReply(reply);

    QString apiError = reply->errorString();
    QByteArray replyContent = reply->readAll();
    QString jsonMessage = getJsonMessageFromReply(replyContent);

    QString errorMessage = actionName + " failed (" + statusCode + "): ";
    if(!jsonMessage.isEmpty())
        errorMessage += jsonMessage;
    else if(!replyContent.isEmpty())
        errorMessage += replyContent;
    else
        errorMessage += apiError;

    qWarning() << errorMessage;

    return getErrorCode(replyContent);
}

}  // namespace infrastructure::persistence::api_error_helper