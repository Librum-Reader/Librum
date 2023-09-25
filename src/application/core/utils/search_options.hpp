#pragma once
#include <QString>

namespace application::core::utils
{

struct SearchOptions
{
    bool wholeWords = false;
    bool caseSensitive = false;
};

}  // namespace application::core::utils