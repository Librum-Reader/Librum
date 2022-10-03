#include "cover_observer.hpp"


using namespace Okular;


namespace application::utility
{

void CoverObserver::notifyPageChanged(int page, int flags)
{
    pageChanged(page, flags);
}

} // namespace application::utility