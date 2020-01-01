/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include "mqmlcallback.h"
#include <QTimer>
MQmlCallback::MQmlCallback(QObject *parent)
    : QObject(parent)
    ,tm_(new QTimer(this))
    ,isProgressCallback_(false)
{
    tm_->setSingleShot(true);
    connect(tm_,&QTimer::timeout,this,&MQmlCallback::timeout);
}

void MQmlCallback::startTimer(int timeoutInXMilliseconds)
{
    if(isProgressCallback_){
        return;
    }
    if(tm_->isActive())tm_->stop();

    if(timeoutInXMilliseconds < 60000){//can not be less than 1min
        timeoutInXMilliseconds =  60000;
    }
    tm_->start(timeoutInXMilliseconds);
}

void MQmlCallback::stopTimer()
{
    if(tm_->isActive())tm_->stop();
}

bool MQmlCallback::timerIsRunning()
{
    return tm_->isActive();
}

bool MQmlCallback::call(const QVariant &data, int dataError, const MQmlCallback::MQmlCallbackError mQmlCallbackError){
    return metaObject()->invokeMethod(this,"jsFunctionCaller"
                                      ,Q_ARG(QVariant,data)
                                      ,Q_ARG(QVariant,dataError)
                                      ,Q_ARG(QVariant,(int)mQmlCallbackError));

}

bool MQmlCallback::isProgressCallback() const
{
    return isProgressCallback_;
}

void MQmlCallback::setIsProgressCallback(bool isProgressCallback)
{
    if(isProgressCallback_ != isProgressCallback){
        isProgressCallback_ = isProgressCallback;
        emit isProgressCallbackChanged(isProgressCallback_);
    }
}
