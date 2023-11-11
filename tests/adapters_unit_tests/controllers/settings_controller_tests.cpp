#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QString>
#include <QVariant>
#include "enum_utils.hpp"
#include "settings_controller.hpp"


using namespace testing;
using namespace application;
using namespace adapters::controllers;
using application::utility::ApplicationSettings;

namespace tests::adapters
{

class SettingsServiceMock : public application::ISettingsService
{
public:
    MOCK_METHOD(QString, getSetting, (SettingKeys, SettingGroups), (override));
    MOCK_METHOD(void, setSetting, (SettingKeys, const QVariant&, SettingGroups),
                (override));
    MOCK_METHOD(void, resetSettingsGroupToDefault, (SettingGroups), (override));
    MOCK_METHOD(void, loadUserSettings, (const QString&, const QString&),
                (override));
    MOCK_METHOD(QString, getSettingsFilePath, (), (override));
    MOCK_METHOD(void, clearUserData, (), (override));
};

struct ASettingsController : public ::testing::Test
{
    ASettingsController()
    {
    }

    void SetUp() override
    {
        settingsController =
            std::make_unique<SettingsController>(&settingsServiceMock);
    }

    SettingsServiceMock settingsServiceMock;
    std::unique_ptr<SettingsController> settingsController;
};

TEST_F(ASettingsController, SucceedsSettingASetting)
{
    // Arrange
    int key = static_cast<int>(SettingKeys::DefaultZoom);
    QVariant value = 30;
    int group = static_cast<int>(SettingGroups::Appearance);

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _, _)).Times(1);


    // Act
    settingsController->setSetting(key, value, group);
}

TEST_F(ASettingsController, FailsSettingASettingIfKeyIsAboveBound)
{
    // Arrange
    int key = static_cast<int>(SettingKeys::SettingKeys_END) + 1;
    QVariant value = 30;
    int group = static_cast<int>(SettingGroups::Appearance);

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _, _)).Times(0);


    // Act
    settingsController->setSetting(key, value, group);
}

TEST_F(ASettingsController, FailsSettingASettingIfKeyIsBelowBound)
{
    // Arrange
    int key = -1;
    QVariant value = 30;
    int group = static_cast<int>(SettingGroups::Appearance);

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _, _)).Times(0);


    // Act
    settingsController->setSetting(key, value, group);
}

TEST_F(ASettingsController, FailsSettingASettingIfGroupIsAboveBound)
{
    // Arrange
    int key = static_cast<int>(SettingKeys::DefaultZoom);
    QVariant value = 30;
    int group = static_cast<int>(SettingGroups::SettingGroups_END) + 1;

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _, _)).Times(0);


    // Act
    settingsController->setSetting(key, value, group);
}

TEST_F(ASettingsController, FailsSettingASettingIfGroupIsBelowBound)
{
    // Arrange
    int key = static_cast<int>(SettingKeys::DefaultZoom);
    QVariant value = 30;
    int group = static_cast<int>(SettingGroups::SettingGroups_END) + 1;

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _, _)).Times(0);


    // Act
    settingsController->setSetting(key, value, group);
}

TEST_F(ASettingsController, SucceedsResettingSettingGroup)
{
    // Arrange
    int group = static_cast<int>(SettingGroups::Appearance);

    // Expect
    EXPECT_CALL(settingsServiceMock, resetSettingsGroupToDefault(_)).Times(1);


    // Act
    settingsController->resetSettingGroup(group);
}

TEST_F(ASettingsController, FailsResettingSettingGroupIfGroupIsInvalid)
{
    // Arrange
    int invalidGroup = 981;

    // Expect
    EXPECT_CALL(settingsServiceMock, resetSettingsGroupToDefault(_)).Times(0);


    // Act
    settingsController->resetSettingGroup(invalidGroup);
}

TEST_F(ASettingsController, SuceedsUpdatingThePropertyMapWhenValueHasChanged)
{
    // Arrange
    auto key = SettingKeys::PageSpacing;
    QString keyName = utility::getNameForEnumValue(key);
    auto value = QVariant::fromValue(25);
    auto group = SettingGroups::Appearance;

    // Act
    // This calls a slot on the settings controller which updates the
    // corresponding property maps
    emit settingsServiceMock.settingChanged(key, value, group);

    // Assert
    auto appearanceMap = settingsController->getAppearanceSettings();
    EXPECT_EQ(value.toInt(), (*appearanceMap)[keyName].toInt());
}

TEST_F(ASettingsController, SuceedsInitialisingPropertyMaps)
{
    // Arrange
    auto pageSpacing = utility::getNameForEnumValue(SettingKeys::PageSpacing);
    auto pageSpacingValue = QVariant::fromValue(25);

    auto upKey = utility::getNameForEnumValue(SettingKeys::MoveUp);
    auto upKeyValue = QVariant::fromValue(QString("ALT+4"));

    // App settings sent to initalize the QQmlPropertymaps with
    ApplicationSettings settings {
        {
            { pageSpacing, pageSpacingValue },
        },
        {},
        {
            { upKey, upKeyValue },
        },
    };

    // Act
    // This calls a slot on the settings controller that causes the property
    // maps to be stored
    emit settingsServiceMock.settingsLoaded(settings);


    // Assert
    auto appearanceMap = settingsController->getAppearanceSettings();
    EXPECT_EQ(pageSpacingValue, appearanceMap->value("PageSpacing").toInt());

    auto shortcutsMap = settingsController->getShortcuts();
    EXPECT_EQ(upKeyValue, shortcutsMap->value("MoveUp").toString());
}

TEST_F(ASettingsController, SucceedsCheckingIfAShortcutIsAlreadyInUse)
{
    // Arrange
    QString shortcutToCheck = "Ctrl+A";
    auto shortcut1 = utility::getNameForEnumValue(SettingKeys::AddBook);
    auto shortcut1Value = QVariant::fromValue(shortcutToCheck);

    auto shortcut2 = utility::getNameForEnumValue(SettingKeys::ZoomIn);
    auto shortcut2Value = QVariant::fromValue(QString("Ctrl+B"));

    // App settings sent to initalize the QQmlPropertymaps with
    ApplicationSettings settings {
        {},
        {},
        {
            { shortcut1, shortcut1Value },
            { shortcut2, shortcut2Value },
        },
    };

    // Make sure the are stored
    emit settingsServiceMock.settingsLoaded(settings);

    QString expectedResult = shortcut1;


    // Act
    auto result = settingsController->checkIfShortcutIsInUse(shortcutToCheck);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST_F(ASettingsController, FailsCheckingIfAShortcutIsAlreadyInUseIfItIsntUsed)
{
    // Arrange
    QString unusedShortcut = "Ctrl+C";
    auto shortcut1 = utility::getNameForEnumValue(SettingKeys::AddBook);
    auto shortcut1Value = QVariant::fromValue(QString("Ctrl+A"));

    auto shortcut2 = utility::getNameForEnumValue(SettingKeys::ZoomIn);
    auto shortcut2Value = QVariant::fromValue(QString("Ctrl+B"));

    // App settings sent to initalize the QQmlPropertymaps with
    ApplicationSettings settings {
        {},
        {},
        {
            { shortcut1, shortcut1Value },
            { shortcut2, shortcut2Value },
        },
    };

    // Make sure the are stored
    emit settingsServiceMock.settingsLoaded(settings);

    QString expectedResult = "";


    // Act
    auto result = settingsController->checkIfShortcutIsInUse(unusedShortcut);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

}  // namespace tests::adapters
