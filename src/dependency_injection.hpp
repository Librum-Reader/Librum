#pragma once
#include "../libs/di/include/boost/di.hpp"
#include "authentication_access.hpp"
#include "authentication_controller.hpp"
#include "authentication_gateway.hpp"
#include "authentication_service.hpp"
#include "book_controller.hpp"
#include "book_metadata_helper.hpp"
#include "book_service.hpp"
#include "book_storage_access.hpp"
#include "book_storage_gateway.hpp"
#include "book_storage_manager.hpp"
#include "downloaded_books_tracker.hpp"
#include "settings_controller.hpp"
#include "settings_service.hpp"
#include "settings_storage_access.hpp"
#include "settings_storage_gateway.hpp"
#include "user_controller.hpp"
#include "user_service.hpp"
#include "user_storage_access.hpp"
#include "user_storage_gateway.hpp"


namespace di = boost::di;

namespace config
{

const auto diConfig = []
{
    using namespace adapters;
    using namespace application;
    using namespace infrastructure;

    return di::make_injector(
        // Authentication
        di::bind<IAuthenticationController>()
            .to<controllers::AuthenticationController>(),
        di::bind<IAuthenticationService>()
            .to<services::AuthenticationService>(),
        di::bind<IAuthenticationGateway>()
            .to<gateways::AuthenticationGateway>(),
        di::bind<IAuthenticationAccess>()
            .to<persistence::AuthenticationAccess>(),

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

        // Settings
        di::bind<ISettingsController>().to<controllers::SettingsController>(),
        di::bind<ISettingsService>().to<services::SettingsService>(),
        di::bind<ISettingsStorageGateway>()
            .to<gateways::SettingsStorageGateway>(),
        di::bind<ISettingsStorageAccess>()
            .to<persistence::SettingsStorageAccess>(),

        // Utility
        di::bind<application::IBookMetadataHelper>()
            .to<application::utility::BookMetadataHelper>(),
        di::bind<application::IDownloadedBooksTracker>()
            .to<application::utility::DownloadedBooksTracker>(),
        di::bind<application::IBookStorageManager>()
            .to<application::utility::BookStorageManager>());
};

}  // namespace config