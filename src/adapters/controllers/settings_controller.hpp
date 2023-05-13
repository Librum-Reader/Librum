#pragma once
#include <QObject>
#include <QVariant>
#include <memory>
#include "application_settings.hpp"
#include "i_settings_controller.hpp"
#include "i_settings_service.hpp"
#include "setting_groups.hpp"
#include "setting_keys.hpp"

namespace adapters::controllers
{

class SettingsController : public ISettingsController
{
    Q_OBJECT

public:
    SettingsController(application::ISettingsService* settingsService);

    void setSetting(int key, const QVariant& value, int group) override;
    void setSetting(QString key, const QVariant& value, int group) override;
    void resetSettingGroup(int group) override;

    QQmlPropertyMap* getAppearanceSettings() override;
    QQmlPropertyMap* getGeneralSettings() override;
    QQmlPropertyMap* getShortcuts() override;
    data_models::ShortcutsModel* getShortcutsModel() override;

private slots:
    void updateChangedSetting(application::setting_keys::SettingKeys key,
                              QVariant value,
                              application::setting_groups::SettingGroups group);
    void initialiseSettings(application::utility::ApplicationSettings settings);

private:
    bool keyIsValid(int key);
    bool groupIsValid(int group);

    QQmlPropertyMap m_appearanceSettingsMap;
    QQmlPropertyMap m_generalSettingsMap;
    QQmlPropertyMap m_shortcutsMap;
    std::unique_ptr<data_models::ShortcutsModel> m_shortcutsModel;
    application::ISettingsService* m_settingsService;
};

}  // namespace adapters::controllers