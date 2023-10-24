#include "authentication_access.hpp"
#include "api_error_helper.hpp"
#include "endpoints.hpp"

using namespace adapters::dtos;

namespace infrastructure::persistence
{
	
AuthenticationAccess::AuthenticationAccess(){	
	QSettings settings;
	domain=settings.value("serverHost").toString();	
}
	
void AuthenticationAccess::authenticateUser(const LoginDto& loginDto)
{
    auto request = createRequest(domain + data::authenticationEndpoint);

    QJsonObject jsonObject;
    jsonObject["email"] = loginDto.email;
    jsonObject["password"] = loginDto.password;

    QJsonDocument jsonDocument { jsonObject };
    QByteArray data = jsonDocument.toJson(QJsonDocument::Compact);

    auto reply = m_networkAccessManager.post(request, data);
	
	// handle ssl errors
	connect(reply, &QNetworkReply::sslErrors, this,
            [reply](const QList<QSslError> &errors) {
               qWarning()<< "Error SSL " << reply->errorString();
               for( int i=0; i<errors.count(); ++i )
                {
                     qWarning()<<errors[i].errorString();
                }
              reply->ignoreSslErrors();

     		});

   connect(reply, &QNetworkReply::errorOccurred, this,
            [reply](QNetworkReply::NetworkError) {
               qWarning()<< "Error " << reply->errorString();
		     });


    // Handle authentication result and release the reply's memory
    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
			  // qWarning() << "-----------reply from server---------------";
              reply->setReadBufferSize(1000);
              QString token=QString::fromUtf8(reply->readAll());
              //qWarning("-------------Strings read-------------------");

              // something wrong went with readall so using low-level read
//                char* buffer=new char[1000];
//                reply->read(buffer,1000);
//                qWarning("-------------Strings read-------------------");
//                qWarning(buffer);
//                QString token=QString::fromUtf8(buffer);
//                delete[] buffer;
				
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
					qWarning() << "-----------api_error_helper::apiRequestFailed---------------";
                    auto errorCode = api_error_helper::logErrorMessage(
                        reply, "Authentication");

                    emit authenticationFinished("", errorCode);
                    reply->deleteLater();
                    return;
                }

                emit authenticationFinished(token);
                reply->deleteLater();
            });
}

void AuthenticationAccess::registerUser(const RegisterDto& registerDto)
{
    auto request = createRequest(domain + data::registrationEndpoint);

    QJsonObject jsonObject;
    jsonObject["firstName"] = registerDto.firstName;
    jsonObject["lastName"] = registerDto.lastName;
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
    auto request =
        createRequest(domain + data::checkIfEmailConfirmedEndpoint + "/" + email);

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