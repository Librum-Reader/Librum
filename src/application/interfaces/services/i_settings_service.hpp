#pragma once
#include <QObject>
#include <QString>

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

    virtual QString getSetting(const QString& settingName) = 0;
    virtual QString setSetting(const QString& settingName,
                               const QString& value) = 0;

public slots:
    virtual void loadUserSettings(const QString& token,
                                  const QString& email) = 0;
    virtual void clearUserData() = 0;
};

}  // namespace application