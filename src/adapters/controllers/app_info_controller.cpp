#include "app_info_controller.hpp"
#include <QDir>

using namespace application;

namespace adapters::controllers
{

AppInfoController::AppInfoController(IAppInfoService* appInfoService) :
    m_appInfoService(appInfoService)
{
    connect(m_appInfoService, &IAppInfoService::newestVersionChanged,
            this, &AppInfoController::newestVersionChanged);
}

QString AppInfoController::getCurrentVersion() const
{
   return m_appInfoService->getInfo("currentVersion");
}

QString AppInfoController::getNewestVersion() const
{
   return m_appInfoService->getInfo("newestVersion");
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

QString AppInfoController::getOperatingSystem() const
{
#ifdef Q_OS_WIN
    return "WIN";
#elif Q_OS_UNIX
    return "UNIX";
#elif Q_OS_MACOS
    return "MACOS";
#endif
}

void AppInfoController::updateApplication()
{
    m_appInfoService->updateApplication();
}

} // namespace adapters::controllers