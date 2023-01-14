#include "settings_controller.hpp"

using namespace application;

namespace adapters::controllers
{

SettingsController::SettingsController(ISettingsService* settingsService) :
    m_settingsService(settingsService)
{
}

QString SettingsController::getSetting(const QString& settingName)
{
    return m_settingsService->getSetting(settingName);
}

void SettingsController::setSetting(const QString& settingName,
                                    const QString& value)
{
    m_settingsService->setSetting(settingName, value);
}

}  // namespace adapters::controllers