#include "gtest/gtest.h"
#include "controller.hpp"

TEST(AController, CanDoStuff)
{
    Controller controller;
    EXPECT_EQ(9, controller.times3(3));
}