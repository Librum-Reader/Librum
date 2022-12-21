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
#include "chapter_tree_model.hpp"
#include "dependency_injection.hpp"
#include "document_item.hpp"
#include "i_book_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "library_proxy_model.hpp"
#include "page_item.hpp"
#include "sidebar_state.hpp"
#include "tag_dto.hpp"
#include "user_controller.hpp"
#include "user_dto.hpp"


void registerTypes();
void loadFonts();
void addTranslations();
void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg);

int main(int argc, char* argv[])
{
    // clang-format off
    // App
    QApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Etovex");
    QGuiApplication::setOrganizationDomain("Etovex.com");
    QGuiApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));

    qInstallMessageHandler(messageHandler);

    addTranslations();
    loadFonts();


    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterType<adapters::data_models::ChapterTreeModel>("Librum.models", 1, 0, "ChapterTreeModel");
    qmlRegisterType<adapters::data_models::LibraryProxyModel>("Librum.models", 1, 0, "LibraryProxyModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageItem>("Librum.elements", 1, 0, "PageItem");
    qmlRegisterType<cpp_elements::DocumentItem>("Librum.elements", 1, 0, "DocumentItem");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();
    qRegisterMetaType<adapters::dtos::UserDto>();


    using namespace adapters::controllers;
    using namespace application::services;


    // Authentication-Stack
    auto authenticationService =
        config::diConfig().create<application::IAuthenticationService*>();
    auto authenticationController =
        std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController",
                                 authenticationController.get());

    // User-Stack
    auto userService = config::diConfig().create<application::IUserService*>();
    auto userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController",
                                 userController.get());

    // Book-Stack
    auto bookService = config::diConfig().create<application::IBookService*>();
    auto bookController = std::make_unique<BookController>(bookService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController",
                                 bookController.get());

    // App info
    auto appInfo = std::make_unique<adapters::data_models::AppInformation>();
    qmlRegisterSingletonInstance("Librum.models", 1, 0, "AppInformation",
                                 appInfo.get());

    // Sidebar
    auto sidebarState = std::make_unique<cpp_elements::SidebarState>();
    qmlRegisterSingletonInstance("Librum.elements", 1, 0, "SidebarState",
                                 sidebarState.get());

    // Enums
    qmlRegisterUncreatableMetaObject(application::staticMetaObject, "Librum.controllers",
                                     1, 0, "BookOperationStatus",
                                     "This is an uncreatable enum!");


    // Setup login connections
    QObject::connect(
        authenticationService,
        &application::IAuthenticationService::loggedIn,
        bookService, &application::IBookService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
        bookService, &application::IBookService::clearUserData);


    QObject::connect( authenticationService, &application::IAuthenticationService::loggedIn,
        userService, &application::IUserService::setupUserData);

    QObject::connect( authenticationService, &application::IAuthenticationService::loggedOut,
        userService, &application::IUserService::clearUserData);


    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");

    const QUrl url("qrc:/main.qml");
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl)
        {
            if(!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
    // clang-format on
}

void messageHandler(QtMsgType type, const QMessageLogContext& context,
                    const QString& msg)
{
    Q_UNUSED(context);

    bool writeToStdOut = false;
    QString logLine;
    switch(type)
    {
    case QtInfoMsg:
        logLine = QString("Info: %1").arg(msg);
        break;
    case QtDebugMsg:
        logLine = QString("Debug: %1").arg(msg);
        writeToStdOut = true;
        break;
    case QtWarningMsg:
        logLine = QString("Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        logLine = QString("Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        logLine = QString("Fatal: %1").arg(msg);
        break;
    }

    QFile file("librum_log.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append) && !writeToStdOut)
    {
        QTextStream logStream(&file);

        QDateTime current = QDateTime::currentDateTime();
        QString dateString = current.toString("dd.MM.yyyy - hh.mm.ss");
        logStream << "(" << dateString << "): " << logLine << Qt::endl
                  << Qt::endl;
    }
    else
    {
        QTextStream out(stdout);
        out << "Librum message: " << logLine << Qt::endl;
    }
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
    const QString fontsPath =
        QGuiApplication::instance()->applicationDirPath() + "/resources/fonts/";
    const QDir fontsDir(fontsPath);
    if(!fontsDir.isEmpty() && fontsDir.exists())
    {
        QDirIterator it(fontsPath, QDir::Files);
        while(it.hasNext())
        {
            int res = QFontDatabase::addApplicationFont(it.next());
            if(res == -1)
                qWarning() << "Loading font file: " + it.fileName() +
                                  " failed.";
        }
    }
    else
    {
        qWarning() << "Unable to load application fonts from " + fontsPath;
    }


    QFont defaultFont("SF Pro Display");
    QGuiApplication::setFont(defaultFont);
}
