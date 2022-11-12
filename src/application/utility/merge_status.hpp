#pragma once

namespace application::utility
{

// A status struct used to hold the information if the database or the local
// library needs to be updated after a merge of two books.
struct MergeStatus
{
    bool updateLocalLibrary = false;
    bool updateDatabase = false;
};

}  // namespace application::utility