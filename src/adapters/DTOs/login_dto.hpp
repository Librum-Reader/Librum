#pragma once
#include <QString>


namespace adapters::dtos
{

struct LoginDto
{
public:
    QString email;
    QString password;
};

}