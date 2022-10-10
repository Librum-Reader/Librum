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
    
    const QString& getFirstName() const;
    void setFirstName(const QString& newFirstName);
    
    const QString& getLastName() const;
    void setLastName(const QString& newLastName);
    
    const QString& getEmailName() const;
    void setEmailName(const QString& newEmailName);
    
    const std::vector<Tag>& getTags() const;
    void setTags(const std::vector<Tag>& newTags);
    
private:
    QString m_firstName;
    QString m_lastName;
    QString m_emailName;
    std::vector<Tag> m_tags;
};

} // namespace domain::models