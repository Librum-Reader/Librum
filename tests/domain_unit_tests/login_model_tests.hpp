#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "login_model.hpp"

using namespace domain::models;


TEST(ALoginModel, ReturnsTheEmailWhenQueried)
{
    QString email = "johndoe@librum.com";
    LoginModel loginModel(email, "SomePassword123");
    ASSERT_EQ(email, loginModel.email());
}

TEST(ALoginModel, ReturnsThePasswordWhenQueried)
{
    QString password = "SomePassword123";
    LoginModel loginModel("johndoe@librum.com", password);
    ASSERT_EQ(password, loginModel.password());
}

TEST(ALoginModelWithValidData, ReturnsTrueWhenValidated)
{
    LoginModel loginModel("johndoe@librum.com", "SomePassword123");
    ASSERT_EQ(true, loginModel.isValid());
}

TEST(ALoginModelWithNoPassword, ReturnsFalseWhenValidated)
{
    LoginModel loginModel("johndoe@librum.com", "");
    ASSERT_EQ(false, loginModel.isValid());
}

TEST(ALoginModelWithNoEmail, ReturnsFalseWhenValidated)
{
    LoginModel loginModel("", "SomePassword123");
    ASSERT_EQ(false, loginModel.isValid());
}

TEST(ALoginModelWithATooShortPassword, ReturnsFalseWhenValidated)
{
    QString tooShortPassword = "123";
    LoginModel loginModel("johndoe@librum.com", tooShortPassword);
    ASSERT_EQ(false, loginModel.isValid());
}

TEST(ALoginModelWithATooLongPassword, ReturnsFalseWhenValidated)
{
    QString tooLongPassword(61, 'a');
    LoginModel loginModel("johndoe@librum.com", tooLongPassword);
    ASSERT_EQ(false, loginModel.isValid());
}