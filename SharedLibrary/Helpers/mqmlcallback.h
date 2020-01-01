/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MQMLCALLBACK_H
#define MQMLCALLBACK_H

#include <QObject>
#include <QVariant>

class QTimer;
class MQmlCallback : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isProgressCallback READ isProgressCallback WRITE setIsProgressCallback NOTIFY isProgressCallbackChanged)
public:

    enum  MQmlCallbackError{
        NoError = 0,
        CallbackCreationFailed = 1,
        Timeout = 2
    };

    Q_ENUM(MQmlCallbackError)

    explicit MQmlCallback(QObject *parent = nullptr);
    Q_INVOKABLE void startTimer(int timeoutInXMilliseconds);
    Q_INVOKABLE void stopTimer();
    Q_INVOKABLE bool timerIsRunning();
    bool call(const QVariant& data,int dataError
              ,const MQmlCallbackError mQmlCallbackError = MQmlCallbackError::NoError);
    bool isProgressCallback() const;
    void setIsProgressCallback(bool isProgressCallback);

signals:
    void timeout();
    void jsFunctionCaller(QVariant data,QVariant  dataError,QVariant  mQmlCallbackError);
    void isProgressCallbackChanged(bool isProgressCallback);
private:
    QTimer *tm_;
    bool isProgressCallback_;//if  is true , will not automatically be destroyed. must be destroyed in your function
};

#endif // MQMLCALLBACK_H
