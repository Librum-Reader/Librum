#include <QObject>
#include <QQmlPropertyMap>
#include <QStandardItemModel>
#include <QVariant>
#include "shortcuts_model/shortcuts_proxy_model.hpp"
#pragma once

namespace adapters
{

/**
 * The SettingsController class is exposed to the UI code and thus is the
 * "entry point" to the settings' backend for reading and writing settings.
 *
 * To create an easy integration with QML, the SettingsController exposes
 * QQmlPropertMaps via which QML can directly access the settings.
 * The QQmlProperty maps need to be updated manually though, so everytime a
 * setting was changed, the SettingsController needs to update the corresponding
 * map. This is done by receiving a signal from the SettingsService that is
 * connected to a slot, which then updates the map.
 *
 * The initialisation of the QQmlPropertyMaps is also done via a signal sent
 * from the SettingsService. Once it has gathered the properties to initialise
 * the maps with, it sends a signal up to the SettingsController, which then
 * uses the data sent to initialise the property maps.
 */
class ISettingsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        QQmlPropertyMap* appearanceSettings READ getAppearanceSettings CONSTANT)
    Q_PROPERTY(
        QQmlPropertyMap* generalSettings READ getGeneralSettings CONSTANT)
    Q_PROPERTY(QQmlPropertyMap* shortcuts READ getShortcuts CONSTANT)
    Q_PROPERTY(adapters::data_models::ShortcutsProxyModel* shortcutsModel READ
                   getShortcutsModel CONSTANT)


public:
    virtual ~ISettingsController() noexcept = default;

    Q_INVOKABLE virtual void setSetting(int key, const QVariant& value,
                                        int group) = 0;
    Q_INVOKABLE virtual void setSetting(QString key, const QVariant& value,
                                        int group) = 0;
    Q_INVOKABLE virtual void resetSettingGroup(int group) = 0;

    virtual QQmlPropertyMap* getAppearanceSettings() = 0;
    virtual QQmlPropertyMap* getGeneralSettings() = 0;
    virtual QQmlPropertyMap* getShortcuts() = 0;
    virtual data_models::ShortcutsProxyModel* getShortcutsModel() = 0;
};

}  // namespace adapters