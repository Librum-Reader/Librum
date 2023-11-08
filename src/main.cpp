#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include <QApplication>
#include <QDateTime>
#include <QFile>
#include <QGuiApplication>
#include <QIcon>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>
#include <QTranslator>
#include <memory>
#include "app_info_controller.hpp"
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "book_service.hpp"
#include "dependency_injection.hpp"
#include "free_books_model.hpp"
#include "i_free_books_service.hpp"
#include "i_library_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "library_proxy_model.hpp"
#include "message_handler.hpp"
#include "page_view.hpp"
#include "setting_groups.hpp"
#include "setting_keys.hpp"
#include "shortcuts_proxy_model.hpp"
#include "sidebar_state.hpp"
#include "tag_dto.hpp"
#include "user_controller.hpp"
#include "word_definition_dto.hpp"


using namespace adapters::controllers;
using namespace application::services;


void registerTypes();
void setupGlobalSettings();
void setupFonts();
void addTranslations(QTranslator& translator);

int main(int argc, char* argv[])
{
    // clang-format off
    // App
    QApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Librum-Reader");
    QGuiApplication::setOrganizationDomain("librumreader.com");
    QGuiApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));

    QIcon icon(":/src/logo.ico");
    app.setWindowIcon(icon);

    qInstallMessageHandler(logging::messageHandler);


    QTranslator translator;
    addTranslations(translator);


    setupGlobalSettings();
    setupFonts();

    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/FontSheet.qml"), "Librum.fonts", 1, 0, "Fonts");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterType<adapters::data_models::LibraryProxyModel>("Librum.models", 1, 0, "LibraryProxyModel");
    qmlRegisterType<adapters::data_models::FreeBooksModel>("Librum.models", 1, 0, "FreeBooksModel");
    qmlRegisterType<adapters::data_models::ShortcutsProxyModel>("Librum.models", 1, 0, "ShortcutsProxyModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageView>("Librum.elements", 1, 0, "PageView");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();
    qRegisterMetaType<adapters::dtos::DictionaryEntryDto>();
    qRegisterMetaType<adapters::dtos::WordTypeDto>();
    qRegisterMetaType<adapters::dtos::WordDefinitionDto>();


    // Authentication Stack
    auto* authenticationService =
        config::diConfig().create<application::IAuthenticationService*>();
    auto authenticationController =
        std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController",
                                 authenticationController.get());

    // App Info Stack
    auto* appInfoService =
        config::diConfig().create<application::IAppInfoService*>();
    auto appInfoController =
        std::make_unique<AppInfoController>(appInfoService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AppInfoController",
                                 appInfoController.get());

    // Ai explanation
    auto* aiExplanationService =
        config::diConfig().create<application::IAiExplanationService*>();
    auto aiExplanationController =
        std::make_unique<AiExplanationController>(aiExplanationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AiExplanationController",
                                 aiExplanationController.get());

    // User Stack
    auto* userService = config::diConfig().create<application::IUserService*>();
    auto userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController",
                                 userController.get());

    // Dictionary Stack
    auto* dictionaryService = config::diConfig().create<application::IDictionaryService*>();
    auto dictionaryController = std::make_unique<DictionaryController>(dictionaryService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "DictionaryController",
                                 dictionaryController.get());

    // Library Stack
    auto* libraryService = config::diConfig().create<application::ILibraryService*>();
    auto libraryController = std::make_unique<LibraryController>(libraryService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "LibraryController",
                                 libraryController.get());

    // Book Stack
    auto bookService = std::make_unique<application::services::BookService>(libraryService);
    auto bookController = std::make_unique<BookController>(bookService.get());
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController",
                                 bookController.get());

    // Free books stack
    auto* freeBooksService = config::diConfig().create<application::IFreeBooksService*>();
    auto freeBooksController = std::make_unique<FreeBooksController>(freeBooksService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "FreeBooksController",
                                 freeBooksController.get());

    // Settings Stack
    auto* settingsService = config::diConfig().create<application::ISettingsService*>();
    auto settingsController = std::make_unique<SettingsController>(settingsService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "SettingsController",
                                 settingsController.get());

    // Sidebar
    auto sidebarState = std::make_unique<cpp_elements::SidebarState>();
    qmlRegisterSingletonInstance("Librum.elements", 1, 0, "SidebarState",
                                 sidebarState.get());

    // Enums
    qmlRegisterUncreatableMetaObject(application::book_operation_status::staticMetaObject, "Librum.controllers",
                                     1, 0, "BookOperationStatus",
                                     "This is an uncreatable enum!");
    
    qmlRegisterUncreatableMetaObject(application::setting_keys::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingKeys",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::setting_groups::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingGroups",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::error_codes::staticMetaObject, "Librum.controllers",
                                     1, 0, "ErrorCode",
                                     "This is an uncreatable enum!");


    // Setup login connections
    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     libraryService, &application::ILibraryService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     libraryService, &application::ILibraryService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     freeBooksService, &application::IFreeBooksService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     aiExplanationService, &application::IAiExplanationService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     aiExplanationService, &application::IAiExplanationService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
        freeBooksService, &application::IFreeBooksService::clearUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     userService, &application::IUserService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     userService, &application::IUserService::clearUserData);


    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     settingsService, &application::ISettingsService::loadUserSettings);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     settingsService, &application::ISettingsService::clearUserData);


    // Setup other connections
    QObject::connect(userService, &application::IUserService::bookStorageDataUpdated,
                     libraryService, &application::ILibraryService::updateUsedBookStorage);

    QObject::connect(libraryController.get(),
                     &adapters::ILibraryController::downloadedProjectGutenbergIdsReady,
                     freeBooksController.get(),
                     &adapters::IFreeBooksController::proccessDownloadedProjectGutenbergIds);

    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/src/presentation/qt_tree_view/qml/");

    QQuickStyle::setStyle("Basic");

    const QUrl url("qrc:/main.qml");
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl)
        {
            if(obj == nullptr && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
    // clang-format on
}

void setupGlobalSettings()
{
    QSettings settings;
    QString cfgFile = settings.value("serverHost", QVariant("")).toString();
    if(cfgFile.isEmpty())
        settings.setValue("serverHost", "https://api.librumreader.com");

    QString sslSettings = settings.value("selfHosted", QVariant("")).toString();
    if(sslSettings.isEmpty())
        settings.setValue("selfHosted", "false");
}

void addTranslations(QTranslator& translator)
{
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for(const QString& locale : uiLanguages)
    {
        const QString baseName = "librum_" + QLocale(locale).name();
        if(translator.load(":/i18n/" + baseName))
        {
            QGuiApplication::installTranslator(&translator);
            break;
        }
    }
}

void loadFont(const QString& path)
{
    int result = QFontDatabase::addApplicationFont(path);
    if(result == -1)
        qWarning() << QString("Loading font file: %1 failed.").arg(path);
}

void setupFonts()
{
    loadFont(":/resources/fonts/SF-Pro-Display-Bold.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Medium.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Regular.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Semibold.otf");

    QFont defaultFont("SF Pro Display");
    defaultFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.1);
    QGuiApplication::setFont(defaultFont);
}
