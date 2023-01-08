#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include <initializer_list>
#include <vector>
#include "book.hpp"
#include "book_meta_data.hpp"

namespace test_data
{

using namespace domain::entities;
using namespace domain::value_objects;

struct FilterByRequestTestData
{
    Book book;
    QString authors = "";
    QString format = "";
    bool onlyBooks = false;
    bool onlyFiles = false;
    bool read = false;
    bool unread = false;
    bool expectedResult = false;
};

FilterByRequestTestData requestFilteringTestData[] = {
    FilterByRequestTestData {
        .book =
            Book { "some/path", BookMetaData { .authors = "Scott Meyers" } },
        .authors = "Scott Meyers",
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book =
            Book { "some/path", BookMetaData { .authors = "Scott Meyers" } },
        .authors = "Bob Martin",
        .expectedResult = false,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .authors = "Skott Maier" } },
        .authors = "Scott Meyers",
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .authors = "Bob Martin" } },
        .authors = "Bob",
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .authors = "Some Author" } },
        .authors = "So",
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .authors = "Reso Arter" } },
        .authors = "Rase Artar",
        .expectedResult = true,
    },

    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Pdf" } },
        .format = "Pdf",
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Pdf" } },
        .format = "Epub",
        .expectedResult = false,
    },

    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Plain" } },
        .onlyFiles = true,
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Pdf" } },
        .onlyFiles = true,
        .expectedResult = false,
    },

    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Pdf" } },
        .onlyBooks = true,
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .format = "Plain" } },
        .onlyBooks = true,
        .expectedResult = false,
    },

    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .pageCount = 100 }, 100 },
        .read = true,
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .pageCount = 100 }, 99 },
        .read = true,
        .expectedResult = false,
    },

    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .pageCount = 100 }, 99 },
        .unread = true,
        .expectedResult = true,
    },
    FilterByRequestTestData {
        .book = Book { "some/path", BookMetaData { .pageCount = 100 }, 100 },
        .unread = true,
        .expectedResult = false,
    },

    FilterByRequestTestData {
        .book = Book { "some/path",
                       BookMetaData { .authors = "Scott Meyers",
                                      .format = "Pdf",
                                      .pageCount = 100 },
                       99 },
        .authors = "Scot Mayers",
        .format = "pdf",
        .unread = true,
        .expectedResult = true,
    },

    FilterByRequestTestData {
        .book = Book { "some/path",
                       BookMetaData { .authors = "Scott Meyers",
                                      .format = "Pdf",
                                      .pageCount = 100 },
                       99 },
        .authors = "Bob Martin",
        .format = "pdf",
        .read = true,
        .expectedResult = false,
    },
};

}  // namespace test_data