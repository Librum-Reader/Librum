#include "cover_observer.hpp"


using namespace Okular;

namespace application::utility
{

void CoverObserver::notifyPageChanged(int page, int flags)
{
    emit pageLoaded(page, flags);
}

}  // namespace application::utility