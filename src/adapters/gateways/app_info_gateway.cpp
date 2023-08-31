#include "app_info_gateway.hpp"

using namespace application;

namespace adapters::gateways
{

AppInfoGateway::AppInfoGateway(IAppInfoAccess *appInfoService) :
    m_appInfoService(appInfoService)
{
    connect(m_appInfoService, &IAppInfoAccess::newestAppVersionReceived,
            this, &AppInfoGateway::newestAppVersionReceived);
}

void adapters::gateways::AppInfoGateway::getNewestAppVersion()
{
    m_appInfoService->getNewestAppVersion();
}

} // adapters::gateways