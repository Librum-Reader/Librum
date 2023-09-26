#pragma once
#include <QString>

namespace application::core::utils
{

struct SearchOptions
{
    bool fromStart = false;
    int currentPage = 0;

    bool wholeWords = false;
    bool caseSensitive = false;
};

}  // namespace application::core::utils