#pragma once
#include <string_view>


class AppInformation
{
public:
    constexpr static std::string_view currentVersion { "v0.0.1" };
    constexpr static std::string_view newestVersion { "v0.0.1" };
    constexpr static std::string_view currentQtVersion { "v6.3.0" };
    constexpr static std::string_view companyName { "Etovex" };
    constexpr static std::string_view companyWebsite { "https://librumreader.netlify.app/" };
    constexpr static std::string_view companyEmail { "support@etovex.com" };
};