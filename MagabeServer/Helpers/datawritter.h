/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef DATAWRITTER_H
#define DATAWRITTER_H

#include <QObject>
#include <QQueue>
#include "Helpers/magabedata.h"
#include <QMap>
#include <QSslSocket>
#include "Server/connectedusers.h"
#include "Helpers/magabesingleton.h"

namespace Mgb {
/**
 * @brief The DataWritter class , writes to all client passes through a single queue
 */
class DataWritter : public QObject
{
    Q_OBJECT
public:
    explicit DataWritter(QObject *parent = nullptr);
    ~DataWritter();
    void sendData(MagabeData& data,const QString &mgbSessionId);
    //send the data immediatry and bypass the queue
    //used for files
    void queueBypassSend(MagabeData &data, const QString &mgbSessionId);
    bool hasDataToSend(const QString &mgbSessionId);

private slots:
    void sendData(const QString &jsonData,const QString &mgbSessionId);
    void startDataSending(const QString &mgbSessionId);
    void onClientDisconnected();
    void onServerStoped();//connected do not change
private:
    void queueData(MagabeData &data,const QString &mgbSessionId);
    MagabeData dequeueData(const QString &mgbSessionId);
    void clearDataFor(const QString &mgbSessionId);
    void print(QString text);
    void clean();
    QMap<QString,QQueue<MagabeData>*> dataToSend_; //mgbSession,data

};

}

typedef MagabeSingleton<Mgb::DataWritter> DWritter;
#endif // DATAWRITTER_H
