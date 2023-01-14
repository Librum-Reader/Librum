#include "settings_service.hpp"
#include <QDebug>
#include <QFile>
#include <QHash>

namespace application::services
{

SettingsService::SettingsService()
{
}

QString SettingsService::getSetting(const QString& settingName)
{
    auto defaultValue = QVariant::fromValue(QString(""));
    return m_settings->value(settingName.toLower(), defaultValue).toString();
}

void SettingsService::setSetting(const QString& settingName,
                                 const QString& value)
{
    auto dataToSet = QVariant::fromValue(value.toLower());
    m_settings->setValue(settingName.toLower(), dataToSet);
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

    if(m_settings->allKeys().isEmpty())
        generateDefaultSettings();
}

QString SettingsService::getUniqueUserHash() const
{
    auto userHash = qHash(m_userEmail);

    return QString::number(userHash);
}

void SettingsService::generateDefaultSettings()
{
    QFile defaultSettingsFile(":/resources/data/default_settings.json");
    if(!defaultSettingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Failed to open default settings file!";
    }

    qDebug() << defaultSettingsFile.readAll();
}

}  // namespace application::services