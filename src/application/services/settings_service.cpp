#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>
#include "setting_keys.hpp"

namespace application::services
{

QString SettingsService::getSetting(SettingKeys key, SettingGroups group)
{
    if(!m_settingsAreValid)
        return "";

    QString keyName = getNameForEnumValue(key);
    QString groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto defaultValue = QVariant::fromValue(QString(""));
    auto result = m_settings->value(keyName, defaultValue);

    m_settings->endGroup();
    return result.toString();
}

void SettingsService::setSetting(SettingKeys key, const QVariant& value,
                                 SettingGroups group)
{
    if(!m_settingsAreValid)
        return;

    auto groupName = getNameForEnumValue(group);
    m_settings->beginGroup(groupName);

    auto keyName = getNameForEnumValue(key);
    m_settings->setValue(keyName, value);

    m_settings->endGroup();
}

void SettingsService::resetSettingGroup(SettingGroups group)
{
    if(!m_settingsAreValid)
        return;

    QString defaultSettingFilePath;
    switch(group)
    {
    case SettingGroups::Appearance:
        defaultSettingFilePath = m_defaultAppearanceSettingsFilePath;
        break;
    case SettingGroups::General:
        defaultSettingFilePath = m_defaultGeneralSettingsFilePath;
        break;
    case SettingGroups::Shortcuts:
        defaultSettingFilePath = m_defaultShortcutsFilePath;
        break;
    case SettingGroups::SettingGroups_END:
        qWarning() << "Called with invalid parameter";
        return;
    }

    QJsonObject defaultSettings = getDefaultSettings(defaultSettingFilePath);
    for(const auto& defaultSettingKey : defaultSettings.keys())
    {
        auto defaultSettingValue =
            defaultSettings.value(defaultSettingKey).toString();

        auto keyAsEnum = getValueForEnumName<SettingKeys>(defaultSettingKey);
        if(keyAsEnum.has_value())
        {
            setSetting(keyAsEnum.value(), defaultSettingValue, group);
        }
        else
        {
            qWarning() << QString("Failed converting setting-key from default "
                                  "settings file with value: %1 to an enum.")
                              .arg(defaultSettingKey);
            continue;
        }
    }
}

void SettingsService::clearSettings()
{
    m_settings->sync();
    QString filePath = m_settings->fileName();
    QFile::remove(filePath);

    m_settingsAreValid = false;
}

void SettingsService::loadUserSettings(const QString& token,
                                       const QString& email)
{
    Q_UNUSED(token);
    m_userEmail = email;

    createSettings();
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
    m_settingsAreValid = true;

    generateDefaultSettings();
}

QString SettingsService::getUniqueUserHash() const
{
    auto userHash = qHash(m_userEmail);

    return QString::number(userHash);
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

        auto keyAsEnum = getValueForEnumName<SettingKeys>(defaultSettingKey);
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
    auto groupName = getNameForEnumValue(group);

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

bool SettingsService::settingsAreValid()
{
    // If the underlying file has been deleted by "clear()", its invalid
    return QFile::exists(m_settings->fileName());
}

template<typename Enum>
QString SettingsService::getNameForEnumValue(Enum value) const
{
    QMetaEnum meta = QMetaEnum::fromType<Enum>();
    auto text = meta.valueToKey(static_cast<int>(value));

    return QString::fromLatin1(text);
}

template<typename Enum>
std::optional<Enum> SettingsService::getValueForEnumName(
    const QString& name) const
{
    QMetaEnum meta = QMetaEnum::fromType<Enum>();
    auto key = meta.keyToValue(name.toLatin1());
    if(key == -1)
        return std::nullopt;

    return static_cast<Enum>(key);
}

// explicit template instantiations
template QString SettingsService::getNameForEnumValue<SettingKeys>(
    SettingKeys) const;
template QString SettingsService::getNameForEnumValue<SettingGroups>(
    SettingGroups) const;

template std::optional<SettingKeys>
    SettingsService::getValueForEnumName<SettingKeys>(const QString&) const;

}  // namespace application::services