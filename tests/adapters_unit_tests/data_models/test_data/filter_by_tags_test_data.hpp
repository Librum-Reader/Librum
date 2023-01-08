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

class FilterByTagsTestData
{
public:
    Book book;
    std::vector<QString> filterTags;
    bool expectedResult;
};

Book getBookWithTags(std::initializer_list<QString> tagNames)
{
    Book book("some/path", {});
    for(auto& tagName : tagNames)
    {
        book.addTag(Tag(tagName));
    }

    return book;
}

FilterByTagsTestData tagFilteringTestData[] = {
    { getBookWithTags({ "First" }), { "First" }, true },
    { getBookWithTags({ "" }), { "First" }, false },
    { getBookWithTags({ "" }), { "First", "Second" }, false },
    { getBookWithTags({ "First", "Second" }), { "Second" }, true },
    { getBookWithTags({ "First" }), { "First", "Second" }, false },
    { getBookWithTags({ "First", "Second" }), { "First", "Second" }, true },
    { getBookWithTags({ "First", "Second" }), {}, true },
    { getBookWithTags({ "" }), {}, true },
    { getBookWithTags({ "Some" }), { "Other" }, false },
    { getBookWithTags({ "Third", "First" }), { "First", "Third" }, true },
    { getBookWithTags({ "Third", "First" }), { "Third" }, true },
    { getBookWithTags({ "Third", "First" }), { "Second" }, false },
};

}  // namespace test_data