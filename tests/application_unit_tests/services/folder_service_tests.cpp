#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <QString>
#include "folder.hpp"
#include "folder_service.hpp"
#include "i_folder_storage_gateway.hpp"

using namespace testing;
using ::testing::ReturnRef;
using namespace application::services;
using namespace application;
using namespace domain::entities;

namespace tests::application
{

class FolderStorageGatewayMock : public IFolderStorageGateway
{
public:
    MOCK_METHOD(void, updateFolder, (const QString&, const Folder&),
                (override));
    MOCK_METHOD(void, fetchFolders, (const QString&), (override));
};

class LocalLibraryTrackerMock : public ILocalLibraryTracker
{
public:
    MOCK_METHOD(void, setLibraryOwner, (const QString&), (override));
    MOCK_METHOD(void, clearLibraryOwner, (), (override));
    MOCK_METHOD(QDir, getLibraryDir, (), (const, override));
    MOCK_METHOD(std::vector<Book>, getTrackedBooks, (), (override));
    MOCK_METHOD(std::optional<Book>, getTrackedBook, (const QUuid&),
                (override));
    MOCK_METHOD(bool, trackBook, (const Book& book), (override));
    MOCK_METHOD(bool, untrackBook, (const QUuid&), (override));
    MOCK_METHOD(bool, updateTrackedBook, (const Book&), (override));
    MOCK_METHOD(void, saveFolders, (const Folder&), (override));
    MOCK_METHOD(Folder, loadFolders, (), (override));
};

struct AFolderService : public ::testing::Test
{
    void SetUp() override
    {
        folderService = std::make_unique<FolderService>(
            &folderStorageGatewayMock, &localLibraryTrackerMock);
    }

    FolderStorageGatewayMock folderStorageGatewayMock;
    LocalLibraryTrackerMock localLibraryTrackerMock;
    std::unique_ptr<FolderService> folderService;
};

TEST_F(AFolderService, SucceedsCreatingAFolderWithRootAsParent)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Act
    auto success = folderService->createFolder("name", "color", "icon",
                                               "description", QUuid());

    // Assert
    ASSERT_TRUE(success);
    auto rootFolder = folderService->getRootFolder();
    ASSERT_EQ(rootFolder->getChildren().size(), 1);
    ASSERT_EQ(rootFolder->getChildren().at(0)->getParent(), rootFolder);
}

TEST_F(AFolderService, SucceedsCreatingAFolderWithExistingFolderAsParent)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(2);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(2);

    // Arrange
    folderService->createFolder("name", "color", "icon", "description",
                                QUuid());
    auto parentFolder = folderService->getRootFolder()->getChildAtIndex(0);

    // Act
    auto success = folderService->createFolder(
        "second", "default", "folder", "description", parentFolder->getUuid());

    // Assert
    ASSERT_TRUE(success);
    ASSERT_EQ(parentFolder->getChildren().size(), 1);
    ASSERT_EQ(parentFolder->getChildren().at(0)->getParent(), parentFolder);
}

TEST_F(AFolderService, FailsCreatingAFolderIfParentDoesNotExist)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(0);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(0);

    // Arrange
    QUuid nonExistentParent = QUuid::createUuid();

    // Act
    auto success = folderService->createFolder(
        "name", "color", "icon", "description", nonExistentParent);

    // Assert
    ASSERT_FALSE(success);
    auto rootFolder = folderService->getRootFolder();
    ASSERT_EQ(rootFolder->getChildren().size(), 0);
}

TEST_F(AFolderService, SucceedsDeletingAFolder)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(4);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(4);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    folderService->createFolder("second", "color", "icon", "description",
                                QUuid());
    folderService->createFolder("third", "color", "icon", "description",
                                QUuid());

    auto secondUuid =
        folderService->getRootFolder()->getChildAtIndex(1)->getUuid();

    // Act
    auto deletedFolders = folderService->deleteFolder(secondUuid);

    // Assert
    ASSERT_EQ(deletedFolders.size(), 1);
    ASSERT_EQ(deletedFolders.at(0), secondUuid.toString(QUuid::WithoutBraces));

    // Ensure that the indexInParent values were changed properly
    auto thirdFolder = folderService->getRootFolder()->getChildAtIndex(1);
    ASSERT_EQ(thirdFolder->getIndexInParent(), 1);
}

TEST_F(AFolderService, SucceedsDeletingAFolderWithChildren)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _))
        .Times(AnyNumber());
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(AnyNumber());

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    folderService->createFolder("second", "color", "icon", "description",
                                QUuid());

    auto secondUuid =
        folderService->getRootFolder()->getChildAtIndex(1)->getUuid();
    folderService->createFolder("child", "color", "icon", "description",
                                secondUuid);
    folderService->createFolder("child2", "color", "icon", "description",
                                secondUuid);

    auto child2Uuid = folderService->getRootFolder()
                          ->getChildAtIndex(1)
                          ->getChildAtIndex(1)
                          ->getUuid();
    folderService->createFolder("grandChild", "color", "icon", "description",
                                child2Uuid);

    // Act
    auto deletedFolders = folderService->deleteFolder(secondUuid);

    // Assert
    ASSERT_EQ(deletedFolders.size(), 4);
    ASSERT_TRUE(
        deletedFolders.contains(secondUuid.toString(QUuid::WithoutBraces)));
    ASSERT_TRUE(
        deletedFolders.contains(child2Uuid.toString(QUuid::WithoutBraces)));
}

TEST_F(AFolderService, FailsDeletingAFolderIfFolderDoesNotExist)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());

    // Act
    auto deletedFolders = folderService->deleteFolder(QUuid::createUuid());

    // Assert
    ASSERT_EQ(deletedFolders.size(), 0);
}

TEST_F(AFolderService, SucceedsUpdatingAFolder)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(2);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(2);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    auto firstFolder = folderService->getRootFolder()->getChildAtIndex(0);

    // Act
    Folder updateFolder = *firstFolder;
    updateFolder.setName("newName");
    updateFolder.setColor("newColor");
    updateFolder.setIcon("newIcon");
    updateFolder.setDescription("newDescription");
    folderService->updateFolder(updateFolder);

    // Assert
    ASSERT_EQ(firstFolder->getName(), updateFolder.getName());
    ASSERT_EQ(firstFolder->getColor(), updateFolder.getColor());
    ASSERT_EQ(firstFolder->getIcon(), updateFolder.getIcon());
    ASSERT_EQ(firstFolder->getDescription(), updateFolder.getDescription());
}

TEST_F(AFolderService, FailsUpdatingAFolderIfFolderDoesNotExist)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Arrange
    QString originalName = "first";
    QString originalColor = "color";
    QString originalIcon = "icon";
    QString originalDescription = "description";
    folderService->createFolder(originalName, originalColor, originalIcon,
                                originalDescription, QUuid());
    auto firstFolder = folderService->getRootFolder()->getChildAtIndex(0);

    // Act
    Folder updateFolder = *firstFolder;
    updateFolder.setUuid(QUuid::createUuid());  // New Uuid
    updateFolder.setName("newName");
    updateFolder.setColor("newColor");
    updateFolder.setIcon("newIcon");
    updateFolder.setDescription("newDescription");
    folderService->updateFolder(updateFolder);

    // Assert
    ASSERT_EQ(firstFolder->getName(), originalName);
    ASSERT_EQ(firstFolder->getColor(), originalColor);
    ASSERT_EQ(firstFolder->getIcon(), originalIcon);
    ASSERT_EQ(firstFolder->getDescription(), originalDescription);
}

TEST_F(AFolderService, SucceedsMovingAFolder)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(4);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(4);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    folderService->createFolder("second", "color", "icon", "description",
                                QUuid());
    folderService->createFolder("third", "color", "icon", "description",
                                QUuid());
    auto firstFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(0)->getUuid();
    auto secondFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(1)->getUuid();
    auto thirdFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(2)->getUuid();

    // Act
    auto success = folderService->moveFolder(firstFolderUuid, secondFolderUuid);

    // Assert
    auto firstFolder = folderService->getFolder(firstFolderUuid);
    auto secondFolder = folderService->getFolder(secondFolderUuid);
    auto thirdFolder = folderService->getFolder(thirdFolderUuid);

    ASSERT_TRUE(success);
    ASSERT_EQ(folderService->getRootFolder()->childCount(), 2);
    ASSERT_EQ(secondFolder->getChildren().size(), 1);
    ASSERT_EQ(secondFolder->getChildAtIndex(0)->getUuid(), firstFolderUuid);
    ASSERT_EQ(firstFolder->getParent()->getUuid(), secondFolderUuid);

    // Ensure that the indexInParent values were adjusted correctly
    ASSERT_EQ(firstFolder->getIndexInParent(), 0);
    ASSERT_EQ(secondFolder->getIndexInParent(), 0);
    ASSERT_EQ(thirdFolder->getIndexInParent(), 1);
}

TEST_F(AFolderService, FailsMovingAFolderIfFolderDoesNotExist)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    auto firstFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(0)->getUuid();
    auto nonExistentUuid = QUuid::createUuid();

    // Act
    auto success = folderService->moveFolder(firstFolderUuid, nonExistentUuid);

    // Assert
    ASSERT_FALSE(success);
}

TEST_F(AFolderService, FailsMovingAFolderIfDestinationDoesNotExist)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    auto firstFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(0)->getUuid();
    auto nonExistentUuid = QUuid::createUuid();

    // Act
    auto success = folderService->moveFolder(nonExistentUuid, firstFolderUuid);

    // Assert
    ASSERT_FALSE(success);
}

TEST_F(AFolderService, FailsMovingAFolderIfDestinationAlreadyIsTheParent)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(1);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(1);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    auto firstFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(0)->getUuid();
    auto rootFolderUuid = folderService->getRootFolder()->getUuid();

    // Act
    auto success = folderService->moveFolder(firstFolderUuid, rootFolderUuid);

    // Assert
    ASSERT_FALSE(success);
}

TEST_F(AFolderService, FailsMovingAFolderIfDestinationIsADescendant)
{
    // Expect
    EXPECT_CALL(folderStorageGatewayMock, updateFolder(_, _)).Times(2);
    EXPECT_CALL(localLibraryTrackerMock, saveFolders(_)).Times(2);

    // Arrange
    folderService->createFolder("first", "color", "icon", "description",
                                QUuid());
    auto firstFolderUuid =
        folderService->getRootFolder()->getChildAtIndex(0)->getUuid();

    folderService->createFolder("second", "color", "icon", "description",
                                firstFolderUuid);
    auto secondFolderUuid = folderService->getFolder(firstFolderUuid)
                                ->getChildAtIndex(0)
                                ->getUuid();

    // Act
    auto success = folderService->moveFolder(firstFolderUuid, secondFolderUuid);

    // Assert
    ASSERT_FALSE(success);
}

TEST_F(AFolderService, SucceedsSettingUpUserData)
{
    // Arrange
    Folder rootFolder("ROOT", "", "", "");
    rootFolder.setLastModified(QDateTime::currentDateTimeUtc().addDays(-2));

    auto child1 =
        std::make_unique<Folder>("SomeFolder", "default", "folder", "");
    child1->setIndexInParent(0);
    rootFolder.addChild(std::move(child1));

    auto child2 =
        std::make_unique<Folder>("AnotherFolder", "default", "car", "");
    child2->setIndexInParent(1);
    rootFolder.addChild(std::move(child2));

    EXPECT_CALL(localLibraryTrackerMock, loadFolders())
        .WillOnce(Return(rootFolder));

    // Act
    folderService->setupUserData("someToken", "someEmail@librum.com");

    // Assert
    ASSERT_EQ(folderService->getRootFolder()->childCount(), 2);
    ASSERT_EQ(folderService->getRootFolder()->getUuid(), rootFolder.getUuid());
}

TEST_F(AFolderService, SucceedsSettingUpUserDataWhenNoLocalLibraryExists)
{
    // Arrange
    Folder invalidFolder("invalid", "", "", "");

    EXPECT_CALL(localLibraryTrackerMock, loadFolders())
        .WillOnce(Return(invalidFolder));

    // Act
    folderService->setupUserData("someToken", "someEmail@librum.com");

    // Assert
    ASSERT_EQ(folderService->getRootFolder()->childCount(), 1);
    ASSERT_EQ(folderService->getRootFolder()->getChildAtIndex(0)->getName(),
              "Archive");
}

};  // namespace tests::application