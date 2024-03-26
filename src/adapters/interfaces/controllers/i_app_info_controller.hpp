#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include "adapters_export.hpp"

namespace adapters
{

/**
 * The AuthenticationController class is exposed to the UI code and thus is the
 * "entry point" to the application's backend for application info operations.
 * It acts as a layer of abstraction which maps the user data to a format usable
 * for the application.
 */
class ADAPTERS_EXPORT IAppInfoController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentVersion READ getCurrentVersion CONSTANT)
    Q_PROPERTY(
        QString newestVersion READ getNewestVersion NOTIFY newestVersionChanged)
    Q_PROPERTY(QString applicationName READ getApplicationName CONSTANT)
    Q_PROPERTY(QString companyName READ getCompanyName CONSTANT)
    Q_PROPERTY(QString website READ getWebsite CONSTANT)
    Q_PROPERTY(QString newsWebsite READ getNewsWebsite CONSTANT)
    Q_PROPERTY(QString companyEmail READ getCompanyEmail CONSTANT)
    Q_PROPERTY(QString githubLink READ getGithubLink CONSTANT)
    Q_PROPERTY(QString feedbackLink READ getFeedbackLink CONSTANT)
    Q_PROPERTY(QString currentQtVersion READ getCurrentQtVersion CONSTANT)
    Q_PROPERTY(QString operatingSystem READ getOperatingSystem CONSTANT)
    Q_PROPERTY(int systemFontSize READ getSystemFontSize CONSTANT)
    Q_PROPERTY(bool online READ isOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(QString language READ getLanguage NOTIFY languageChanged)

public:
    virtual ~IAppInfoController() noexcept = default;

    Q_INVOKABLE virtual void updateApplication() = 0;
    Q_INVOKABLE virtual bool switchToLanguage(const QString& language) = 0;
    Q_INVOKABLE virtual void setQmlApplicationEngine(
        QQmlApplicationEngine* engine) = 0;

private:
    virtual QString getCurrentVersion() const = 0;
    virtual QString getNewestVersion() const = 0;
    virtual QString getApplicationName() const = 0;
    virtual QString getCompanyName() const = 0;
    virtual QString getWebsite() const = 0;
    virtual QString getNewsWebsite() const = 0;
    virtual QString getCompanyEmail() const = 0;
    virtual QString getGithubLink() const = 0;
    virtual QString getFeedbackLink() const = 0;
    virtual QString getCurrentQtVersion() const = 0;
    virtual QString getOperatingSystem() const = 0;
    virtual double getSystemFontSize() const = 0;
    virtual bool isOnline() const = 0;
    virtual QString getLanguage() const = 0;

signals:
    void newestVersionChanged();
    void downloadingBinariesProgressChanged(double progress);
    void applicaitonUpdateFailed();
    void isOnlineChanged();
    void languageChanged();
};

}  // namespace adapters
