#pragma once
#include <vector>
#include <QString>
#include "tag.hpp"


namespace domain::models
{

class User
{
public:
    User();
    User(const QString& firstName, const QString& lastName, const QString& email);
    
    const QString& getFirstName() const;
    void setFirstName(const QString& newFirstName);
    
    const QString& getLastName() const;
    void setLastName(const QString& newLastName);
    
    const QString& getEmail() const;
    void setEmail(const QString& newEmail);
    
    const std::vector<Tag>& getTags() const;
    void setTags(const std::vector<Tag>& newTags);
    
private:
    QString m_firstName;
    QString m_lastName;
    QString m_email;
    std::vector<Tag> m_tags;
};

} // namespace domain::models