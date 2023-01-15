#include "settings_storage_access.hpp"

namespace infrastructure::persistence
{

void SettingsStorageAccess::getSettings(const QString& authToken)
{
}

void SettingsStorageAccess::setSettings(const QString& authToken,
                                        const QByteArray& data)
{
}

QNetworkRequest SettingsStorageAccess::createRequest(const QUrl& url,
                                                     const QString& authToken)
{
}

bool SettingsStorageAccess::checkForErrors(int expectedStatusCode,
                                           QNetworkReply* reply)
{
}

}  // namespace infrastructure::persistence