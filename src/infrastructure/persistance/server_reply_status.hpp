#pragma once
#include <QString>

namespace infrastructure::persistence
{

struct ServerReplyStatus
{
    bool success = true;
    QString errorMessage = "";
};

}  // namespace infrastructure::persistence