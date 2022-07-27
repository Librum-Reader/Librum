#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include <boost/di.hpp>
#include "dependency_injection.hpp"
#include "i_authentication_service.hpp"


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
    
    
    addTranslations();
    loadFonts();
    
    
    // Register types
    qmlRegisterSingletonType(QUrl(u"qrc:/StyleSheet.qml"_qs), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl(u"qrc:/IconSheet.qml"_qs), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl(u"qrc:/Globals.qml"_qs), "Librum.globals", 1, 0, "Globals");
    
    auto authenticationController = config::diConfig().create<adapters::IAuthenticationController*>();
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController", authenticationController);
    
    
    
    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");
    
    const QUrl url(u"qrc:/main.qml"_qs);
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
        QDirIterator it(fontsPath);
        while(it.hasNext())
        {
            QFontDatabase::addApplicationFont(it.next());
        }
    }
    else
    {
        qWarning() << "Unable to load application fonts from " + fontsPath;
    }
}