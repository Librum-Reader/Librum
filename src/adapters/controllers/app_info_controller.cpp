#include "app_info_controller.hpp"
#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QSettings>

using namespace application;

namespace adapters::controllers
{

AppInfoController::AppInfoController(IAppInfoService* appInfoService) :
    m_appInfoService(appInfoService)
{
    connect(m_appInfoService, &IAppInfoService::newestVersionChanged, this,
            &AppInfoController::newestVersionChanged);

    connect(m_appInfoService,
            &IAppInfoService::downloadingBinariesProgressChanged, this,
            &AppInfoController::downloadingBinariesProgressChanged);

    connect(m_appInfoService, &IAppInfoService::applicationUpdateFailed, this,
            &AppInfoController::applicaitonUpdateFailed);


    // Setup network info
    auto success = QNetworkInformation::loadDefaultBackend();
    if(!success)
        qWarning() << "Failed loading QNetworkInformation backend";

    m_networkInfo = QNetworkInformation::instance();
    if(m_networkInfo == nullptr)
    {
        qWarning() << "Failed loading QNetworkInformation instance";
    }
    else
    {
        connect(m_networkInfo, &QNetworkInformation::reachabilityChanged, this,
                &AppInfoController::isOnlineChanged);
    }
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

QString AppInfoController::getFeedbackLink() const
{
    return m_appInfoService->getInfo("feedbackLink");
}

QString AppInfoController::getDiscord() const
{
    return m_appInfoService->getInfo("discord");
}

QString AppInfoController::getCurrentQtVersion() const
{
    return qVersion();
}

QString AppInfoController::getOperatingSystem() const
{
#ifdef Q_OS_WIN
    return "WIN";
#elif defined(Q_OS_MAC)
    return "MACOS";
#else
    return "UNIX";
#endif
}

void AppInfoController::updateApplication()
{
    m_appInfoService->updateApplication();
}

double AppInfoController::getSystemFontSize() const
{
    auto os = getOperatingSystem();
    if(os == "MACOS")
        return 12.5;

    return 9;
}

bool AppInfoController::isOnline() const
{
    if(m_networkInfo == nullptr)
        return true;

    if(m_networkInfo->reachability() ==
       QNetworkInformation::Reachability::Online)
    {
        return true;
    }

    return false;
}

QString AppInfoController::getLanguage() const
{
    return m_language;
}

bool AppInfoController::switchToLanguage(const QString& language)
{
    if(!m_translator.isEmpty())
        QGuiApplication::removeTranslator(&m_translator);

    auto success = m_translator.load(":/i18n/librum_" + language);
    if(!success)
    {
        qWarning() << "Failed switching to language: " << language;
        return false;
    }

    // The english translation is empty since it is the default language. Thus
    // it causes an error when trying to switch to english. Ignore it.
    if(!QGuiApplication::installTranslator(&m_translator) && language != "en")
    {
        qWarning() << "Failed installing translator for language: " << language;
        return false;
    }

    // Use "English" for all kinds of English variants like American English
    if(QLocale(language).language() == QLocale::Language::English)
        m_language = "English";
    // Use "Bahasa Indonesia" instead of "Indonesian"
    else if(QLocale(language).language() == QLocale::Language::Indonesian)
        m_language = "Bahasa Indonesia";
    else
        m_language = QLocale(language).nativeLanguageName();


    QSettings settings;
    settings.setValue("language", language);

    m_engine->retranslate();
    emit languageChanged();
    return true;
}

void AppInfoController::setQmlApplicationEngine(QQmlApplicationEngine* engine)
{
    m_engine = engine;
}

}  // namespace adapters::controllers
