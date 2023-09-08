#include "app_info_gateway.hpp"

using namespace application;

namespace adapters::gateways
{

AppInfoGateway::AppInfoGateway(IAppInfoAccess *appInfoAccess) :
    m_appInfoAccess(appInfoAccess)
{
    connect(m_appInfoAccess, &IAppInfoAccess::newestAppVersionReceived,
            this, &AppInfoGateway::newestAppVersionReceived);

    connect(m_appInfoAccess, &IAppInfoAccess::downloadingBinariesFinished,
            this, &AppInfoGateway::downloadingBinariesFinished);

    connect(m_appInfoAccess, &IAppInfoAccess::downloadingBinariesProgressChanged,
            this, &AppInfoGateway::downloadingBinariesProgressChanged);
}

void adapters::gateways::AppInfoGateway::getNewestAppVersion()
{
    m_appInfoAccess->getNewestAppVersion();
}

void AppInfoGateway::downloadBinaries(const QString &packageName)
{
    m_appInfoAccess->downloadBinaries(packageName);
}

} // adapters::gateways