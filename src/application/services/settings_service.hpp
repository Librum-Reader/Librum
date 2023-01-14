#pragma once
#include <QObject>
#include "i_settings_service.hpp"

namespace application::services
{

class SettingsService : public ISettingsService
{
    Q_OBJECT

public:
    SettingsService();

    QString getSetting(const QString& settingName) override;
    QString setSetting(const QString& settingName,
                       const QString& value) override;

public slots:
    void loadUserSettings(const QString& token, const QString& email) override;
    void clearUserData() override;
};

}  // namespace application::services