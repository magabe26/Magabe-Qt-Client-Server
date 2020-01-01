/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "datafilter.h"
#include <QDebug>
DataFilter::DataFilter(QObject *parent) : QObject(parent),
    buffer_(new QByteArray())
  ,filterStoped_(false)
{

}

void DataFilter::appendToBuffer(const QByteArray &dirtyData,const QString mgbSessionId)
{
    if(dirtyData.isNull())
        return;

    mutex_.lock();

    buffer_->append(dirtyData);

    QByteArray clean = getValidData();
    bool added = false;
    do{
        if(!clean.isEmpty()){
            CleanData d;
            d.mgbSessionId = mgbSessionId;
            d.cleanData = QSharedPointer<QByteArray>(new QByteArray(clean));
            cleanDataQueue_.enqueue(d);
            added = true;
        }
        clean = getValidData();
    }while (!clean.isEmpty());

    if(added)
        cleanDataAvailable_.wakeOne();

    mutex_.unlock();
}

CleanData DataFilter::getCleanData()
{
    mutex_.lock();
    if(filterStoped_){
        filterStoped_= false;
    }
    while (cleanDataQueue_.isEmpty() && (!filterStoped_)) {
        cleanDataAvailable_.wait(&mutex_);
    }
    CleanData data;
    if((!filterStoped_) && (!cleanDataQueue_.isEmpty())){
        data = cleanDataQueue_.dequeue();
    }
    mutex_.unlock();
    return data;
}

void DataFilter::stopFilter(){
    filterStoped_ = true;
    cleanDataAvailable_.wakeAll();
}

bool DataFilter::hasValidData(QByteArray * const buffer, int *r_index, int *n_index){
    *r_index = -1;
    *n_index = -1;

    if(!buffer){
        return false;
    }

    int i_r = buffer->indexOf('\r');
    int i_n = -1;
    if(i_r == -1){
        return false;
    }else{
        i_n = buffer->indexOf('\n',i_r);
    }

    if((i_n - i_r) == 1){
        *r_index = i_r;
        *n_index = i_n;
        return true;
    }else{
        return false;
    }

}

QByteArray DataFilter::getValidData(){
    QByteArray data;
    int r_index;
    int n_index;

    if(hasValidData(buffer_,&r_index,&n_index)){
        if((buffer_) && (r_index != -1) && (n_index != -1)){
            data = buffer_->left(r_index);
            int rmvlen = data.size() +2; //2 for \r\n
            buffer_->remove(0,rmvlen);
        }
    }
    return data;
}
