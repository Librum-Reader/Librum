#include "settings_storage_gateway.hpp"

namespace adapters::gateways
{

SettingsStorageGateway::SettingsStorageGateway(
    ISettingsStorageAccess* settingsStorageAccess) :
    m_settingsStorageAccess(settingsStorageAccess)
{
}

void SettingsStorageGateway::getSettings(const QString& authToken)
{
    m_settingsStorageAccess->getSettings(authToken);
}

void SettingsStorageGateway::setSettings(const QString& authToken,
                                         const QByteArray& data)
{
    m_settingsStorageAccess->setSettings(authToken, data);
}

}  // namespace adapters::gateways