#include "authentication_access.hpp"


using namespace adapters::dtos;


namespace infrastructure::persistence
{

AuthenticationAccess::AuthenticationAccess()
    : m_authenticationEndpoint("https://localhost:7084/api/login"),
      m_registrationEndpoint("https://localhost:7084/api/register")
{
}


void AuthenticationAccess::authenticateUser(const LoginDto& loginDto)
{
    auto request = createRequest(m_authenticationEndpoint);
        
    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    
    connect(m_reply.get(), &QNetworkReply::finished, 
            this, &AuthenticationAccess::proccessAuthenticationResult);
}

void AuthenticationAccess::registerUser(const RegisterDto& registerDto)
{
    auto request = createRequest(m_registrationEndpoint);
    
    QJsonObject jsonObject;
    jsonObject["firstName"] = registerDto.firstName;
    jsonObject["lastName"] = registerDto.lastName;
    jsonObject["email"] = registerDto.email;
    jsonObject["password"] = registerDto.password;
    
    QJsonDocument jsonDocument{jsonObject};
    QByteArray data = jsonDocument.toJson();
    
    
    m_reply.reset(m_networkAccessManager.post(request, data));
    
    connect(m_reply.get(), &QNetworkReply::finished,
            this, &AuthenticationAccess::proccessRegistrationResult);
}


bool AuthenticationAccess::checkForErrors(int expectedStatusCode)
{
    if(m_reply->error() != QNetworkReply::NoError)
        qDebug() << "there was an error! " << m_reply->errorString();
    
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(statusCode != expectedStatusCode)
    {
        qDebug() << "there was an error! " << m_reply->readAll();
        return true;
    }
    
    return false;
}

QNetworkRequest AuthenticationAccess::createRequest(QUrl url)
{
    QNetworkRequest result{ url };
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
    auto expectedStatusCode = 201;
    if(checkForErrors(expectedStatusCode))
    {
        QString reason = m_reply->readAll();
        emit registrationFinished(false, reason);
        return;
    }
    
    emit registrationFinished(true, "");
}

} // namespace infrastructure::persistence