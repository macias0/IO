#include <QtQml>
#include <QtQml/QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "cpp/mediator.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Mediator mediator;
    engine.rootContext()->setContextProperty("mediator", &mediator);

    qmlRegisterType<EView>("EView", 1, 0, "EView");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
