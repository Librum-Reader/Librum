#include "settings_controller.hpp"
#include <QDebug>
#include <setting_groups.hpp>
#include <setting_keys.hpp>

using namespace application;

namespace adapters::controllers
{

SettingsController::SettingsController(ISettingsService* settingsService) :
    m_settingsService(settingsService)
{
}

QString SettingsController::getSetting(int key, int group)
{
    if(!keyIsValid(key) || !groupIsValid(group))
        return "";

    auto keyAsEnum = static_cast<SettingKeys>(key);
    auto groupAsEnum = static_cast<SettingGroups>(group);
    return m_settingsService->getSetting(keyAsEnum, groupAsEnum);
}

void SettingsController::setSetting(int key, const QVariant& value, int group)
{
    if(!keyIsValid(key) || !groupIsValid(group))
        return;

    auto keyAsEnum = static_cast<SettingKeys>(key);
    auto groupAsEnum = static_cast<SettingGroups>(group);
    return m_settingsService->setSetting(keyAsEnum, value, groupAsEnum);
}

bool SettingsController::keyIsValid(int key)
{
    int settingKeysEnd = static_cast<int>(SettingKeys::SettingKeys_END);
    if(key >= settingKeysEnd || key < 0)
    {
        qWarning() << Q_FUNC_INFO << "Invalid setting key: " << key;
        return false;
    }

    return true;
}

bool SettingsController::groupIsValid(int group)
{
    int settingGroupsEnd = static_cast<int>(SettingGroups::SettingGroups_END);
    if(group >= settingGroupsEnd || group < 0)
    {
        qWarning() << Q_FUNC_INFO << "Invalid setting group: " << group;
        return false;
    }

    return true;
}

}  // namespace adapters::controllers