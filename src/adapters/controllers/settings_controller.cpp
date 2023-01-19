#include "settings_controller.hpp"

using namespace application;

namespace adapters::controllers
{

SettingsController::SettingsController(ISettingsService* settingsService) :
    m_settingsService(settingsService)
{
}

QString SettingsController::getSetting(const QString& settingName,
                                       const QString& group)
{
    return m_settingsService->getSetting(settingName, group);
}

void SettingsController::setSetting(const QString& settingName,
                                    const QString& value, const QString& group)
{
    m_settingsService->setSetting(settingName, value, group);
}

}  // namespace adapters::controllers