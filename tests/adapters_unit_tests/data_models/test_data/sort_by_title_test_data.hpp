#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

class SortByTitleTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

SortByTitleTestData titleSortingTestData[] = {
    { Book("some/path", BookMetaData { .title = "ABook" }),
      Book("some/path", BookMetaData { .title = "ZBook" }), true },

    { Book("some/path", BookMetaData { .title = "Blank Paper" }),
      Book("some/path", BookMetaData { .title = "Stay Hidden" }), true },

    { Book("some/path", BookMetaData { .title = "The Happy Alien" }),
      Book("some/path", BookMetaData { .title = "The Bad Guy" }), false },

    { Book("some/path", BookMetaData { .title = "Innocent Eyes" }),
      Book("some/path", BookMetaData { .title = "Heart Me" }), false },

    { Book("some/path", BookMetaData { .title = "The Number" }),
      Book("some/path", BookMetaData { .title = "Spear of Gold" }), false },

    { Book("some/path", BookMetaData { .title = "Priest of Heaven" }),
      Book("some/path", BookMetaData { .title = "Answering God" }), false },

    { Book("some/path", BookMetaData { .title = "Winter Fairy" }),
      Book("some/path", BookMetaData { .title = "Choice of Gold" }), false },

    { Book("some/path", BookMetaData { .title = "Time Lies" }),
      Book("some/path", BookMetaData { .title = "Time Ticks" }), true },

    { Book("some/path", BookMetaData { .title = "Border with A Goal" }),
      Book("some/path", BookMetaData { .title = "World of Tomorrow" }), true },

    { Book("some/path", BookMetaData { .title = "Victory of Glory" }),
      Book("some/path", BookMetaData { .title = "Avatar" }), false },

    { Book("some/path", BookMetaData { .title = "A Turkey For Christmas" }),
      Book("some/path", BookMetaData { .title = "maginary Gravity" }), true },

    { Book("some/path", BookMetaData { .title = "The Hollow Spirit" }),
      Book("some/path", BookMetaData { .title = "Changing History" }), false },
};

}  // namespace test_data