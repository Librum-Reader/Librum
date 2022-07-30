#pragma once
#include <QString>


namespace adapters::dtos
{

struct RegisterDto
{
public:
    QString firstName;
    QString lastName;
    QString email;
    QString password;
};

}