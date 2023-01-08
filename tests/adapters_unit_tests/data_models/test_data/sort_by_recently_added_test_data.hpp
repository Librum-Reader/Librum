#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QDateTime>
#include "book.hpp"
#include "book_meta_data.hpp"
#include "library_proxy_model.hpp"

namespace test_data
{

using namespace testing;
using namespace domain::entities;
using namespace domain::value_objects;
using namespace adapters::data_models;

class SortByRecentlyAddedTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

SortByRecentlyAddedTestData recentlyAddedSortingTestData[] = {
    {
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addDays(-1),
             }),
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc(),
             }),
        false,
    },

    {
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc(),
             }),
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addDays(-1),
             }),
        true,
    },

    {
        Book("some/path",
             BookMetaData {
                 .addedToLibrary =
                     QDateTime::currentDateTimeUtc().addMSecs(-100),
             }),
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc(),
             }),
        false,
    },

    {
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addDays(-2),
             }),
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addDays(-3),
             }),
        true,
    },

    {
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addYears(-9),
             }),
        Book("some/path",
             BookMetaData {
                 .addedToLibrary = QDateTime::currentDateTimeUtc().addDays(-4),
             }),
        false,
    },
};

}  // namespace test_data