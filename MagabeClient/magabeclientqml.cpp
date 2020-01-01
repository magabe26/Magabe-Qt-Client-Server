/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeclientqml.h"
#include <QtQuick/QQuickItem>
#include "magabeclientapp.h"
#include "Helpers/userinfo.h"
#include <QQmlContext>
#include "Helpers/mqmlcallback.h"
#include "Helpers/errors.h"
#include "Helpers/sort.h"


void MagabeClientQml::registerTypes(QQmlApplicationEngine *engine,MagabeClientApp *app)
{
    //UncreatableType
     qmlRegisterUncreatableType<Errors>("com.magabelab.magabeClient", 1, 0, "Errors", QStringLiteral("Errors cannot be instantiated directly"));
     qmlRegisterUncreatableType<Sort>("com.magabelab.magabeClient", 1, 0, "SortOrder", QStringLiteral("SortOrder cannot be instantiated directly"));

    if(engine && app){
        engine->rootContext()->setContextProperty("magabeClientApp",app);
    }else{
        if(!engine)
            qWarning() << QString("MagabeClientQml::registerTypes| Error ,Failed to register ContextProperty -> (%1)").arg("QQmlApplicationEngine *engine");
        if(!app)
            qWarning() << QString("MagabeClientQml::registerTypes | Error ,Failed to register ContextProperty -> (%1)").arg("MagabeClientApp *app");
    }
    //CreatableType
    qmlRegisterType<UserInfo>("com.magabelab.magabeClient", 1, 0, "UserInfo");
    qmlRegisterType<MQmlCallback>("com.magabelab.MQmlCallback", 1, 0, "MQmlCallback");

}
