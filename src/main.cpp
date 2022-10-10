#include <memory>
#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <QQuickStyle>
#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include "app_information.hpp"
#include "book_dto.hpp"
#include "chapter_tree_model.hpp"
#include "dependency_injection.hpp"
#include "i_book_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "sidebar_state.hpp"
#include "document_item.hpp"
#include "page_item.hpp"
#include "tag_dto.hpp"
#include "user_controller.hpp"



void registerTypes();
void loadFonts();
void addTranslations();


int main(int argc, char *argv[])
{
    // App
    QApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Etovex");
    QGuiApplication::setOrganizationDomain("Etovex.com");
    QGuiApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));
    
    addTranslations();
    loadFonts();
    
    
    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterType<adapters::data_models::ChapterTreeModel>("Librum.models", 1, 0, "ChapterTreeModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageItem>("Librum.elements", 1, 0, "PageItem");
    qmlRegisterType<cpp_elements::DocumentItem>("Librum.elements", 1, 0, "DocumentItem");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();
    
    
    using namespace adapters::controllers;
    using namespace application::services;
    
    // Authentication-Stack
    auto authenticationService = config::diConfig().create<application::IAuthenticationService*>();
    auto authenticationController = std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController", authenticationController.get());
    
    // User-Stack
    auto userService = config::diConfig().create<application::IUserService*>();
    auto userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController", userController.get());
    
    // Book-Stack
    auto bookService = config::diConfig().create<application::IBookService*>();
    auto bookController = std::make_unique<BookController>(bookService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController", bookController.get());
    
    // App info
    auto appInfo = std::make_unique<adapters::data_models::AppInformation>();
    qmlRegisterSingletonInstance("Librum.models", 1, 0, "AppInformation", appInfo.get());
    
    // Sidebar
    auto sidebarState = std::make_unique<cpp_elements::SidebarState>();
    qmlRegisterSingletonInstance("Librum.elements", 1, 0, "SidebarState", sidebarState.get());
    
    // Enums
    qmlRegisterUncreatableMetaObject(application::staticMetaObject, "Librum.controllers", 1, 0, 
                                     "BookOperationStatus", "This is an uncreatable enum!");
    
    
    
    
    // Setup authentication-token connections
    QObject::connect(authenticationService, &application::IAuthenticationService::authenticationTokenRegistered,
                     bookService, &application::IBookService::setAuthenticationToken);
    
    QObject::connect(authenticationService, &application::IAuthenticationService::authenticationTokenRemoved,
                         bookService, &application::IBookService::clearAuthenticationToken);
    
    
    QObject::connect(authenticationService, &application::IAuthenticationService::authenticationTokenRegistered,
                     userService, &application::IUserService::setAuthenticationToken);
    
    QObject::connect(authenticationService, &application::IAuthenticationService::authenticationTokenRemoved,
                         userService, &application::IUserService::clearAuthenticationToken);
    
    
    
    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");
    
    const QUrl url("qrc:/main.qml");
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return QGuiApplication::exec();
}


void addTranslations()
{
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "Librum_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName))
        {
            QGuiApplication::installTranslator(&translator);
            break;
        }
    }
}

void loadFonts()
{
    const QString fontsPath = QGuiApplication::instance()->applicationDirPath() + "/resources/fonts/";
    const QDir fontsDir(fontsPath);
    if(!fontsDir.isEmpty() && fontsDir.exists())
    {
        QDirIterator it(fontsPath, QDir::Files);
        while(it.hasNext())
        {
            int res = QFontDatabase::addApplicationFont(it.next());
            if(res == -1)
                qWarning() << "Loading font file: " + it.fileName() + " failed.";
        }
    }
    else
    {
        qWarning() << "Unable to load application fonts from " + fontsPath;
    }
    
    
    QFont defaultFont("SF Pro Display");
    QGuiApplication::setFont(defaultFont);
}
