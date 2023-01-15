#pragma once
#include <QObject>
#include "i_settings_storage_access.hpp"
#include "i_settings_storage_gateway.hpp"

namespace adapters::gateways
{

class SettingsStorageGateway : public application::ISettingsStorageGateway
{
    Q_OBJECT

public:
    SettingsStorageGateway(ISettingsStorageAccess* settingsStorageAccess);

    void getSettings(const QString& authToken) override;
    void setSettings(const QString& authToken, const QByteArray& data) override;

private:
    ISettingsStorageAccess* m_settingsStorageAccess;
};

}  // namespace adapters::gateways