#pragma once
#include <QString>
#include <QVariant>
#include <utility>
#include <vector>

namespace application::utility
{

// A POD-object which holds all application settings
struct ApplicationSettings
{
    // Each setting has a key (QString) and value (QVariant)
    std::vector<std::pair<QString, QVariant>> appearanceSettings;
    std::vector<std::pair<QString, QVariant>> generalSettings;
    std::vector<std::pair<QString, QVariant>> shortcuts;
};

}  // namespace application::utility