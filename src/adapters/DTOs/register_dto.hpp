#pragma once
#include <QString>

namespace adapters::dtos
{

struct RegisterDto
{
    QString firstName;
    QString lastName;
    QString email;
    QString password;
};

}  // namespace adapters::dtos