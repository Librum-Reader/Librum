#pragma once
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QModelIndex>
#include <QString>
#include "library_model.hpp"
#include "library_proxy_model.hpp"


using namespace testing;
using ::testing::ReturnRef;
using namespace adapters;
using namespace adapters::data_models;
using namespace domain::value_objects;
using namespace domain::entities;

namespace tests::adapters
{

class SortByTitleTestData
{
public:
    Book first;
    Book second;
    bool expectedResult;
};

class ALibraryProxyModel : public ::testing::TestWithParam<SortByTitleTestData>
{
public:
    void SetUp() override
    {
    }

    LibraryProxyModel libraryProxyModel;
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

TEST_P(ALibraryProxyModel, Works)
{
    // Arrange
    SortByTitleTestData data = GetParam();
    std::vector<Book> bookVec { data.first, data.second };

    LibraryModel model(bookVec);
    libraryProxyModel.setSourceModel(&model);
    libraryProxyModel.setSortRole(LibraryProxyModel::SortRole::Title);


    // Act
    QModelIndex parent;
    auto result = libraryProxyModel.lessThan(model.index(0, 0, parent),
                                             model.index(1, 0, parent));

    // Assert
    EXPECT_EQ(data.expectedResult, result);
}

// Register the test cases
INSTANTIATE_TEST_SUITE_P(TestSuite, ALibraryProxyModel,
                         ::testing::ValuesIn(titleSortingTestData));

}  // namespace tests::adapters