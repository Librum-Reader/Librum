#include "authentication_access.hpp"
#include "endpoints.hpp"


using namespace adapters::dtos;

namespace infrastructure::persistence
{

void AuthenticationAccess::authenticateUser(const LoginDto& loginDto)
{
    auto request = createRequest(data::authenticationEndpoint);

    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);


    m_reply.reset(m_networkAccessManager.post(request, data));

    connect(m_reply.get(), &QNetworkReply::finished, this,
            &AuthenticationAccess::proccessAuthenticationResult);
}

void AuthenticationAccess::registerUser(const RegisterDto& registerDto)
{
    auto request = createRequest(data::registrationEndpoint);

    QJsonObject jsonObject;
    jsonObject["firstName"] = registerDto.firstName;
    jsonObject["lastName"] = registerDto.lastName;
    jsonObject["email"] = registerDto.email;
    jsonObject["password"] = registerDto.password;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);


    m_reply.reset(m_networkAccessManager.post(request, data));

    connect(m_reply.get(), &QNetworkReply::finished, this,
            &AuthenticationAccess::proccessRegistrationResult);
}

ServerReplyStatus AuthenticationAccess::validateServerReply(
    int expectedStatusCode)
{
    int statusCode =
        m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    auto replyHasError = m_reply->error() != QNetworkReply::NoError;
    if(replyHasError || statusCode != expectedStatusCode)
    {
        QString errorMessage = m_reply->readAll();
        qWarning() << QString("Authentication error: %1 \nServer replied: %2")
                          .arg(m_reply->errorString(), errorMessage);

        return ServerReplyStatus {
            .success = false,
            .errorMessage = errorMessage,
        };
    }

    return ServerReplyStatus { .success = true };
}

QNetworkRequest AuthenticationAccess::createRequest(QUrl url)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    result.setRawHeader("X-Version", "1.0");

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

void AuthenticationAccess::proccessAuthenticationResult()
{
    auto expectedStatusCode = 200;
    auto replyStatus = validateServerReply(expectedStatusCode);
    if(!replyStatus.success)
    {
        emit authenticationFinished("");
        return;
    }

    emit authenticationFinished(m_reply->readAll());
}

void AuthenticationAccess::proccessRegistrationResult()
{
    auto expectedStatusCode = 201;
    auto replyStatus = validateServerReply(expectedStatusCode);
    if(!replyStatus.success)
    {
        emit registrationFinished(false, replyStatus.errorMessage);
        return;
    }

    emit registrationFinished(true, "");
}

}  // namespace infrastructure::persistence