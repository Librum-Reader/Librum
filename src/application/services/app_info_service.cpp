#include "app_info_service.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

namespace application::services
{

AppInfoService::AppInfoService(IAppInfoGateway *appInfoGateway)
    : m_appInfoGateway(appInfoGateway)
{
    connect(m_appInfoGateway, &IAppInfoGateway::newestAppVersionReceived,
            this, &AppInfoService::newestAppVersionReceived);
}

QString AppInfoService::getInfo(const QString &key)
{
    QFile infoFile(m_infoFilePath);
    if(!infoFile.open(QIODevice::ReadOnly))
    {
        qWarning() << QString("Failed opening app info file at: %1")
                          .arg(m_infoFilePath);
        return "";
    }

    auto jsonDoc = QJsonDocument::fromJson(infoFile.readAll());
    auto jsonObject = jsonDoc.object();

    auto value = jsonObject[key].toString();
    if(value.isEmpty())
    {
        qWarning() << QString("Failed getting invalid app info key: %1")
                          .arg(key);
        return "";
    }

    return value;
}

void AppInfoService::getNewestAppVersion()
{
    m_appInfoGateway->getNewestAppVersion();
}

}  // namespace application::services