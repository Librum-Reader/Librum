#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include <QDateTime>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QString>
#include <QTranslator>
#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <memory>
#include "app_information.hpp"
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "dependency_injection.hpp"
#include "document_item.hpp"
#include "i_book_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "library_proxy_model.hpp"
#include "message_handler.hpp"
#include "page_item.hpp"
#include "setting_groups.hpp"
#include "setting_keys.hpp"
#include "shortcuts_proxy_model.hpp"
#include "sidebar_state.hpp"
#include "tag_dto.hpp"
#include "user_controller.hpp"


using namespace adapters::controllers;
using namespace application::services;


void registerTypes();
void loadFonts();
void addTranslations();

int main(int argc, char* argv[])
{
    // clang-format off
    // App
    QApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Etovex");
    QGuiApplication::setOrganizationDomain("Etovex.com");
    QGuiApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));
    app.addLibraryPath(QCoreApplication::applicationDirPath() + "/bin/plugins/");

    qInstallMessageHandler(logging::messageHandler);

    addTranslations();
    loadFonts();


    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterType<adapters::data_models::LibraryProxyModel>("Librum.models", 1, 0, "LibraryProxyModel");
    qmlRegisterType<adapters::data_models::ShortcutsProxyModel>("Librum.models", 1, 0, "ShortcutsProxyModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageItem>("Librum.elements", 1, 0, "PageItem");
    qmlRegisterType<cpp_elements::DocumentItem>("Librum.elements", 1, 0, "DocumentItem");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();


    // Authentication-Stack
    auto* authenticationService =
        config::diConfig().create<application::IAuthenticationService*>();
    auto authenticationController =
        std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController",
                                 authenticationController.get());

    // User-Stack
    auto* userService = config::diConfig().create<application::IUserService*>();
    auto userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController",
                                 userController.get());

    // Book-Stack
    auto* bookService = config::diConfig().create<application::IBookService*>();
    auto bookController = std::make_unique<BookController>(bookService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController",
                                 bookController.get());

    // Settings-Stack
    auto* settingsService = config::diConfig().create<application::ISettingsService*>();
    auto settingsController = std::make_unique<SettingsController>(settingsService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "SettingsController",
                                 settingsController.get());

    // App info
    auto appInfo = std::make_unique<adapters::data_models::AppInformation>();
    qmlRegisterSingletonInstance("Librum.models", 1, 0, "AppInformation",
                                 appInfo.get());

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

    qmlRegisterUncreatableMetaObject(application::api_error_codes::staticMetaObject, "Librum.controllers",
                                     1, 0, "ApiErrorCodes",
                                     "This is an uncreatable enum!");


    // Setup login connections
    QObject::connect(
        authenticationService,
        &application::IAuthenticationService::loggedIn,
        bookService, &application::IBookService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
        bookService, &application::IBookService::clearUserData);


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
                     bookService, &application::IBookService::updateUsedBookStorage);


    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/src/presentation/qt_tree_view/qml/");

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

void addTranslations()
{
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for(const QString& locale : uiLanguages)
    {
        const QString baseName = "Librum_" + QLocale(locale).name();
        if(translator.load(":/i18n/" + baseName))
        {
            QGuiApplication::installTranslator(&translator);
            break;
        }
    }
}

void loadFonts()
{
    QString fontsPath =
        QGuiApplication::applicationDirPath() + "/resources/fonts/";
    QDir fontsDir(fontsPath);
    if(!fontsDir.isEmpty() && fontsDir.exists())
    {
        QDirIterator it(fontsPath, QDir::Files);
        while(it.hasNext())
        {
            int res = QFontDatabase::addApplicationFont(it.next());
            if(res == -1)
                qWarning() << QString("Loading font file: %1 failed.")
                                  .arg(it.fileName());
        }
    }
    else
    {
        qWarning() << QString("Unable to load application fonts from %1")
                          .arg(fontsPath);
    }


    QFont defaultFont("SF Pro Display");
    QGuiApplication::setFont(defaultFont);
}
