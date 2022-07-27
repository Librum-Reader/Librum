#pragma once
#include <boost/di.hpp>
#include "i_authentication_controller.hpp"
#include "authentication_controller.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = [] {
    using namespace adapters;
    
    return di::make_injector(
                di::bind<controllers::IAuthenticationController>().to<controllers::AuthenticationController>()
                );
};

} // namespace config