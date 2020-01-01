/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeserverqml.h"
#include <QtQuick/QQuickItem>
#include  "Helpers/user.h"
#include <QQmlContext>
#include "Helpers/mqmlcallback.h"
#include "Helpers/errors.h"
#include "Helpers/sort.h"

void MagabeServerQml::registerTypes(QQmlApplicationEngine *engine, MagabeServerApp *app)
{
    //UncreatableType
     qmlRegisterUncreatableType<Errors>("com.magabelab.magabeServer", 1, 0, "Errors", QStringLiteral("Errors cannot be instantiated directly"));
     qmlRegisterUncreatableType<Sort>("com.magabelab.magabeServer", 1, 0, "SortOrder", QStringLiteral("SortOrder cannot be instantiated directly"));

    if(engine && app){
        engine->rootContext()->setContextProperty("magabeServerApp",app);
    }else{
        if(!engine)
            qWarning() << QString("MagabeServerQml::registerTypes | Error ,Failed to register ContextProperty -> (%1)").arg("QQmlApplicationEngine *engine");
        if(!app)
            qWarning() << QString("MagabeServerQml::registerTypes | Error ,Failed to register ContextProperty -> (%1)").arg("MagabeServerApp *app");

    }

    //CreatableType
    qmlRegisterType<User>("com.magabelab.magabeServer", 1, 0, "User");
    qmlRegisterType<MQmlCallback>("com.magabelab.MQmlCallback", 1, 0, "MQmlCallback");

}
