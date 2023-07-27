#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QSettings>
#include <memory>
#include <optional>
#include "i_settings_service.hpp"
#include "application_export.hpp"

namespace application::services
{

// A wrapper class around QSettings which manages the application settings
class APPLICATION_LIBRARY SettingsService : public ISettingsService
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

    QString getUniqueUserHash() const;

    std::unique_ptr<QSettings> m_settings;
    QString m_defaultAppearanceSettingsFilePath =
        ":/resources/data/default_appearance_settings.json";
    QString m_defaultGeneralSettingsFilePath =
        ":/resources/data/default_general_settings.json";
    QString m_defaultShortcutsFilePath =
        ":/resources/data/default_shortcuts.json";
    QString m_userEmail;
    QString m_authToken;
};

}  // namespace application::services
