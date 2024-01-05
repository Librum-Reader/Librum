#pragma once
#include <QString>
#include "adapters_export.hpp"

namespace adapters::data_models
{

struct ADAPTERS_EXPORT FolderFilterRequest
{
    QString uuid = "";
    bool allBooks = true;
    bool onlyUnsorted = false;
};

}  // namespace adapters::data_models
