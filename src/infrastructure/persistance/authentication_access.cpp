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

bool AuthenticationAccess::checkForErrors(int expectedStatusCode)
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();

    int statusCode =
        m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != expectedStatusCode)
    {
        qDebug() << "there was an error! " << m_reply->readAll();
        return true;
    }

    return false;
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
    if(checkForErrors(expectedStatusCode))
    {
        emit authenticationFinished("");
        return;
    }

    QString result = m_reply->readAll();
    emit authenticationFinished(result);
}

void AuthenticationAccess::proccessRegistrationResult()
{
    QString message = m_reply->readAll();
    auto expectedStatusCode = 201;
    if(checkForErrors(expectedStatusCode))
    {
        emit registrationFinished(false, message);
        return;
    }

    emit registrationFinished(true, "");
}

}  // namespace infrastructure::persistence