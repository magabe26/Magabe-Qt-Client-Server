/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "connectionpipe.h"
#include "Controllers/controller.h"
ConnectionPipe::ConnectionPipe()
{

}

void ConnectionPipe::registerObject(const QObject *object){
    Controller::registerObject(object);
}

void ConnectionPipe::unRegisterObject(const QObject *object){
    Controller::unRegisterObject(object);
}

bool ConnectionPipe::isSignalRegistered(const QString &className, const QString &signalName)
{
    return Controller::isSignalRegistered(className,signalName);
}

bool ConnectionPipe::emitSignal(const QString &senderClassName, const QString &signalName, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9){
    return Controller::emitSignal(senderClassName,signalName,val0, val1,val2,val3,val4,val5,val6,val7,val8,val9);
}

bool ConnectionPipe::connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName){
    return Controller::connectSlot(senderClassName,signalName,receiver,slotName);
}

bool ConnectionPipe::connectSlot(const QObject *sender, const QString &signalName, const QObject *receiver, const QString &slotName)
{
    if(sender && (receiver)){
        return Controller::connectSlot(sender->metaObject()->className(),signalName,receiver,slotName);
    }else{
        return false;
    }
}

QStringList ConnectionPipe::getSignalClassNames(const QString &signalName)
{
   return Controller::getSignalClassNames(signalName);
}

bool ConnectionPipe::connectSignals(const QString &senderClassName, const QString &senderSignalName, const QString &receiverClassName, const QString &receiverSignalName)
{
    return Controller::connectSignals(senderClassName,senderSignalName,receiverClassName, receiverSignalName);
}

bool ConnectionPipe::connectSignals(const QString &senderClassName, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName)
{
    if(receiver){
        return Controller::connectSignals(senderClassName,senderSignalName,receiver->metaObject()->className(), receiverSignalName);
    }else{
        return false;
    }
}

bool ConnectionPipe::connectSignals(const QObject *sender, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName)
{
    if((receiver) && (sender)){
        return Controller::connectSignals(sender->metaObject()->className(),senderSignalName,receiver->metaObject()->className(), receiverSignalName);
    }else{
        return false;
    }
}

bool ConnectionPipe::isSignalRegistered(const QString &signalName)
{
    return Controller::isSignalRegistered(signalName);
}
