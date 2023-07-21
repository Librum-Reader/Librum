#pragma once
#include <QImage>
#include <QString>
#include <vector>
#include "tag.hpp"
#include "domain_export.hpp"

namespace domain::entities
{

class DOMAIN_LIBRARY User : public QObject
{
    Q_OBJECT

public:
     User(const QString& firstName, const QString& lastName,
         const QString& email, long usedBookStorage, long bookStorageLimit,
         const QDateTime& profilePictureLastUpdated = QDateTime(),
         bool hasProfilePicture = false);

     const QString& getFirstName() const;
     void setFirstName(const QString& newFirstName);

     const QString& getLastName() const;
     void setLastName(const QString& newLastName);

     const QString& getEmail() const;
     void setEmail(const QString& newEmail);

     long getUsedBookStorage() const;
     void setUsedBookStorage(long newUsedBookStorage);

     long getBookStorageLimit() const;
     void setBookStorageLimit(long newBookStorageLimit);

     bool hasProfilePicture() const;
     void setHasProfilePicture(bool newValue);

     const QDateTime& getProfilePictureLastUpdated() const;
     void setProfilePictureLastUpdated(const QDateTime& newLastUpdated);

     const QString& getProfilePicturePath() const;
     void setProfilePicturePath(const QString& path);

     const std::vector<Tag>& getTags() const;
     const Tag* getTagByName(const QString& tagName) const;
     const Tag* getTagByUuid(const QUuid& uuid) const;
     bool addTag(const Tag& tag);
     bool deleteTag(const QUuid& uuid);
     bool renameTag(const QUuid& uuid, const QString& newName);

     void clearData();

signals:
     void tagInsertionStarted(int index);
     void tagInsertionEnded();
     void tagDeletionStarted(int index);
     void tagDeletionEnded();
     void tagsChanged(int index);

private:
    int getTagIndex(const QUuid& uuid) const;

    QString m_firstName;
    QString m_lastName;
    QString m_email;
    long m_usedBookStorage;
    long m_bookStorageLimit;
    QDateTime m_profilePictureLastUpdated;
    bool m_hasProfilePicture;
    QString m_profilePicturePath;
    std::vector<Tag> m_tags;
};

}  // namespace domain::entities
