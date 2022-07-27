#include <QObject>

#pragma once


class AuthenticationController : public QObject
{
    Q_OBJECT
    
public:
    AuthenticationController();
    Q_INVOKABLE void registerUser();
};

