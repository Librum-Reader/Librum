#include "settings_storage_gateway.hpp"

namespace adapters::gateways
{

SettingsStorageGateway::SettingsStorageGateway(
    ISettingsStorageAccess* settingsStorageAccess) :
    m_settingsStorageAccess(settingsStorageAccess)
{
    connect(m_settingsStorageAccess,
            &ISettingsStorageAccess::finishedGettingSettings, this,
            &SettingsStorageGateway::finishedGettingSettings);
}

void SettingsStorageGateway::getSettings(const QString& authToken)
{
    m_settingsStorageAccess->getSettings(authToken);
}

void SettingsStorageGateway::updateSettings(const QString& authToken,
                                            const QByteArray& data)
{
    m_settingsStorageAccess->updateSettings(authToken, data);
}

}  // namespace adapters::gateways