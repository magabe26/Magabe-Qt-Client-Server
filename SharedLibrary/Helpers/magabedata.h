/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef MAGABEDATA_H
#define MAGABEDATA_H
#include "extras.h"

class MagabeData : public Extras
{
public:
    MagabeData();
    explicit MagabeData(const QString &data);
    MagabeData(const QString &controller,const QString &action);
    MagabeData &setController(const QString &name);
    QString controller();
    MagabeData &setAction(const QString &name);
    QString action();
};

#endif // MAGABEDATA_H
