#pragma once
#include <QObject>
#include <QString>


namespace application
{

class IUserService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserService() noexcept = default;
    
    virtual QString getFirstName() = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;
    
    virtual QString getLastName() = 0;
    virtual void setLastName(const QString& newLastName) = 0;
    
    virtual QString getEmail() = 0;
    virtual void setEmail(const QString& newEmail) = 0;
    
public slots:
    virtual void setAuthenticationToken(const QString& token) = 0;
    virtual void clearAuthenticationToken() = 0;
};

} // namespace application