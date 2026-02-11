#pragma once
#include <boost-di/di.hpp>
#include "ai_explanation_access.hpp"
#include "ai_explanation_controller.hpp"
#include "ai_explanation_gateway.hpp"
#include "ai_explanation_service.hpp"
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
#include "dictionary_access.hpp"
#include "dictionary_controller.hpp"
#include "dictionary_gateway.hpp"
#include "dictionary_service.hpp"
#include "folder_controller.hpp"
#include "folder_service.hpp"
#include "folder_storage_access.hpp"
#include "folder_storage_gateway.hpp"
#include "free_books_controller.hpp"
#include "free_books_service.hpp"
#include "free_books_storage_access.hpp"
#include "free_books_storage_gateway.hpp"
#include "i_app_info_access.hpp"
#include "i_app_info_service.hpp"
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
#include "tools_controller.hpp"
#include "tools_service.hpp"
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

        // Ai explanation
        di::bind<IAiExplanationController>()
            .to<controllers::AiExplanationController>(),
        di::bind<IAiExplanationService>().to<services::AiExplanationService>(),
        di::bind<IAiExplanationGateway>().to<gateways::AiExplanationGateway>(),
        di::bind<IAiExplanationAccess>().to<persistence::AiExplanationAccess>(),

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

        // Free books
        di::bind<IFreeBooksController>().to<controllers::FreeBooksController>(),
        di::bind<IFreeBooksService>().to<services::FreeBooksService>(),
        di::bind<IFreeBooksStorageGateway>()
            .to<gateways::FreeBooksStorageGateway>(),
        di::bind<IFreeBooksStorageAccess>()
            .to<persistence::FreeBooksStorageAccess>(),

        // User
        di::bind<IUserController>().to<controllers::UserController>(),
        di::bind<IUserService>().to<services::UserService>(),
        di::bind<IUserStorageGateway>().to<gateways::UserStorageGateway>(),
        di::bind<IUserStorageAccess>().to<persistence::UserStorageAccess>(),

        // Dictionary
        di::bind<IDictionaryController>()
            .to<controllers::DictionaryController>(),
        di::bind<IDictionaryService>().to<services::DictionaryService>(),
        di::bind<IDictionaryGateway>().to<gateways::DictionaryGateway>(),
        di::bind<IDictionaryAccess>().to<persistence::DictionaryAccess>(),

        // Folders
        di::bind<IFolderController>().to<controllers::FolderController>(),
        di::bind<IFolderService>().to<services::FolderService>(),
        di::bind<IFolderStorageGateway>().to<gateways::FolderStorageGateway>(),
        di::bind<IFolderStorageAccess>().to<persistence::FolderStorageAccess>(),

        // Settings
        di::bind<ISettingsController>().to<controllers::SettingsController>(),
        di::bind<ISettingsService>().to<services::SettingsService>(),

        // Tools
        di::bind<IToolsController>().to<controllers::ToolsController>(),
        di::bind<IToolsService>().to<services::ToolsService>(),

        // Utility
        di::bind<application::IMetadataExtractor>()
            .to<application::core::MetadataExtractor>(),
        di::bind<application::ILocalLibraryTracker>()
            .to<application::utility::LocalLibraryTracker>(),
        di::bind<application::ILibraryStorageManager>()
            .to<application::managers::LibraryStorageManager>());
};

}  // namespace config
