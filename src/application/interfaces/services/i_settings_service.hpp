#pragma once
#include <QObject>
#include <QVariant>
#include "setting_groups.hpp"
#include "setting_keys.hpp"

namespace application
{

/**
 *  The SettingsService handles storing and reading application settings.
 */
class ISettingsService : public QObject
{
    Q_OBJECT

public:
    virtual ~ISettingsService() noexcept = default;

    virtual QString getSetting(SettingKeys key, SettingGroups group) = 0;
    virtual void setSetting(SettingKeys key, const QVariant& value,
                            SettingGroups group) = 0;
    virtual void clearSettings() = 0;

public slots:
    virtual void loadUserSettings(const QString& token,
                                  const QString& email) = 0;
    virtual void clearUserData() = 0;
};

}  // namespace application