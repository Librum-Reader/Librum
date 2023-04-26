#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

class SortByReadingProgressTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

SortByReadingProgressTestData readingProgressSortingTestData[] = {
    { Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 600),
      Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 700), false },

    { Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 430),
      Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 310), true },

    { Book("some/path", BookMetaData { .pageCount = 300, .lastOpened = QDateTime::currentDateTimeUtc() }, 200),
      Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 400), true },

    { Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 900),
      Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 0), true },

    { Book("some/path", BookMetaData { .pageCount = 10, .lastOpened = QDateTime::currentDateTimeUtc() }, 5),
      Book("some/path", BookMetaData { .pageCount = 900, .lastOpened = QDateTime::currentDateTimeUtc() }, 500), false },
};

}  // namespace test_data