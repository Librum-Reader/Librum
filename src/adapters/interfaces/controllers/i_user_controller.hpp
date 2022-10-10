#include <QObject>
#pragma once


namespace adapters
{

class IUserController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ getFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ getLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString email READ getEmail NOTIFY emailChanged)
    
public:
    virtual ~IUserController() noexcept = default;
    
    virtual QString getFirstName() = 0;
    virtual void setFirstName(const QString& newFirstName) = 0;
    
    virtual QString getLastName() = 0;
    virtual void setLastName(const QString& newLastName) = 0;
    
    virtual QString getEmail() = 0;
    virtual void setEmail(const QString& newEmail) = 0;
    
signals:
    void firstNameChanged();
    void lastNameChanged();
    void emailChanged();
};

} // namespace adapters