/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABESERVERQML_H
#define MAGABESERVERQML_H
#include <QQmlApplicationEngine>
#include "magabeserverapp.h"

namespace MagabeServerQml
{
void registerTypes(QQmlApplicationEngine *engine,MagabeServerApp *app);
}

#endif // MAGABESERVERQML_H
