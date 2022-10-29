#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "login_model.hpp"


using namespace domain::models;

namespace tests::domain
{

TEST(ALoginModel, SucceedsValidation)
{
    // Arrange
    LoginModel loginModel("johndoe@librum.com", "SomePassword123");

    // Act
    auto result = loginModel.isValid();

    // Assert
    EXPECT_TRUE(result);
}

TEST(ALoginModel, FailsValidationIfPasswordDoesNotExist)
{
    // Arrange
    LoginModel loginModel("johndoe@librum.com", "");

    // Act
    auto result = loginModel.isValid();

    // Assert
    EXPECT_FALSE(result);
}

TEST(ALoginModel, FailsValidationIfEmailDoesNotExist)
{
    // Arrange
    LoginModel loginModel("", "SomePassword123");

    // Act
    auto result = loginModel.isValid();

    // Assert
    EXPECT_FALSE(result);
}

TEST(ALoginModel, FailsValidationIfPasswordIsTooShort)
{
    // Arrange
    QString tooShortPassword = "SomeP";
    LoginModel loginModel("johndoe@librum.com", tooShortPassword);

    // Act
    auto result = loginModel.isValid();

    // Assert
    EXPECT_FALSE(result);
}

TEST(ALoginModel, FailsValidationIfPasswordIsTooLong)
{
    // Arrange
    QString tooLongPassword(61, 'a');
    LoginModel loginModel("johndoe@librum.com", tooLongPassword);

    // Act
    auto result = loginModel.isValid();

    // Assert
    EXPECT_FALSE(result);
}

}  // namespace tests::domain