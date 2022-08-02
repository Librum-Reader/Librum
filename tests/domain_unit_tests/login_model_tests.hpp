#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "login_model.hpp"

using namespace domain::models;


TEST(ALoginModel, ReturnsTheEmailWhenQueried)
{
    // Arrange
    QString email = "johndoe@librum.com";
    LoginModel loginModel(email, "SomePassword123");
    
    // Act
    QString result = loginModel.email();
    
    // Assert
    ASSERT_EQ(email, result);
}

TEST(ALoginModel, ReturnsThePasswordWhenQueried)
{
    // Arrange
    QString password = "SomePassword123";
    LoginModel loginModel("johndoe@librum.com", password);
    
    // Act
    QString result = loginModel.password();
    
    // Assert
    ASSERT_EQ(password, result);
}

TEST(ALoginModelWithValidData, ReturnsTrueWhenValidated)
{
    // Arrange
    LoginModel loginModel("johndoe@librum.com", "SomePassword123");
    
    // Act
    bool result = loginModel.isValid();
    
    // Assert
    ASSERT_TRUE(result);
}

TEST(ALoginModelWithNoPassword, ReturnsFalseWhenValidated)
{
    // Arrange
    LoginModel loginModel("johndoe@librum.com", "");
    
    // Act
    bool result = loginModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ALoginModelWithNoEmail, ReturnsFalseWhenValidated)
{
    // Arrange
    LoginModel loginModel("", "SomePassword123");
    
    // Act
    bool result = loginModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ALoginModelWithATooShortPassword, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooShortPassword = "123";
    LoginModel loginModel("johndoe@librum.com", tooShortPassword);
    
    // Act
    bool result = loginModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}

TEST(ALoginModelWithATooLongPassword, ReturnsFalseWhenValidated)
{
    // Arrange
    QString tooLongPassword(61, 'a');
    LoginModel loginModel("johndoe@librum.com", tooLongPassword);
    
    // Act
    bool result = loginModel.isValid();
    
    // Assert
    ASSERT_FALSE(result);
}