#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "i_settings_storage_gateway.hpp"
#include "settings_service.hpp"

using namespace testing;
using namespace application::services;

namespace tests::application
{

class SettingsStorageGatewayMock : public ::application::ISettingsStorageGateway
{
public:
    MOCK_METHOD(void, getSettings, (const QString&), (override));
    MOCK_METHOD(void, updateSettings, (const QString&, const QByteArray&),
                (override));
};

struct ASettingsService : public ::testing::Test
{
    void SetUp() override
    {
        settingsService =
            std::make_unique<SettingsService>(&settingsStorageGatewayMock);

        settingsService->loadUserSettings(testingEmail, "");
    }

    void TearDown() override
    {
        settingsService->clearSettings();
        settingsService->clearUserData();
    }

    SettingsStorageGatewayMock settingsStorageGatewayMock;
    std::unique_ptr<SettingsService> settingsService;

private:
    QString testingEmail = "LibrumTestEmail@librum.com.fake";
};

TEST_F(ASettingsService, SucceedsSettingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";

    // Expect
    EXPECT_CALL(settingsStorageGatewayMock, updateSettings(_, _)).Times(1);


    // Act
    settingsService->setSetting(settingName, settingValue);
}

TEST_F(ASettingsService, FailsSettingASettingIfSettingsAreInvalid)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeValue";

    settingsService->clearSettings();

    // Expect
    EXPECT_CALL(settingsStorageGatewayMock, updateSettings(_, _)).Times(0);


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

    // Expect
    EXPECT_CALL(settingsStorageGatewayMock, updateSettings(_, _)).Times(1);


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

TEST_F(ASettingsService, FailsGettingASettingIfSettingDoesNotExist)
{
    // Arrange
    QString nonExistentSettingName = "SomeSetting";


    // Act
    auto result = settingsService->getSetting(nonExistentSettingName);

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