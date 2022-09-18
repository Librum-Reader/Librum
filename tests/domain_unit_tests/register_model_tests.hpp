#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "register_model.hpp"

using namespace domain::models;


TEST(ARegisterModel, SucceedsValidation)
{
    // Arrange
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_TRUE(result);
}

TEST(ARegisterModel, FailsValidationsIfFirstNameIsTooShort)
{
    // Arrange
    QString tooShortFirstName = "J";
    RegisterModel registerModel(tooShortFirstName, "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfFirstNameIsTooLong)
{
    // Arrange
    QString tooLongFirstName = QString(41, 'J');
    RegisterModel registerModel(tooLongFirstName, "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfLastNameTooShort)
{
    // Arrange
    QString tooShortLastName = "D";
    RegisterModel registerModel("John", tooShortLastName, "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfLastNameIsTooLong)
{
    // Arrange
    QString tooLongLastName = QString(51, 'D');
    RegisterModel registerModel("John", tooLongLastName, "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfEmailIsTooShort)
{
    // Arrange
    QString tooShortEmail = "j";
    RegisterModel registerModel("John", "Doe", tooShortEmail, "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfEmailIsTooLong)
{
    // Arrange
    QString tooLongEmail = QString(51, 'j');
    RegisterModel registerModel("John", "Doe", tooLongEmail, "SomePassword123", true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfPasswordIsTooShort)
{
    // Arrange
    QString tooShortPassword = "SomeP";
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", tooShortPassword, true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}

TEST(ARegisterModel, FailsValidationIfPasswordIsTooLong)
{
    // Arrange
    QString tooLongPassword = QString(61, 'S');
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", tooLongPassword, true);
    
    // Assert
    auto result = registerModel.isValid();
    
    // Assert
    EXPECT_FALSE(result);
}