#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tictactoegame.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    TicTacToeGame tictactoegame;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("tictactoegame", &tictactoegame);

    const QUrl url(QStringLiteral("qrc:/xo/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
