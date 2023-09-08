#pragma once
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT LoginDto
{
    QString email;
    QString password;
};

}  // namespace adapters::dtos
