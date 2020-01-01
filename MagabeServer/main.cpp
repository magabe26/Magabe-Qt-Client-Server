/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include "Server/MagabeServer.h"
#include "magabeserverapp.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "magabeserverqml.h"
#include "Models/messagesmodel.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    MagabeServerApp mServerApp; //MY SERVER APP

    MagabeServer server(CONTROLLER_IDENTIFIER_REF,0);
    server.setMaxThreadCount(15);

    QQmlApplicationEngine engine;
    MagabeServerQml::registerTypes(&engine,&mServerApp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
