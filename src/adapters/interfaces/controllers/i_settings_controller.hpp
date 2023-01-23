#include <QObject>
#include <QVariant>
#include "setting_groups.hpp"
#include "setting_keys.hpp"
#pragma once

namespace adapters
{

/**
 * The SettingsController class is exposed to the UI code and thus is the
 * "entry point" to the settings' backend for reading and writing settings.
 */
class ISettingsController : public QObject
{
    Q_OBJECT

public:
    virtual ~ISettingsController() noexcept = default;

    Q_INVOKABLE virtual QString getSetting(int key, int group) = 0;
    Q_INVOKABLE virtual void setSetting(int key, const QVariant& value,
                                        int group) = 0;
    Q_INVOKABLE virtual void resetSettingGroup(int group) = 0;

signals:
    void reload();
};

}  // namespace adapters