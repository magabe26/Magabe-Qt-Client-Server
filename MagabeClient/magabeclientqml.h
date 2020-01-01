/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABECLIENTQML_H
#define MAGABECLIENTQML_H
#include <QQmlApplicationEngine>
#include <magabeclientapp.h>

namespace MagabeClientQml{

void registerTypes(QQmlApplicationEngine *engine,MagabeClientApp *app);

}

#endif // MAGABECLIENTQML_H
