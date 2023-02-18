#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include "enum_utils.hpp"
#include "setting_keys.hpp"

using namespace application::utility;
using SettingsVector = std::vector<std::pair<QString, QVariant>>;

namespace application::services
{

QString SettingsService::getSetting(SettingKeys key, SettingGroups group)
{
    QString keyName = getNameForEnumValue(key);
    QString groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto defaultValue = QVariant::fromValue(QString(""));
    auto result = m_settings->value(keyName, defaultValue);

    if(result == defaultValue)
    {
        qWarning() << QString("Requested Setting with key: %1 does not exist.")
                          .arg(keyName);
    }

    m_settings->endGroup();
    return result.toString();
}

void SettingsService::setSetting(SettingKeys key, const QVariant& value,
                                 SettingGroups group)
{
    auto groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto keyName = getNameForEnumValue(key);
    m_settings->setValue(keyName, value);
    m_settings->endGroup();

    emit settingChanged(key, value, group);
}

void SettingsService::resetSettingsGroupToDefault(SettingGroups group)
{
    auto defaultSettings = getDefaultSettingsForGroup(group);
    for(const auto& defaultSettingKey : defaultSettings.keys())
    {
        auto defaultValue = defaultSettings.value(defaultSettingKey).toString();
        auto keyAsEnum = getValueForEnumName<SettingKeys>(defaultSettingKey);
        if(keyAsEnum.has_value())
        {
            setSetting(keyAsEnum.value(), defaultValue, group);
        }
        else
        {
            qWarning() << QString("Failed converting setting-key from default "
                                  "settings file with value: %1 to an enum.")
                              .arg(defaultSettingKey);
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

    setupSettings();
    loadSettings();
}

void SettingsService::clearUserData()
{
    m_userEmail.clear();
}

void SettingsService::setupSettings()
{
    // Setup the QSettings object
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
    }

    return QString();
}

void SettingsService::generateDefaultSettings()
{
    loadDefaultSettingsGroup(SettingGroups::Appearance);
    loadDefaultSettingsGroup(SettingGroups::General);
    loadDefaultSettingsGroup(SettingGroups::Shortcuts);
}

void SettingsService::loadDefaultSettingsGroup(SettingGroups group)
{
    auto defaultSettings = getDefaultSettingsForGroup(group);
    if(defaultSettings.isEmpty())
    {
        qCritical() << QString("Failed loading default settings for group: %1")
                           .arg(getNameForEnumValue(group));
    }

    for(const auto& defaultSettingKey : defaultSettings.keys())
    {
        // Default settings are only loaded for settings which don't yet exist.
        // E.g. First login, no settings exist, so default values are loaded.
        // Thus skip all settings which already exist.
        if(defaultSettingAlreadyExists(defaultSettingKey, group))
            continue;

        auto defaultValue = defaultSettings.value(defaultSettingKey).toString();
        auto keyAsEnum = getValueForEnumName<SettingKeys>(defaultSettingKey);
        if(keyAsEnum.has_value())
        {
            setSetting(keyAsEnum.value(), defaultValue, group);
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
    auto groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    bool exists = m_settings->contains(key);

    m_settings->endGroup();
    return exists;
}

QJsonObject SettingsService::getDefaultSettingsForGroup(SettingGroups group)
{
    QString defaultSettingsFilePath = getDefaultSettingsFilePathForEnum(group);
    QFile defaultSettingsFile(defaultSettingsFilePath);
    if(!defaultSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << QString("Failed opening the default settings file: %1")
                          .arg(defaultSettingsFilePath);
        return {};
    }

    QByteArray rawJson = defaultSettingsFile.readAll();
    auto jsonDoc = QJsonDocument::fromJson(rawJson);
    return jsonDoc.object();
}

void SettingsService::loadSettings()
{
    ApplicationSettings result;

    result.appearanceSettings = getSettingsForGroup(SettingGroups::Appearance);
    result.generalSettings = getSettingsForGroup(SettingGroups::General);
    result.shortcuts = getSettingsForGroup(SettingGroups::Shortcuts);

    emit settingsLoaded(result);
}

SettingsVector SettingsService::getSettingsForGroup(SettingGroups group)
{
    SettingsVector result;
    QString groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto keys = m_settings->allKeys();
    for(auto& key : keys)
    {
        auto value = m_settings->value(key);
        if(value.isNull())
        {
            qWarning() << QString("Failed reading setting with name: %1 and "
                                  "group: %2")
                              .arg(key, groupName);
            m_settings->endGroup();
            return {};
        }

        result.emplace_back(key, value);
    }

    m_settings->endGroup();
    return result;
}

}  // namespace application::services