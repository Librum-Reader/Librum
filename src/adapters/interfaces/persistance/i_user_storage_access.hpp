#pragma once
#include <QObject>
#include <QString>


namespace adapters
{

class IUserStorageAccess : public QObject
{
    Q_OBJECT
    
public:
    virtual ~IUserStorageAccess() noexcept = default;
    
    virtual void getUser(const QString& authenticationToken) = 0;
    
private slots:
    virtual void proccessGetUserResult() = 0;
    
signals:
    void userReady(const QString firstName, const QString lastName, 
                   const QString email);
    void gettingUserFailed();
};

} // namespace adapters