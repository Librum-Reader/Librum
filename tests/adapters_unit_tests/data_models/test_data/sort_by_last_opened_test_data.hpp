#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

class SortByLastOpenedTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

SortByLastOpenedTestData lastOpenedSortingTestData[] = {
    {
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addDays(-1),
             }),
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc(),
             }),
        false,
    },

    {
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc(),
             }),
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addDays(-1),
             }),
        true,
    },

    {
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addMSecs(-100),
             }),
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc(),
             }),
        false,
    },

    {
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addDays(-2),
             }),
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addDays(-3),
             }),
        true,
    },

    {
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addYears(-9),
             }),
        Book("some/path",
             BookMetaData {
                 .lastOpened = QDateTime::currentDateTimeUtc().addDays(-4),
             }),
        false,
    },
};

}  // namespace test_data