#pragma once
#include <QString>
#include "adapters_export.hpp"

namespace adapters::data_models
{

struct ADAPTERS_EXPORT FilterRequest
{
    QString authors = "";
    QString format = "";
    QString date = "";
    bool onlyBooks = false;
    bool onlyFiles = false;
    bool read = false;
    bool unread = false;
};

}  // namespace adapters::data_models
