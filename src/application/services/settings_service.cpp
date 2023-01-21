#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>

namespace application::services
{

QString SettingsService::getSetting(const QString& settingName,
                                    const QString& group)
{
    if(!m_settingsAreValid)
        return "";

    if(!group.isEmpty())
        m_settings->beginGroup(group);

    auto defaultValue = QVariant::fromValue(QString(""));
    auto result =
        m_settings->value(settingName.toLower(), defaultValue).toString();

    if(!group.isEmpty())
        m_settings->endGroup();

    return result;
}

void SettingsService::setSetting(const QString& settingName,
                                 const QString& value, const QString& group)
{
    if(!m_settingsAreValid)
        return;

    if(!group.isEmpty())
        m_settings->beginGroup(group);

    auto valueToSet = QVariant::fromValue(value.toLower());
    m_settings->setValue(settingName.toLower(), valueToSet);

    if(!group.isEmpty())
        m_settings->endGroup();
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
    loadDefaultSettings("Appearance", m_defaultAppearanceSettingsFilePath);
    loadDefaultSettings("General", m_defaultGeneralSettingsFilePath);
    loadDefaultSettings("Shortcuts", m_defaultShortcutsFilePath);
}

void SettingsService::loadDefaultSettings(const QString& group,
                                          const QString& filePath)
{
    QJsonObject defaultSettings = getDefaultSettings(filePath);

    m_settings->beginGroup(group);
    for(const auto& settingKey : defaultSettings.keys())
    {
        if(m_settings->contains(settingKey))
            continue;

        auto settingValue = defaultSettings.value(settingKey).toString();
        setSetting(settingKey, settingValue);
    }
    m_settings->endGroup();
}

QJsonObject SettingsService::getDefaultSettings(const QString& path)
{
    QFile defaultSettingsFile(path);
    if(!defaultSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open default settings file: " << path << "!";
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

}  // namespace application::services