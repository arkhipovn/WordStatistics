#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "controller.h"

#include "items/barchart.h"
#include "items/line.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setApplicationName("WordStatistics");
    app.setOrganizationName("Test");

    Controller controller;

    qmlRegisterType <BarChart> ("Items", 1, 0, "BarChart");
    qmlRegisterType <Line> ("Items", 1, 0, "Line");

    qmlRegisterType <WordStatistics> ("WordStatistics", 1, 0, "WordStatistics");
    qRegisterMetaType<WordList>("WordList");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("programSettings", Settings::getInstance());
    engine.rootContext()->setContextProperty("controller", &controller);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

