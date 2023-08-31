#include "app_info_controller.hpp"

using namespace application;

namespace adapters::controllers
{

AppInfoController::AppInfoController(IAppInfoService* appInfoService) :
    m_appInfoService(appInfoService)
{
    connect(m_appInfoService, &IAppInfoService::newestAppVersionReceived,
            this, &AppInfoController::setNewestVersion);

    m_appInfoService->getNewestAppVersion();
}

void AppInfoController::setNewestVersion(const QString& newestVersion)
{
    m_newestVersion = newestVersion;
    emit newestVersionChanged();
}

QString AppInfoController::getCurrentVersion() const
{
   return m_appInfoService->getInfo("currentVersion");
}

QString AppInfoController::getNewestVersion() const
{
   return m_newestVersion;
}

QString AppInfoController::getApplicationName() const
{
    return m_appInfoService->getInfo("applicationName");
}

QString AppInfoController::getCompanyName() const
{
    return m_appInfoService->getInfo("companyName");
}

QString AppInfoController::getWebsite() const
{
    return m_appInfoService->getInfo("website");
}

QString AppInfoController::getNewsWebsite() const
{
    return m_appInfoService->getInfo("newsWebsite");
}

QString AppInfoController::getCompanyEmail() const
{
    return m_appInfoService->getInfo("companyEmail");
}

QString AppInfoController::getGithubLink() const
{
    return m_appInfoService->getInfo("githubLink");
}

QString AppInfoController::getCurrentQtVersion() const
{
    return qVersion();
}

} // namespace adapters::controllers