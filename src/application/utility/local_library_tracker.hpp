#pragma once
#include "i_local_library_tracker.hpp"

namespace application::utility
{

class LocalLibraryTracker : public ILocalLibraryTracker
{
public:
    std::vector<domain::entities::Book> getTrackedBooks() override;
    std::optional<domain::entities::Book> getTrackedBook(
        const QUuid& uuid) override;
    bool trackBook(const domain::entities::Book& book) override;
    bool untrackBook(const QUuid& uuid) override;
    bool updateTrackedBook(const domain::entities::Book& book) override;

    void saveFolders(const domain::entities::Folder& folder) override;
    domain::entities::Folder loadFolders() override;

    // Setup methods
    void setLibraryOwner(const QString& libraryOwnerEmail) override;
    void clearLibraryOwner() override;
    QDir getLibraryDir() const override;

private:
    void ensureUserLibraryExists() const;
    QJsonDocument parseLibMetaFile(QByteArray&& data) const;
    QString getUserLibraryName(QString email) const;

    QString m_libraryOwnerEmail;
    QDir m_libraryFolder;
    const QString m_fileExtension = ".libmeta";
    const QString m_rootFolderFileName = "folders.json";
};

}  // namespace application::utility