#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>
#include <QString>
#include "qdiriterator.h"
#include "qfontdatabase.h"
#include "qqml.h"
#include "sidebar_state.hpp"
#include "test_request.hpp"
#include "test_class.hpp"


int main(int argc, char *argv[])
{    
    // App
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Etovex");
    QGuiApplication::setOrganizationDomain("Etovex.com");
    QGuiApplication::setApplicationName("Librum");

    
    // Translations
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



    // Loading fonts
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



    // Type registering
    qmlRegisterSingletonType(QUrl(u"qrc:/sources/src/presentation/StyleSheet.qml"_qs), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl(u"qrc:/sources/src/presentation/IconSheet.qml"_qs), "Librum.icons", 1, 0, "Icons");
    
    SidebarState sidebarState;
    qmlRegisterSingletonInstance("Librum.extensions.sidebar", 1, 0, "SidebarState", &sidebarState);

    
    
    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/sources/src/presentation/modules");
    
    const QUrl url(u"qrc:/sources/src/presentation/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
}