#pragma once
#include <QObject>


namespace adapters::controllers
{

class AuthenticationController : public QObject
{
    Q_OBJECT
    
public:
    AuthenticationController();
    Q_INVOKABLE void registerUser();
};

} // namespace adapters::controllers