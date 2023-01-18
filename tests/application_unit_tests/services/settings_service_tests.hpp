#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "settings_service.hpp"

using namespace testing;
using namespace application::services;

namespace tests::application
{

struct ASettingsService : public ::testing::Test
{
    void SetUp() override
    {
        settingsService = std::make_unique<SettingsService>();
        settingsService->loadUserSettings(testingEmail, "");
    }

    void TearDown() override
    {
        settingsService->clearSettings();
        settingsService->clearUserData();
    }

    std::unique_ptr<SettingsService> settingsService;

private:
    QString testingEmail = "LibrumTestEmail@librum.com.fake";
};

TEST_F(ASettingsService, SucceedsSettingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";


    // Act
    settingsService->setSetting(settingName, settingValue);
}

TEST_F(ASettingsService, SucceedsSettingASettingInAGroup)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";
    QString settingGroup = "SomeGroup";


    // Act
    settingsService->setSetting(settingName, settingValue, settingGroup);
}

TEST_F(ASettingsService, FailsSettingASettingIfSettingsAreInvalid)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";

    settingsService->clearSettings();


    // Act
    settingsService->setSetting(settingName, settingValue);
}

TEST_F(ASettingsService, SucceedsOverridingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString initialValue = "SomeValue";
    QString newValue = "SomeValue";

    settingsService->setSetting(settingName, initialValue);


    // Act
    settingsService->setSetting(settingName, newValue);

    // Assert
    auto actualValue = settingsService->getSetting(settingName);
    EXPECT_EQ(newValue.toLower(), actualValue);
}

TEST_F(ASettingsService, SucceedsGettingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";

    settingsService->setSetting(settingName, settingValue);


    // Act
    auto result = settingsService->getSetting(settingName);

    // Assert
    EXPECT_EQ(settingValue.toLower(), result);
}

TEST_F(ASettingsService, SucceedsGettingASettingFromAGroup)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";
    QString settingGroup = "SomeGroup";

    settingsService->setSetting(settingName, settingValue, settingGroup);


    // Act
    auto result = settingsService->getSetting(settingName, settingGroup);

    // Assert
    EXPECT_EQ(settingValue.toLower(), result);
}

TEST_F(ASettingsService, FailsGettingASettingIfSettingDoesNotExist)
{
    // Arrange
    QString nonExistentSettingName = "SomeSetting";


    // Act
    auto result = settingsService->getSetting(nonExistentSettingName);

    // Assert
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ASettingsService, FailsGettingASettingIfSettingIsNotInGroup)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";
    QString settingGroup = "SomeGroup";

    settingsService->setSetting(settingName, settingValue, settingGroup);


    // Act
    auto result = settingsService->getSetting(settingName, "NonExistentGroup");

    // Assert
    EXPECT_TRUE(result.isEmpty());
}

TEST_F(ASettingsService, FailsGettingASettingIfSettingsAreInvalid)
{
    // Arrange
    QString settingName = "SomeSetting";
    settingsService->clearSettings();

    // Act
    auto result = settingsService->getSetting(settingName);

    // Assert
    EXPECT_TRUE(result.isEmpty());
}


}  // namespace tests::application