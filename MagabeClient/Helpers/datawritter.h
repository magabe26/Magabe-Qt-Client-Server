/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef DATAWRITTER_H
#define DATAWRITTER_H

#include <QObject>
#include <QSslSocket>
#include "Helpers/magabedata.h"
#include "Helpers/magabesingleton.h"
#include <QQueue>
#include "Helpers/constants.h"
#include <QTimer>

class SimpleCrypt;
namespace Mgb {

class DataWritter : public QObject
{
    Q_OBJECT
public:
    explicit DataWritter(QObject *parent = nullptr);
    void sendData(MagabeData& data);
    void configureSocket(QSslSocket *socket);
    void clearDataToSend();
    ~DataWritter();
signals:

public slots:
    bool hasDataToSend();
    void sendData(QString data);

private slots:
    void startDataSending();
private:
    void queueData(MagabeData& data);
    MagabeData dequeueData();
    void write(MagabeData data);
    QSslSocket *socket_;
    QQueue<MagabeData>* dataToSend_; //socketdescriptor,data
    void print(QString status);
    QTimer *timer_;
    SimpleCrypt *crypto_;
};

}

typedef MagabeSingleton<Mgb::DataWritter> DWritter;
#endif // DATAWRITTER_H
