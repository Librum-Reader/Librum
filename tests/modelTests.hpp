#include "gtest/gtest.h"
#include "model.hpp"

TEST(AModel, CanDoStuffAsWell)
{
    Model model;
    EXPECT_EQ(6, model.times2(3));
}