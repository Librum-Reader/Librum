#pragma once
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_LIBRARY RegisterDto
{
    QString firstName;
    QString lastName;
    QString email;
    QString password;
};

}  // namespace adapters::dtos
