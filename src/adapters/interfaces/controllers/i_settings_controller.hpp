#include <QObject>
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

    Q_INVOKABLE virtual QString getSetting(const QString& settingName,
                                           const QString& group = "") = 0;
    Q_INVOKABLE virtual void setSetting(const QString& settingName,
                                        const QString& value,
                                        const QString& group = "") = 0;
};

}  // namespace adapters