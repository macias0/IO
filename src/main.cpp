#include <QtQml>
#include <QtQml/QQmlApplicationEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "cpp/basichelper.h"
#include "cpp/mediator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    BasicHelper basicHelper;
    engine.rootContext()->setContextProperty("basicHelper", &basicHelper);
    Mediator mediator;
    engine.rootContext()->setContextProperty("mediator", &mediator);

    qmlRegisterType<EView>("EView", 1, 0, "EView");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
