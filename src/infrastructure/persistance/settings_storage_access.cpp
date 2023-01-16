#include "settings_storage_access.hpp"

namespace infrastructure::persistence
{

void SettingsStorageAccess::getSettings(const QString& authToken)
{
    Q_UNUSED(authToken);
}

void SettingsStorageAccess::updateSettings(const QString& authToken,
                                           const QByteArray& data)
{
    Q_UNUSED(authToken);
    Q_UNUSED(data);
}

}  // namespace infrastructure::persistence