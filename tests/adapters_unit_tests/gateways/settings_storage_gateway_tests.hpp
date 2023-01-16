#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "i_settings_storage_access.hpp"
#include "settings_storage_gateway.hpp"


using namespace testing;
using namespace adapters::gateways;
using namespace adapters;

namespace tests::adapters
{

class SettingsStorageAccessMock : public ISettingsStorageAccess
{
public:
    MOCK_METHOD(void, getSettings, (const QString&), (override));
    MOCK_METHOD(void, updateSettings, (const QString&, const QByteArray&),
                (override));
};

struct ASettingsStorageGateway : public testing::Test
{
    void SetUp() override
    {
        settingsStorageGateway = std::make_unique<SettingsStorageGateway>(
            &settingsStorageAccessMock);
    }

    SettingsStorageAccessMock settingsStorageAccessMock;
    std::unique_ptr<SettingsStorageGateway> settingsStorageGateway;
};

TEST_F(ASettingsStorageGateway, SucceedsGettingSettings)
{
    // Expect
    EXPECT_CALL(settingsStorageAccessMock, getSettings(_)).Times(1);

    // Act
    settingsStorageGateway->getSettings("secureToken");
}

TEST_F(ASettingsStorageGateway, SucceedsUpdatingSettings)
{
    // Expect
    EXPECT_CALL(settingsStorageAccessMock, updateSettings(_, _)).Times(1);

    // Act
    settingsStorageGateway->updateSettings("secureToken", "SomeData");
}

}  // namespace tests::adapters