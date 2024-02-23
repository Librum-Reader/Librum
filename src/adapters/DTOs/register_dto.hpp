#pragma once
#include <QString>
#include "adapters_export.hpp"

namespace adapters::dtos
{

struct ADAPTERS_EXPORT RegisterDto
{
    QString name;
    QString email;
    QString password;
};

}  // namespace adapters::dtos