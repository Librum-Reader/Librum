#pragma once
#include <QObject>
#include "i_app_info_service.hpp"
#include "i_app_info_gateway.hpp"

namespace application::services
{

class AppInfoService : public IAppInfoService
{
    Q_OBJECT

public:
    AppInfoService(IAppInfoGateway* appInfoGateway);

    QString getInfo(const QString &key) override;
    void getNewestAppVersion() override;

private:
    QString m_infoFilePath = ":/app_info.json";

    IAppInfoGateway* m_appInfoGateway;
};

}  // namespace application::services