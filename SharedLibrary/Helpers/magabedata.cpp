/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabedata.h"
#include <QDebug>

//the class works dont change anathing
MagabeData::MagabeData()
    :Extras()
{

}

MagabeData::MagabeData(const QString &data)
    :Extras()
{
    Extras extras(data.toLatin1());
    if(extras.isValid()){
        setData(extras.toByteArray());

    }
}

MagabeData::MagabeData(const QString &controller, const QString &action){
    if(!controller.isEmpty())
        setController(controller);

    if(!action.isEmpty())
        setAction(action);
}

MagabeData &MagabeData::setController(const QString &name){
    addElement(CONTROLLER_SIGNATURE,'C',name);
    return *this;
}

QString MagabeData::controller(){
    return getElement(CONTROLLER_SIGNATURE,'C');
}

MagabeData &MagabeData::setAction(const QString &name){
    addElement(ACTION_SIGNATURE,'A',name);
    return *this;
}

QString MagabeData::action(){
    return getElement(ACTION_SIGNATURE,'A');
}
