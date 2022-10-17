#pragma once
#include <QObject>
#include <QString>
#include <QImage>


namespace application
{

class IUserService : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserService() noexcept = default;
    
    virtual void loadUser() = 0;
    
    virtual QString getFirstName() const = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;
    
    virtual QString getLastName() const = 0;
    virtual void setLastName(const QString& newLastName) = 0;
    
    virtual QString getEmail() const = 0;
    virtual void setEmail(const QString& newEmail) = 0;
    
    virtual QImage getProfilePicture() const = 0;
    virtual void setProfilePicture(const QImage& image) = 0;
    
signals:
    void finishedLoadingUser(bool success);

public slots:
    virtual void setAuthenticationToken(const QString& token,
                                        const QString& email) = 0;
    virtual void clearAuthenticationToken() = 0;
};

} // namespace application