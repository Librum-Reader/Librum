#pragma once
#include <QByteArray>
#include <QJsonObject>
#include <QSettings>
#include <memory>
#include "i_settings_service.hpp"

namespace application::services
{

class SettingsService : public ISettingsService
{
    Q_OBJECT

public:
    QString getSetting(const QString& settingName,
                       const QString& group = "") override;
    void setSetting(const QString& settingName, const QString& value,
                    const QString& group = "") override;
    void clearSettings() override;

public slots:
    void loadUserSettings(const QString& token, const QString& email) override;
    void clearUserData() override;

private:
    void createSettings();
    QString getUniqueUserHash() const;
    void generateDefaultSettings();
    void loadDefaultSettings(const QString& group, const QString& filePath);
    QJsonObject getDefaultSettings(const QString& path);
    QByteArray getSettingsAsBytes();
    bool settingsAreValid();

    std::unique_ptr<QSettings> m_settings;
    QString m_defaultAppearanceSettingsFilePath =
        ":/resources/data/default_appearance_settings.json";
    QString m_defaultGeneralSettingsFilePath =
        ":/resources/data/default_general_settings.json";
    QString m_defaultShortcutsFilePath =
        ":/resources/data/default_shortcuts.json";
    QString m_userEmail;
    QString m_authToken;
    bool m_settingsAreValid = false;
};

}  // namespace application::services