#include <QString>

#pragma once

namespace adapters::dtos
{

struct LoginDto
{
public:
    QString email;
    QString password;
};

}