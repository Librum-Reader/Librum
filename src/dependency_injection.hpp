#pragma once
#include <boost/di.hpp>
#include "i_authentication_controller.hpp"
#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "authentication_service.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = [] {
    using namespace adapters;
    using namespace application;
    
    return di::make_injector(
                di::bind<controllers::IAuthenticationController>().to<controllers::AuthenticationController>(),
                di::bind<services::IAuthenticationService>().to<services::AuthenticationService>()
                );
};

} // namespace config