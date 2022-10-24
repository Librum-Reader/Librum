#pragma once
#include "../libs/di/include/boost/di.hpp"

#include "i_book_metadata_helper.hpp"
#include "book_metadata_helper.hpp"
#include "i_downloaded_books_tracker.hpp"
#include "downloaded_books_tracker.hpp"
#include "i_internet_connection_info.hpp"
#include "internet_connection_info.hpp"

#include "i_book_service.hpp"
#include "book_service.hpp"
#include "i_book_controller.hpp"
#include "book_controller.hpp"
#include "i_book_storage_gateway.hpp"
#include "book_storage_gateway.hpp"
#include "i_book_storage_access.hpp"
#include "book_storage_access.hpp"

#include "i_authentication_controller.hpp"
#include "authentication_controller.hpp"
#include "i_authentication_service.hpp"
#include "authentication_service.hpp"
#include "i_authentication_gateway.hpp"
#include "authentication_gateway.hpp"
#include "i_authentication_access.hpp"
#include "authentication_access.hpp"

#include "i_user_controller.hpp"
#include "user_controller.hpp"
#include "i_user_service.hpp"
#include "user_service.hpp"
#include "i_user_storage_gateway.hpp"
#include "user_storage_gateway.hpp"
#include "i_user_storage_access.hpp"
#include "user_storage_access.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = [] {
    using namespace adapters;
    using namespace application;
    using namespace infrastructure;
    
    return di::make_injector(
                // Authentication
                di::bind<IAuthenticationController>().to<controllers::AuthenticationController>(),
                di::bind<IAuthenticationService>().to<services::AuthenticationService>(),
                di::bind<IAuthenticationGateway>().to<gateways::AuthenticationGateway>(),
                di::bind<IAuthenticationAccess>().to<persistence::AuthenticationAccess>(),
                
                // Books
                di::bind<IBookController>().to<controllers::BookController>(),
                di::bind<IBookService>().to<services::BookService>(),
                di::bind<IBookStorageGateway>().to<gateways::BookStorageGateway>(),
                di::bind<IBookStorageAccess>().to<persistence::BookStorageAccess>(),
                
                // User
                di::bind<IUserController>().to<controllers::UserController>(),
                di::bind<IUserService>().to<services::UserService>(),
                di::bind<IUserStorageGateway>().to<gateways::UserStorageGateway>(),
                di::bind<IUserStorageAccess>().to<persistence::UserStorageAccess>(),
                
                // Utility
                di::bind<application::IBookMetadataHelper>().to<application::utility::BookMetadataHelper>(),
                di::bind<application::IDownloadedBooksTracker>().to<application::utility::DownloadedBooksTracker>(),
                di::bind<application::IInternetConnectionInfo>().to<application::utility::InternetConnectionInfo>()
                );
};

} // namespace config