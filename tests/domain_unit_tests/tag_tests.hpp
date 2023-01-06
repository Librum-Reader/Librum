#pragma once
#include <gtest/gtest.h>
#include "tag.hpp"


using namespace domain::entities;

namespace tests::domain
{

TEST(ATag, SucceedsEqualityComparisonToAnotherTag)
{
    Tag firstTag("SomeTag");
    Tag secondTag("SomeTag", firstTag.getUuid().toString(QUuid::WithoutBraces));

    EXPECT_EQ(firstTag, secondTag);
}

TEST(ATag, FailsEqualityComparisonToAnotherTagIfUuidDiffers)
{
    Tag firstTag("SomeTag");
    Tag secondTag("SomeTag");

    EXPECT_NE(firstTag, secondTag);
}

TEST(ATag, FailsEqualityComparisonToAnotherTagIfNameDiffers)
{
    Tag firstTag("SomeTag");
    Tag secondTag("DifferentTag",
                  firstTag.getUuid().toString(QUuid::WithoutBraces));

    EXPECT_NE(firstTag, secondTag);
}

TEST(ATag, SucceedsValidityCheck)
{
    QString validName = "Programming";
    Tag firstTag(validName);

    EXPECT_TRUE(firstTag.isValid());
}

TEST(ATag, FailsValidityCheckIfNameIsTooShort)
{
    QString shortName = "X";
    Tag firstTag(shortName);

    EXPECT_FALSE(firstTag.isValid());
}

}  // namespace tests::domain