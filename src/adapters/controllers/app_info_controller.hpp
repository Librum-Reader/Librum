#pragma once
#include <QNetworkInformation>
#include <QObject>
#include <QTimer>
#include <QTranslator>
#include "adapters_export.hpp"
#include "i_app_info_controller.hpp"
#include "i_app_info_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT AppInfoController : public IAppInfoController
{
    Q_OBJECT

public:
    AppInfoController(application::IAppInfoService* appInfoService);

    bool switchToLanguage(const QString& language) override;
    void setQmlApplicationEngine(QQmlApplicationEngine* engine) override;

private:
    QString getCurrentVersion() const override;
    QString getNewestVersion() const override;
    QString getApplicationName() const override;
    QString getCompanyName() const override;
    QString getWebsite() const override;
    QString getNewsWebsite() const override;
    QString getCompanyEmail() const override;
    QString getGithubLink() const override;
    QString getFeedbackLink() const override;
    QString getCurrentQtVersion() const override;
    QString getOperatingSystem() const override;
    void updateApplication() override;
    double getSystemFontSize() const override;
    bool isOnline() const override;
    QString getLanguage() const override;

    application::IAppInfoService* m_appInfoService;
    QNetworkInformation* m_networkInfo;
    QTranslator m_translator;
    QQmlApplicationEngine* m_engine;
    QString m_language;
};

}  // namespace adapters::controllers
