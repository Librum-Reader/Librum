#include <QObject>
#include <QQmlPropertyMap>
#include <QVariant>
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
    Q_PROPERTY(
        QQmlPropertyMap* appearanceSettings READ getAppearanceSettings CONSTANT)
    Q_PROPERTY(
        QQmlPropertyMap* generalSettings READ getGeneralSettings CONSTANT)
    Q_PROPERTY(QQmlPropertyMap* shortcuts READ getShortcuts CONSTANT)


public:
    virtual ~ISettingsController() noexcept = default;

    Q_INVOKABLE virtual void setSetting(int key, const QVariant& value,
                                        int group) = 0;
    Q_INVOKABLE virtual void resetSettingGroup(int group) = 0;

    virtual QQmlPropertyMap* getAppearanceSettings() = 0;
    virtual QQmlPropertyMap* getGeneralSettings() = 0;
    virtual QQmlPropertyMap* getShortcuts() = 0;
};

}  // namespace adapters