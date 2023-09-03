#pragma once
#include <QObject>
#include "i_app_info_access.hpp"
#include "i_app_info_gateway.hpp"

namespace adapters::gateways
{

class AppInfoGateway : public application::IAppInfoGateway
{
    Q_OBJECT

public:
    AppInfoGateway(IAppInfoAccess* appInfoService);

    void getNewestAppVersion();
    void downloadBinaries(const QString &packageName);

private:
    IAppInfoAccess* m_appInfoAccess;
};

} // adapters::gateways