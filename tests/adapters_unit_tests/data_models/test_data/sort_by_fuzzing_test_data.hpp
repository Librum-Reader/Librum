#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

class SortByFuzzingTestData
{
public:
    Book first;
    Book second;
    QString sortString;
    bool expectedResult;
};

SortByFuzzingTestData fuzzSortingTestData[] = {
    { Book("some/path", BookMetaData { .title = "ABook" }),
      Book("some/path", BookMetaData { .title = "ZBook" }), "ABook", true },

    { Book("some/path", BookMetaData { .title = "ABook" }),
      Book("some/path", BookMetaData { .title = "ZBook" }), "ZBook", false },

    { Book("some/path", BookMetaData { .title = "Blank Paper" }),
      Book("some/path", BookMetaData { .title = "Stay Hidden" }), "Blank",
      true },

    { Book("some/path", BookMetaData { .title = "The Happy Alien" }),
      Book("some/path", BookMetaData { .title = "The Bad Guy" }), "Happy",
      true },

    { Book("some/path", BookMetaData { .title = "Innocent Eyes" }),
      Book("some/path", BookMetaData { .title = "Heart Me" }), "Inocent",
      true },

    { Book("some/path", BookMetaData { .title = "The Number" }),
      Book("some/path", BookMetaData { .title = "Spear of Gold" }), "The",
      true },

    { Book("some/path", BookMetaData { .title = "Priest of Heaven" }),
      Book("some/path", BookMetaData { .title = "Answering God" }), "God",
      false },

    { Book("some/path", BookMetaData { .title = "Winter Fairy" }),
      Book("some/path", BookMetaData { .title = "Choice of Gold" }), "Winter",
      true },

    { Book("some/path", BookMetaData { .title = "Time Lies" }),
      Book("some/path", BookMetaData { .title = "Time Ticks" }), "Time L",
      true },

    { Book("some/path", BookMetaData { .title = "Border with A Goal" }),
      Book("some/path", BookMetaData { .title = "World of Tomorrow" }),
      "Broder wit gol", true },

    { Book("some/path", BookMetaData { .title = "Victory of Glory" }),
      Book("some/path", BookMetaData { .title = "Avatar" }), "Glory", true },

    { Book("some/path", BookMetaData { .title = "A Turkey For Christmas" }),
      Book("some/path", BookMetaData { .title = "maginary Gravity" }), "Grav",
      false },

    { Book("some/path", BookMetaData { .title = "The Hollow Spirit" }),
      Book("some/path", BookMetaData { .title = "Changing History" }), "Hollow",
      true },
};

}  // namespace test_data