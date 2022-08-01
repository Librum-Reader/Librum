#pragma once
#include <boost/di.hpp>
#include "i_authentication_controller.hpp"
#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "authentication_service.hpp"
#include "i_user_gateway.hpp"
#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "user_storage_access.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = [] {
    
    return di::make_injector(
                di::bind<adapters::IAuthenticationController>().to<adapters::controllers::AuthenticationController>(),
                di::bind<application::IAuthenticationService>().to<application::services::AuthenticationService>(),
                di::bind<application::IUserGateway>().to<adapters::gateways::UserStorageGateway>(),
                di::bind<adapters::IUserStorageAccess>().to<infrastructure::persistence::UserStorageAccess>()
                );
};

} // namespace config