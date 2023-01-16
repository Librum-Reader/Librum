#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "settings_controller.hpp"
#include "settings_service.hpp"

using namespace testing;
using namespace application;
using namespace adapters::controllers;

namespace tests::adapters
{

class SettingsServiceMock : public application::ISettingsService
{
public:
    MOCK_METHOD(QString, getSetting, (const QString&), (override));
    MOCK_METHOD(void, setSetting, (const QString&, const QString&), (override));
    MOCK_METHOD(void, loadUserSettings, (const QString&, const QString&),
                (override));
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

TEST_F(ASettingsController, SucceedsGettingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";

    // Expect
    EXPECT_CALL(settingsServiceMock, getSetting(_)).Times(1);

    // Act
    settingsController->getSetting(settingName);
}

TEST_F(ASettingsController, SucceedsSettingASetting)
{
    // Arrange
    QString settingName = "SomeSetting";
    QString settingValue = "SomeSetting";

    // Expect
    EXPECT_CALL(settingsServiceMock, setSetting(_, _)).Times(1);

    // Act
    settingsController->setSetting(settingName, settingValue);
}

}  // namespace tests::adapters