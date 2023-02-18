#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QMetaEnum>
#include <QString>
#include "enum_utils.hpp"
#include "test_enum.hpp"


using namespace testing;
using namespace application;

namespace tests::application
{

TEST(AnEnumUtility, SucceedsGettingNameForEnum)
{
    // Arrange
    auto value = TestEnum::Enum::First;

    QString expectedResult = "First";


    // Act
    auto result = utility::getNameForEnumValue(value);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

TEST(AnEnumUtility, SucceedsGettingValueForEnumName)
{
    // Arrange
    QString val = "Second";

    auto expectedResult = TestEnum::Enum::Second;


    // Act
    auto result = utility::getValueForEnumName<TestEnum::Enum>(val);

    // Assert
    EXPECT_EQ(static_cast<int>(expectedResult),
              static_cast<int>(result.value()));
}

TEST(AnEnumUtility, FailsGettingsValueForEnumName)
{
    // Arrange
    QString val = "NonExitentEnum";

    auto expectedResult = std::nullopt;


    // Act
    auto result = utility::getValueForEnumName<TestEnum::Enum>(val);

    // Assert
    EXPECT_EQ(expectedResult, result);
}

}  // namespace tests::application