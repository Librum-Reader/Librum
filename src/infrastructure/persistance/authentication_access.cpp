#include "authentication_access.hpp"
#include "api_error_helper.hpp"
#include "endpoints.hpp"

using namespace adapters::dtos;

namespace infrastructure::persistence
{

AuthenticationAccess::AuthenticationAccess()
{
    QSettings appSettings;
    m_serverHost = appSettings.value("serverHost").toString();
}

void AuthenticationAccess::authenticateUser(const LoginDto& loginDto)
{
    auto request = createRequest(m_serverHost + data::loginEndpoint);

    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto result = m_networkAccessManager.post(request, data);

    // Handle authentication result and release the reply's memory
    connect(result, &QNetworkReply::finished, this,
            [this, result]()
            {
                if(api_error_helper::apiRequestFailed(result, 200))
                {
                    auto errorCode = api_error_helper::logErrorMessage(
                        result, "Authentication");

                    emit authenticationFinished("", errorCode);
                    result->deleteLater();
                    return;
                }

                result->setReadBufferSize(1000);
                QString authenticationToken =
                    QString::fromUtf8(result->readAll());
                emit authenticationFinished(authenticationToken);
                result->deleteLater();
            });

    // Process the case of ssl errors occuring
    connect(result, &QNetworkReply::sslErrors, this,
            [result](const QList<QSslError>& sslErrors)
            {
                qWarning() << "Last SSL Error: " << result->errorString();

                for(const auto& error : sslErrors)
                {
                    qWarning() << error.errorString();
                }

                QSettings appSettings;

                if(appSettings.value("selfHosted").toString() == "true")
                    result->ignoreSslErrors();
            });

    // Process the case of network errors occuring
    connect(result, &QNetworkReply::errorOccurred, this,
            [result](QNetworkReply::NetworkError)
            {
                qWarning() << "Network Error " << result->errorString();
            });
}

void AuthenticationAccess::registerUser(const RegisterDto& registerDto)
{
    auto request = createRequest(m_serverHost + data::registerEndpoint);

    QJsonObject jsonObject;
    jsonObject["name"] = registerDto.name;
    jsonObject["email"] = registerDto.email;
    jsonObject["password"] = registerDto.password;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);

    // Handle registration result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 201))
                {
                    auto errorCode = api_error_helper::logErrorMessage(
                        reply, "Registration");

                    emit registrationFinished(errorCode);
                    reply->deleteLater();
                    return;
                }

                emit registrationFinished();
                reply->deleteLater();
            });
}

void AuthenticationAccess::checkIfEmailConfirmed(const QString& email)
{
    auto request = createRequest(
        m_serverHost + data::checkIfEmailConfirmedEndpoint + "/" + email);

    auto reply = m_networkAccessManager.get(request);

    // Handle registration result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    api_error_helper::logErrorMessage(
                        reply, "Checking if email is confirmed");

                    emit emailConfirmationCheckFinished(false);
                    reply->deleteLater();
                    return;
                }

                bool emailConfirmed =
                    reply->readAll().compare("true", Qt::CaseInsensitive) == 0;
                emit emailConfirmationCheckFinished(emailConfirmed);
                reply->deleteLater();
            });
}

QNetworkRequest AuthenticationAccess::createRequest(QUrl url)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence
