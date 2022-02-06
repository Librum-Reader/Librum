#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLocale>
#include <QTranslator>
#include <QString>
#include "testrequest.hpp"


int main(int argc, char *argv[])
{    
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Etovex");
    app.setOrganizationDomain("Etovex.com");
    app.setApplicationName("Librum");
    
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Librum_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    
    TestRequest testRequest;
    
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(&testRequest);
    
    const QUrl url(u"qrc:/Librum/src/app/view/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
