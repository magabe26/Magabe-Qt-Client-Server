/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "datawritter.h"
#include "debug.h"
#include "Helpers/connectionpipe.h"
#include "registeredobjects.h"

namespace Mgb {

DataWritter::DataWritter(QObject *parent) : QObject(parent)
{
    ConnectionPipe::registerObject(this);

    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServer::className,RegisteredObjects::MagabeServer::Signals::serverStoped,this,RegisteredObjects::Mgb_DataWritter::Slots::onServerStoped);

}

/**
   * @brief DataWritter::~DataWritter
   */
DataWritter::~DataWritter()
{
    ConnectionPipe::unRegisterObject(this);
    clean();
}

/**
   * @brief DataWritter::queueData
   * @param data
   * @param mgbSessionId
   */
void DataWritter::queueData(MagabeData& data, const QString &mgbSessionId)
{
    QSslSocket *client = const_cast<QSslSocket *>(ConnectedUserList::instance()->getSocket(mgbSessionId));

    if(data.isEmpty() || (!client))return;

    if(!dataToSend_.contains(mgbSessionId)){
        dataToSend_.insert(mgbSessionId,new QQueue<MagabeData>());
        connect(client,&QSslSocket::disconnected,this,&DataWritter::onClientDisconnected);
    }

    if(!dataToSend_.value(mgbSessionId)->contains(data)){ //avoild duplicate
        dataToSend_.value(mgbSessionId)->enqueue(data);
    }

}

/**
   * @brief DataWritter::dequeueData
   * @param mgbSessionId
   * @return
   */
MagabeData DataWritter::dequeueData(const QString &mgbSessionId)
{
    return (hasDataToSend(mgbSessionId))
            ? dataToSend_.value(mgbSessionId)->dequeue()
            : MagabeData();
}


/**
   * @brief DataWritter::hasDataToSend
   * @param mgbSessionId
   * @return
   */
bool DataWritter::hasDataToSend(const QString &mgbSessionId)
{

    return (dataToSend_.contains(mgbSessionId)
            && (dataToSend_.value(mgbSessionId))
            && (!dataToSend_.value(mgbSessionId)->isEmpty()));
}

/**
   * @brief DataWritter::startDataSending
   * @param mgbSessionId
   */
void DataWritter::startDataSending(const QString &mgbSessionId)
{

    if(hasDataToSend(mgbSessionId)){
        QSslSocket *client = const_cast<QSslSocket *>(ConnectedUserList::instance()->getSocket(mgbSessionId));
        if(!client){//happens when user has disconnected and onClientDisconnected did not triggered
            clearDataFor(mgbSessionId);
            return;
        }
        ConnectedUserList::instance()->send(dequeueData(mgbSessionId),mgbSessionId);
        startDataSending(mgbSessionId); //using recursion ,start again for this mgbSessionId
    }
}

void DataWritter::onClientDisconnected()
{
    //print("DataWritter::onClientDisconnected");
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    if(client){
        QString mgbSessionId = ConnectedUserList::instance()->getMgbSessionId(client);
        clearDataFor(mgbSessionId);
        client->disconnect(this);
    }
}

void DataWritter::onServerStoped()
{
   // qDebug() << "DataWritter::onServerStoped";
    clean();
}


/**
   * @brief DataWritter::sendData
   * @param data
   * @param mgbSessionId
   */
void DataWritter::sendData(MagabeData &data, const QString &mgbSessionId)
{
    queueData(data,mgbSessionId);
    if(hasDataToSend(mgbSessionId))//check if is still not empty
        startDataSending(mgbSessionId);
}


void DataWritter::queueBypassSend(MagabeData &data, const QString &mgbSessionId)
{
    ConnectedUserList::instance()->send(data,mgbSessionId);

}
/**
   * @brief DataWritter::sendData
   * @param jsonData
   * @param mgbSessionId
   */
void DataWritter::sendData(const QString &jsonData, const QString &mgbSessionId)
{
    MagabeData data(jsonData);
    sendData(data,mgbSessionId);
}


/**
   * @brief DataWritter::removeQueue
   * @param mgbSessionId
   */
void DataWritter::clearDataFor(const QString &mgbSessionId)
{
    if(dataToSend_.contains(mgbSessionId)){
        QQueue<MagabeData>* toRemove = dataToSend_.value(mgbSessionId,nullptr);
        dataToSend_.remove(mgbSessionId); //remove first,help prevent crashes
        if(toRemove){ //if is valid pointer
            toRemove->clear();
            delete toRemove;
            toRemove = nullptr;

#if PRINT_DATA_SENT
            print(QString("Data to send for [mgbSessionId = %1] cleared").arg(mgbSessionId));
#endif

        }
    }

}

void DataWritter::print(QString text)
{
    qDebug()<< text;
}


/**
   * @brief DataWritter::clean
   */
void DataWritter::clean()
{
    QMapIterator<QString,QQueue<MagabeData>*> i(dataToSend_);
    while (i.hasNext()) {
        i.next();
        auto queue =  i.value();
        dataToSend_.remove(i.key());//remove first
        if(queue){
            queue->clear();
            delete queue;
            queue = nullptr;
        }
    }
}

}
