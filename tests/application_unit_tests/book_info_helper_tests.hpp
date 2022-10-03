#pragma once
#include <gtest/gtest.h>
#include <QString>
#include "book_info_helper.hpp"


using namespace testing;


//TEST(ABookInfoManager, SucceedsParsingBookTitleFromFilePath)
//{
//    // Arrange
//    application::utility::BookInfoHelper bookInfoHelper;
    
//    QString expectedResult = "myBook";
//    QString filePath = "/some/file/path/" + expectedResult + ".pdf";
    
    
//    // Act
//    auto result = bookInfoHelper.getTitle();
    
//    // Assert
//    EXPECT_EQ(expectedResult, result);
//}

//TEST(ABookInfoManager, SucceedsParsingBookTitleFromFilePathIfNoFileExtensionExists)
//{
//    // Arrange
//    application::utility::BookInfoHelper bookInfoHelper;
    
//    QString expectedResult = "myBook";
//    QString filePath = "/some/file/path/" + expectedResult + "";
    
    
//    // Act
//    auto result = bookInfoHelper.getTitle();
    
//    // Assert
//    EXPECT_EQ(expectedResult, result);
//}