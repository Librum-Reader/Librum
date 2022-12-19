#pragma once

namespace application::utility
{

// A status struct used to hold information about whether the database or the
// local library needs to be updated after merging two books
struct MergeStatus
{
    bool updateLocalLibrary = false;
    bool updateDatabase = false;
};

}  // namespace application::utility