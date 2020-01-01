/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "Client/magabeclient.h"
#include "magabeclientapp.h"
#include "magabeclientqml.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    //qmlRegisterType<TestOBJ>("com.magabe", 1, 0, "TestObj");

    MagabeClientApp magabeClientApp; //MY CLIENT APP

    MagabeClient client(CONTROLLER_IDENTIFIER_REF,0);
    client.setMaxThreadCount(15);

    QQmlApplicationEngine engine;
    MagabeClientQml::registerTypes(&engine,&magabeClientApp);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


