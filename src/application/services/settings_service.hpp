#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QSettings>
#include <memory>
#include <optional>
#include "application_export.hpp"
#include "i_settings_service.hpp"

namespace application::services
{

// A wrapper class around QSettings which manages the application settings
class APPLICATION_EXPORT SettingsService : public ISettingsService
{
    Q_OBJECT

public:
    QString getSetting(SettingKeys key, SettingGroups group) override;
    void setSetting(SettingKeys key, const QVariant& value,
                    SettingGroups group) override;
    void resetSettingsGroupToDefault(SettingGroups group) override;
    QString getSettingsFilePath() override;

public slots:
    void loadUserSettings(const QString& token, const QString& email) override;
    void clearUserData() override;

private:
    void setupSettings();

    void generateDefaultSettings();
    void loadDefaultSettingsGroup(SettingGroups group);
    QJsonObject getDefaultSettingsForGroup(SettingGroups group);
    QString getDefaultSettingsFilePathForEnum(SettingGroups group);
    bool defaultSettingAlreadyExists(const QString& key, SettingGroups group);

    void loadSettings();
    std::vector<std::pair<QString, QVariant>> getSettingsForGroup(
        SettingGroups group);

    std::unique_ptr<QSettings> m_settings;
    QString m_defaultAppearanceSettingsFilePath =
        ":/resources/data/default_appearance_settings.json";
    QString m_defaultBehaviorSettingsFilePath =
        ":/resources/data/default_behavior_settings.json";
    QString m_defaultShortcutsFilePath =
        ":/resources/data/default_shortcuts.json";
    QString m_userEmail;
    QString m_authToken;
};

}  // namespace application::services
