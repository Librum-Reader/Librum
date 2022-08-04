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