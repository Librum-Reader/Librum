#pragma once
#include <QObject>
#include <QString>
#include "user.hpp"


namespace application
{

class IUserService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserService() noexcept = default;
    
    virtual void loadUser() = 0;
    
    virtual QString getFirstName() = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;
    
    virtual QString getLastName() = 0;
    virtual void setLastName(const QString& newLastName) = 0;
    
    virtual QString getEmail() = 0;
    virtual void setEmail(const QString& newEmail) = 0;
    
signals:
    void finishedLoadingUser(bool success);

public slots:
    virtual void proccessUserInformation(const domain::models::User& user,
                                         bool success) = 0;
    virtual void setAuthenticationToken(const QString& token) = 0;
    virtual void clearAuthenticationToken() = 0;
};

} // namespace application