#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include "enum_utils.hpp"
#include "setting_keys.hpp"

namespace application::services
{

QString SettingsService::getSetting(SettingKeys key, SettingGroups group)
{
    QString keyName = utility::getNameForEnumValue(key);
    QString groupName = utility::getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto defaultValue = QVariant::fromValue(QString(""));
    auto result = m_settings->value(keyName, defaultValue);

    m_settings->endGroup();
    return result.toString();
}

void SettingsService::setSetting(SettingKeys key, const QVariant& value,
                                 SettingGroups group)
{
    auto groupName = utility::getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto keyName = utility::getNameForEnumValue(key);
    m_settings->setValue(keyName, value);
    m_settings->endGroup();

    emit settingChanged(key, value, group);
}

void SettingsService::resetSettingGroup(SettingGroups group)
{
    QString filePath = getDefaultSettingsFilePathForEnum(group);
    QJsonObject defaultSettings = getDefaultSettings(filePath);

    for(const auto& defaultKey : defaultSettings.keys())
    {
        auto defaultValue = defaultSettings.value(defaultKey).toString();
        auto keyAsEnum = utility::getValueForEnumName<SettingKeys>(defaultKey);
        if(keyAsEnum.has_value())
        {
            setSetting(keyAsEnum.value(), defaultValue, group);
        }
        else
        {
            qWarning() << QString("Failed converting setting-key from default "
                                  "settings file with value: %1 to an enum.")
                              .arg(defaultKey);
        }
    }
}

QString SettingsService::getSettingsFilePath()
{
    return m_settings->fileName();
}

void SettingsService::loadUserSettings(const QString& token,
                                       const QString& email)
{
    Q_UNUSED(token);
    m_userEmail = email;

    createSettings();
    loadSettings();
}

void SettingsService::clearUserData()
{
    m_userEmail.clear();
}

void SettingsService::createSettings()
{
    auto uniqueFileName = getUniqueUserHash();
    auto format = QSettings::NativeFormat;
    m_settings = std::make_unique<QSettings>(uniqueFileName, format);

    generateDefaultSettings();
}

QString SettingsService::getUniqueUserHash() const
{
    auto userHash = qHash(m_userEmail);

    return QString::number(userHash);
}

QString SettingsService::getDefaultSettingsFilePathForEnum(SettingGroups group)
{
    switch(group)
    {
    case SettingGroups::Appearance:
        return m_defaultAppearanceSettingsFilePath;
        break;
    case SettingGroups::General:
        return m_defaultGeneralSettingsFilePath;
        break;
    case SettingGroups::Shortcuts:
        return m_defaultShortcutsFilePath;
        break;
    case SettingGroups::SettingGroups_END:
        qCritical() << "Failed getting default settings file path for enum "
                       "called SettingGroups_END";
        return QString();
    }

    return QString();
}

void SettingsService::generateDefaultSettings()
{
    loadDefaultSettings(SettingGroups::Appearance,
                        m_defaultAppearanceSettingsFilePath);
    loadDefaultSettings(SettingGroups::General,
                        m_defaultGeneralSettingsFilePath);
    loadDefaultSettings(SettingGroups::Shortcuts, m_defaultShortcutsFilePath);
}

void SettingsService::loadDefaultSettings(SettingGroups group,
                                          const QString& filePath)
{
    QJsonObject defaultSettings = getDefaultSettings(filePath);

    for(const auto& defaultSettingKey : defaultSettings.keys())
    {
        // Only load default settings which dont yet exist
        if(defaultSettingAlreadyExists(defaultSettingKey, group))
            continue;

        auto defaultSettingValue =
            defaultSettings.value(defaultSettingKey).toString();

        auto keyAsEnum =
            utility::getValueForEnumName<SettingKeys>(defaultSettingKey);
        if(keyAsEnum.has_value())
        {
            setSetting(keyAsEnum.value(), defaultSettingValue, group);
        }
        else
        {
            qWarning() << QString("Failed converting setting-key from default "
                                  "settings file with value: %1 to an enum.")
                              .arg(defaultSettingKey);
        }
    }
}

bool SettingsService::defaultSettingAlreadyExists(const QString& key,
                                                  SettingGroups group)
{
    auto groupName = utility::getNameForEnumValue(group);

    m_settings->beginGroup(groupName);
    bool exists = m_settings->contains(key);
    m_settings->endGroup();

    return exists;
}

QJsonObject SettingsService::getDefaultSettings(const QString& path)
{
    QFile defaultSettingsFile(path);
    if(!defaultSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << QString("Failed opening the default settings file: %1")
                          .arg(path);
    }

    QByteArray rawJson = defaultSettingsFile.readAll();
    auto jsonDoc = QJsonDocument::fromJson(rawJson);
    return jsonDoc.object();
}

void SettingsService::loadSettings()
{
    utility::ApplicationSettings result;

    result.appearanceSettings = getSettingsForGroup(SettingGroups::Appearance);
    result.generalSettings = getSettingsForGroup(SettingGroups::General);
    result.shortcuts = getSettingsForGroup(SettingGroups::Shortcuts);

    emit settingsLoaded(result);
}

std::vector<std::pair<QString, QVariant>> SettingsService::getSettingsForGroup(
    SettingGroups group)
{
    QString groupName = utility::getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    std::vector<std::pair<QString, QVariant>> result;

    auto keys = m_settings->allKeys();
    for(auto& key : keys)
    {
        auto value = m_settings->value(key);
        if(!value.isNull())
        {
            qCritical() << QString("Failed reading setting with name: %1 and "
                                   "group: %2")
                               .arg(key, groupName);
            return {};
        }

        result.emplace_back(key, value);
    }

    m_settings->endGroup();
    return result;
}

}  // namespace application::services