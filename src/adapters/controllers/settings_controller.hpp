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

    QString getSetting(int key, int group) override;
    void setSetting(int key, const QVariant& value, int group) override;

private:
    bool keyIsValid(int key);
    bool groupIsValid(int group);

    application::ISettingsService* m_settingsService;
};

}  // namespace adapters::controllers