#include "folder_storage_access.hpp"
#include <QNetworkReply>
#include <QSettings>
#include "api_error_helper.hpp"
#include "endpoints.hpp"
#include "error_code.hpp"

namespace infrastructure::persistence
{

using application::error_codes::ErrorCode;

FolderStorageAccess::FolderStorageAccess()
{
    QSettings settings;
    domain = settings.value("serverHost").toString();
}

void FolderStorageAccess::updateFolder(const QString& authToken,
                                       const QByteArray& folderJson)
{
    auto request =
        createRequest(domain + data::folderUpdateEndpoint, authToken);
    auto reply = m_networkAccessManager.post(request, folderJson);

    connect(reply, &QNetworkReply::finished,
            [reply]()
            {
                reply->deleteLater();
            });
}

void FolderStorageAccess::fetchFolders(const QString& authToken)
{
    auto request = createRequest(domain + data::folderGetEndpoint, authToken);
    auto reply = m_networkAccessManager.get(request);

    connect(reply, &QNetworkReply::finished, this,
            [this, reply]()
            {
                if(api_error_helper::apiRequestFailed(reply, 200))
                {
                    auto code = api_error_helper::logErrorMessage(
                        reply, "Fetching folders");

                    if(code == static_cast<int>(ErrorCode::UserHasNoRootFolder))
                    {
                        emit foldersFetched(QByteArray());
                    }

                    reply->deleteLater();
                    return;
                }

                auto data = reply->readAll();
                emit foldersFetched(data);
                reply->deleteLater();
            });
}

QNetworkRequest FolderStorageAccess::createRequest(QUrl url,
                                                   const QString& authToken)
{
    QNetworkRequest result { url };
    result.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if(!authToken.isEmpty())
    {
        result.setRawHeader(QByteArray("Authorization"),
                            "Bearer " + authToken.toUtf8());
    }

    QSslConfiguration sslConfiguration = result.sslConfiguration();
    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
    result.setSslConfiguration(sslConfiguration);

    return result;
}

}  // namespace infrastructure::persistence