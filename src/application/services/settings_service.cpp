#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>

namespace application::services
{

SettingsService::SettingsService(
    ISettingsStorageGateway* settingsStorageGateway) :
    m_settingsStorageGateway(settingsStorageGateway)
{
    connect(m_settingsStorageGateway,
            &ISettingsStorageGateway::finishedGettingSettings, this,
            &SettingsService::mergeSettings);
}

QString SettingsService::getSetting(const QString& settingName)
{
    if(!m_settingsAreValid)
        return "";

    auto defaultValue = QVariant::fromValue(QString(""));
    return m_settings->value(settingName.toLower(), defaultValue).toString();
}

void SettingsService::setSetting(const QString& settingName,
                                 const QString& value)
{
    if(!m_settingsAreValid)
        return;

    auto valueToSet = QVariant::fromValue(value.toLower());
    m_settings->setValue(settingName.toLower(), valueToSet);

    m_settingsStorageGateway->updateSettings(m_authToken, getSettingsAsBytes());
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

void SettingsService::mergeSettings(const QByteArray& data, bool success)
{
    if(!success)
    {
        qWarning() << "Getting settings from server failed!";
        return;
    }

    if(getSettingsAsBytes() != data)
    {
        m_settings->clear();
        writeDataToSettingsFile(data);
    }
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
    QJsonObject defaultSettings = getDefaultSettings();
    for(const auto& settingKey : defaultSettings.keys())
    {
        if(m_settings->contains(settingKey))
            continue;

        auto settingValue = defaultSettings.value(settingKey).toString();
        setSetting(settingKey, settingValue);
    }
}

QJsonObject SettingsService::getDefaultSettings()
{
    QFile defaultSettingsFile(m_defaultSettingsFile);
    if(!defaultSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open default settings file!";
    }

    QByteArray rawJson = defaultSettingsFile.readAll();
    auto jsonDoc = QJsonDocument::fromJson(rawJson);
    return jsonDoc.object();
}

QByteArray SettingsService::getSettingsAsBytes()
{
    QFile settingsFile(m_settings->fileName());
    if(!settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open settings file!";
    }

    return settingsFile.readAll();
}

bool SettingsService::settingsAreValid()
{
    // If the underlying file has been deleted by "clear()", its invalid
    return QFile::exists(m_settings->fileName());
}

void SettingsService::writeDataToSettingsFile(const QByteArray& data)
{
    QFile settingsFile(m_settings->fileName());
    if(!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open settings file!";
    }

    settingsFile.write(data);
}

}  // namespace application::services