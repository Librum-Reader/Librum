#pragma once
#include <QString>


namespace adapters::dtos
{

struct LoginDto
{
    QString email;
    QString password;
};

}