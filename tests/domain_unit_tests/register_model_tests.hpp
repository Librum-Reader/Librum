#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "register_model.hpp"

using namespace domain::models;


TEST(ARegisterModel, ReturnsTheFirstNameWhenQueried)
{
    // Arrange
    QString firstName = "John";
    RegisterModel registerModel(firstName, "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    QString result = registerModel.firstName();
    
    // Assert
    ASSERT_EQ(firstName, result);
}

TEST(ARegisterModel, ReturnsTheLastNameWhenQueried)
{
    // Arrange
    QString lastName = "Doe";
    RegisterModel registerModel("John", lastName, "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    QString result = registerModel.lastName();
    
    // Assert
    ASSERT_EQ(lastName, result);
}

TEST(ARegisterModel, ReturnsTheEmailWhenQueried)
{
    // Arrange
    QString email = "johndoe@librum.com";
    RegisterModel registerModel("John", "Doe", email, "SomePassword123", true);
    
    // Assert
    QString result = registerModel.email();
    
    // Assert
    ASSERT_EQ(email, result);
}

TEST(ARegisterModel, ReturnsThePasswordWhenQueried)
{
    // Arrange
    QString password = "SomePassword123";
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", password, true);
    
    // Assert
    QString result = registerModel.password();
    
    // Assert
    ASSERT_EQ(password, result);
}

TEST(ARegisterModel, ReturnsTheKeepUpdatedWhenQueried)
{
    // Arrange
    bool keepUpdated = true;
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", "SomePassword123", keepUpdated);
    
    // Assert
    bool result = registerModel.keepUpdated();
    
    // Assert
    ASSERT_EQ(keepUpdated, result);
}

TEST(ARegisterModelWithValidData, ReturnsTrueWhenValidated)
{
    // Arrange
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_TRUE(result);
}

TEST(ARegisterModelWithATooShortFirstName, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooShortFirstName = "J";
    RegisterModel registerModel(tooShortFirstName, "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooLongFirstName, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooLongFirstName = QString(41, 'J');
    RegisterModel registerModel(tooLongFirstName, "Doe", "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooShortLastName, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooShortLastName = "D";
    RegisterModel registerModel("John", tooShortLastName, "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooLongLastName, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooLongLastName = QString(51, 'D');
    RegisterModel registerModel("John", tooLongLastName, "johndoe@librum.com", "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooShortEmail, ReturnsFalseWhenValidated)
{
    // Arrange
 
    QString tooShortEmail = "j";
    RegisterModel registerModel("John", "Doe", tooShortEmail, "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooLongEmail, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooLongEmail = QString(51, 'j');
    RegisterModel registerModel("John", "Doe", tooLongEmail, "SomePassword123", true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooShortPassword, ReturnsFalseWhenValidated)
{
    // Arrange
 
    QString tooShortPassword = "SomeP";
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", tooShortPassword, true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ARegisterModelWithATooLongPassword, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooLongPassword = QString(61, 'S');
    RegisterModel registerModel("John", "Doe", "johndoe@librum.com", tooLongPassword, true);
    
    // Assert
    bool result = registerModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}