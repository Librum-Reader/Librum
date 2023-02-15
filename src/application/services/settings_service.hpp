#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QSettings>
#include <memory>
#include <optional>
#include "i_settings_service.hpp"

namespace application::services
{

class SettingsService : public ISettingsService
{
    Q_OBJECT

public:
    QString getSetting(SettingKeys key, SettingGroups group) override;
    void setSetting(SettingKeys key, const QVariant& value,
                    SettingGroups group) override;
    void resetSettingGroup(SettingGroups group) override;
    void clearSettings() override;

public slots:
    void loadUserSettings(const QString& token, const QString& email) override;
    void clearUserData() override;

private:
    void createSettings();
    void generateDefaultSettings();
    void loadDefaultSettings(SettingGroups group, const QString& filePath);
    QJsonObject getDefaultSettings(const QString& path);
    void loadSettings();
    std::vector<std::pair<QString, QVariant>> getSettingsForGroup(
        SettingGroups group);
    bool settingsAreValid();
    bool defaultSettingAlreadyExists(const QString& key, SettingGroups group);
    QString getUniqueUserHash() const;
    QString getDefaultSettingsFilePathForEnum(SettingGroups group);

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