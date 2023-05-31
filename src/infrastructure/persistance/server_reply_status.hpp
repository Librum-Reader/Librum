#pragma once
#include <QString>

namespace infrastructure::persistence
{

struct ServerReplyStatus
{
    bool success = true;
    QString errorMessage = "";
    int errorCode = 0;
};

}  // namespace infrastructure::persistence