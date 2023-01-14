#pragma once
#include <QObject>
#include <memory>
#include "i_settings_controller.hpp"
#include "i_settings_service.hpp"

namespace adapters::controllers
{

class SettingsController : public ISettingsController
{
    Q_OBJECT

public:
    SettingsController(application::ISettingsService* settingsService);

    QString getSetting(const QString& settingName) override;
    void setSetting(const QString& settingName, const QString& value) override;

private:
    std::unique_ptr<application::ISettingsService> m_settingsService;
};

}  // namespace adapters::controllers