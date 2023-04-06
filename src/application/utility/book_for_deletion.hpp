#pragma once
#include <QUuid>

namespace application::utility
{

struct BookForDeletion
{
    QUuid uuid;
    bool downloaded;
};

}  // namespace application::utility