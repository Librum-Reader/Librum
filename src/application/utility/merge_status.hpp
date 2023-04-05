#pragma once

namespace application::utility
{

// A status struct used to hold information about whether the remote or the
// local library is outdated.
struct MergeStatus
{
    bool localLibraryOutdated = false;
    bool remoteLibraryOutdated = false;
};

}  // namespace application::utility