#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QSettings>
#include <memory>
#include "i_settings_service.hpp"
#include "i_settings_storage_gateway.hpp"

namespace application::services
{

class SettingsService : public ISettingsService
{
    Q_OBJECT

public:
    SettingsService(ISettingsStorageGateway* settingsStorageGateway);

    QString getSetting(const QString& settingName) override;
    void setSetting(const QString& settingName, const QString& value) override;

public slots:
    void loadUserSettings(const QString& token, const QString& email) override;
    void clearUserData() override;

private slots:
    void mergeSettings(const QByteArray& data);

private:
    void createSettings();
    QString getUniqueUserHash() const;
    void generateDefaultSettings();
    QJsonObject getDefaultSettings();
    QByteArray getSettingsAsBytes();

    ISettingsStorageGateway* m_settingsStorageGateway;
    std::unique_ptr<QSettings> m_settings;
    QString m_defaultSettingsFile = ":/resources/data/default_settings.json";
    QString m_userEmail;
    QString m_authToken;
};

}  // namespace application::services