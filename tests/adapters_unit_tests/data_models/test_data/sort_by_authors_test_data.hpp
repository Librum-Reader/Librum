#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

class SortByAuthorsTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

SortByAuthorsTestData authorsSortingTestData[] = {
    { Book("some/path", BookMetaData { .authors = "Ike Marks" }),
      Book("some/path", BookMetaData { .authors = "Ava Osborne" }), false },

    { Book("some/path", BookMetaData { .authors = "Dick Krueger" }),
      Book("some/path", BookMetaData { .authors = "Lewis Thompson" }), true },

    { Book("some/path", BookMetaData { .authors = "Lilian Parker" }),
      Book("some/path", BookMetaData { .authors = "Lindsey Sheppard" }), true },

    { Book("some/path", BookMetaData { .authors = "Garner Everett" }),
      Book("some/path", BookMetaData { .authors = "Palmer Allen" }), true },

    { Book("some/path", BookMetaData { .authors = "Sidney Gibson" }),
      Book("some/path", BookMetaData { .authors = "Burl Lang" }), false },

    { Book("some/path", BookMetaData { .authors = "Dell Morrow" }),
      Book("some/path", BookMetaData { .authors = "Emory Harris" }), true },

    { Book("some/path", BookMetaData { .authors = "Michael Scott" }),
      Book("some/path", BookMetaData { .authors = "Scott Meyers" }), true },

    { Book("some/path", BookMetaData { .authors = "Bjarne Stroustrup" }),
      Book("some/path", BookMetaData { .authors = "Timmy jackings" }), true },

    { Book("some/path", BookMetaData { .authors = "Nellie Stanton" }),
      Book("some/path", BookMetaData { .authors = "Mitchel Nichols" }), false },

    { Book("some/path", BookMetaData { .authors = "Victoria Glary" }),
      Book("some/path", BookMetaData { .authors = "Ashley Christen" }), false },

    { Book("some/path", BookMetaData { .authors = "Tillman Werner" }),
      Book("some/path", BookMetaData { .authors = "Alf Morton" }), false },

    { Book("some/path", BookMetaData { .authors = "West Brock" }),
      Book("some/path", BookMetaData { .authors = "Wilfred Thomas" }), true },
};

}  // namespace test_data