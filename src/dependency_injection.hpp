#pragma once
#include "../libs/di/include/boost/di.hpp"
#include "app_info_access.hpp"
#include "app_info_controller.hpp"
#include "app_info_gateway.hpp"
#include "app_info_service.hpp"
#include "authentication_access.hpp"
#include "authentication_controller.hpp"
#include "authentication_gateway.hpp"
#include "authentication_service.hpp"
#include "book_controller.hpp"
#include "book_service.hpp"
#include "highlight_storage_access.hpp"
#include "highlight_storage_gateway.hpp"
#include "highlight_storage_manager.hpp"
#include "i_app_info_access.hpp"
#include "i_app_info_service.hpp"
#include "i_highlight_storage_access.hpp"
#include "i_highlight_storage_gateway.hpp"
#include "i_highlight_storage_manager.hpp"
#include "i_metadata_extractor.hpp"
#include "library_controller.hpp"
#include "library_service.hpp"
#include "library_storage_access.hpp"
#include "library_storage_gateway.hpp"
#include "library_storage_manager.hpp"
#include "local_library_tracker.hpp"
#include "metadata_extractor.hpp"
#include "settings_controller.hpp"
#include "settings_service.hpp"
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

        // App Info
        di::bind<IAppInfoController>().to<controllers::AppInfoController>(),
        di::bind<IAppInfoService>().to<services::AppInfoService>(),
        di::bind<IAppInfoGateway>().to<gateways::AppInfoGateway>(),
        di::bind<IAppInfoAccess>().to<persistence::AppInfoAccess>(),

        // Library
        di::bind<ILibraryController>().to<controllers::LibraryController>(),
        di::bind<ILibraryService>().to<services::LibraryService>(),
        di::bind<ILibraryStorageGateway>()
            .to<gateways::LibraryStorageGateway>(),
        di::bind<ILibraryStorageAccess>()
            .to<persistence::LibraryStorageAccess>(),

        // Books
        di::bind<IBookController>().to<controllers::BookController>(),
        di::bind<IBookService>().to<services::BookService>(),

        // Highligts
        di::bind<IHighlightStorageManager>()
            .to<managers::HighlightStorageManager>(),
        di::bind<IHighlightStorageGateway>()
            .to<gateways::HighlightStorageGateway>(),
        di::bind<IHighlightStorageAccess>()
            .to<persistence::HighlightStorageAccess>(),

        // User
        di::bind<IUserController>().to<controllers::UserController>(),
        di::bind<IUserService>().to<services::UserService>(),
        di::bind<IUserStorageGateway>().to<gateways::UserStorageGateway>(),
        di::bind<IUserStorageAccess>().to<persistence::UserStorageAccess>(),

        // Settings
        di::bind<ISettingsController>().to<controllers::SettingsController>(),
        di::bind<ISettingsService>().to<services::SettingsService>(),

        // Utility
        di::bind<application::IMetadataExtractor>()
            .to<application::core::MetadataExtractor>(),
        di::bind<application::ILocalLibraryTracker>()
            .to<application::utility::LocalLibraryTracker>(),
        di::bind<application::ILibraryStorageManager>()
            .to<application::managers::LibraryStorageManager>());
};

}  // namespace config