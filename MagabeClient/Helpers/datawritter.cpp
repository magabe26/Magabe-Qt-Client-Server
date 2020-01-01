/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "datawritter.h"
#include "debug.h"
#include "security.h"


namespace Mgb {

DataWritter::DataWritter(QObject *parent)
    : QObject(parent)
    ,socket_(nullptr)
    ,dataToSend_(new QQueue<MagabeData>())
    ,timer_(new QTimer(this))

{
    crypto_ = Security::instance();
    timer_->setInterval(50);//data are writern at 50msec  interval
    connect(timer_,&QTimer::timeout,this,&DataWritter::startDataSending);
}

/**
   * @brief DataWritter::~DataWritter
   */
DataWritter::~DataWritter()
{
    if(timer_->isActive())
        timer_->stop();

    if(dataToSend_){
        dataToSend_->clear();
        delete dataToSend_;
        dataToSend_ = nullptr;
    }
}

/**
   * @brief DataWritter::queueData
   * @param data
   */
void DataWritter::queueData(MagabeData& data)
{
    if(!data.isEmpty() && !dataToSend_->contains(data))
        dataToSend_->enqueue(data);
}

/**
   * @brief DataWritter::dequeueData
   * @return
   */
MagabeData DataWritter::dequeueData()
{
    return hasDataToSend() ? dataToSend_->dequeue() : MagabeData();
}


/**
   * @brief DataWritter::hasDataToSend
   * @return
   */
bool DataWritter::hasDataToSend()
{
    return ((dataToSend_) && (!dataToSend_->isEmpty()));
}

void DataWritter::sendData(QString data){
    MagabeData d(data);
    sendData(d);
}

/**
   * @brief DataWritter::startDataSending
   */
void DataWritter::startDataSending()
{
    if(hasDataToSend()){
        write(dequeueData());
    } else{
        if(timer_->isActive())
            timer_->stop();
    }
}

/**
   * @brief DataWritter::sendData
   * @param data
   */
void DataWritter::sendData(MagabeData &data)
{
    if(socket_){
        queueData(data);
        if(!timer_->isActive()){
            timer_->start();
        }
    }
}

/**
   * @brief DataWritter::setSocket
   * @param socket
   */
void DataWritter::configureSocket( QSslSocket* socket)
{
    if(socket){
        socket_ = socket;
    }
}

/**
   * @brief DataWritter::removeQueue
   */
void DataWritter::clearDataToSend()
{
    if(timer_->isActive())
        timer_->stop();

    if(dataToSend_)
        dataToSend_->clear();
}



/**
   * @brief DataWritter::writeToClient
   * @param data
   */
void DataWritter::write(MagabeData data)
{
    if((socket_) && !data.isEmpty()){
        QString dataForWriting = crypto_->encryptToString(data.toByteArray()).append("\r\n");
        if(socket_->write(dataForWriting.toLatin1()) != -1){

#if PRINT_DATA_SENT
            print("Data sent to sever"+data.toByteArray());
#endif
        }else{
            print("Error ,unable to send data");
        }
    }

}

void DataWritter::print(QString status)
{
    qDebug()<< status;
}

}
