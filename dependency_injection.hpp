#pragma once
#include "libs/di/include/boost/di.hpp"
#include "book_controller.hpp"
#include "i_book_controller.hpp"
#include "i_authentication_controller.hpp"
#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "authentication_service.hpp"
#include "i_user_storage_gateway.hpp"
#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "user_storage_access.hpp"
#include "i_book_service.hpp"
#include "book_service.hpp"
#include "book_info_helper.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = [] {
    return di::make_injector(
                di::bind<adapters::IAuthenticationController>().to<adapters::controllers::AuthenticationController>(),
                di::bind<application::IAuthenticationService>().to<application::services::AuthenticationService>(),
                di::bind<application::IUserStorageGateway>().to<adapters::gateways::UserStorageGateway>(),
                di::bind<adapters::IUserStorageAccess>().to<infrastructure::persistence::UserStorageAccess>(),
                
                di::bind<adapters::IBookController>().to<adapters::controllers::BookController>(),
                di::bind<application::IBookService>().to<application::services::BookService>(),
                
                di::bind<application::IBookInfoHelper>().to<application::utility::BookInfoManager>()
                );
};

} // namespace config