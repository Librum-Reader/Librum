#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlContext>
#include <QTranslator>
#include <QLocale>
#include <QString>

int main(int argc, char *argv[])
{    
    // App
    QGuiApplication app(argc, argv);
    app.setOrganizationName("Etovex");
    app.setOrganizationDomain("Etovex.com");
    app.setApplicationName("Librum");
    
    
    // Translations
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Librum_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    
    
    // Startup
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/sources/src/app/view/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    
    return app.exec();
}
