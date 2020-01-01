/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeclient.h"
#include <iostream>
#include <QHostAddress>
#include <QFile>
#include "security.h"
#include "authentication.h"
#include "Helpers/datawritter.h"
#include <QThread>
#include "Models/model.h"
#include "Models/statistticsmodel.h"
#include "Models/userprofilepicturemodel.h"
#include "Controllers/authenticationcontroller.h"
#include "Helpers/connectionpipe.h"
#include "registeredobjects.h"
#include "Controllers/conversationcontroller.h"
#include <QCoreApplication>


MagabeClient::MagabeClient(ControllerIdentifier *controllerIdentifier,QObject *parent) : QObject(parent),
    socket_(new QSslSocket(this))
{
    connect(QCoreApplication::instance(),&QCoreApplication::aboutToQuit,this,&MagabeClient::onAppAboutToQuit);


    //INIT NECESSARY RESOURCE FOR THE CLIENT APP TO WORK
    { CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(AuthenticationController)
                CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(ConversationController)

                //CReate Tables
                CREATE_TABLE_IF_NOT_EXISTS(StatisticsModel)//this table first ,because other model use it
                CREATE_TABLE_IF_NOT_EXISTS(CurrentUserIdModel)
                CREATE_TABLE_IF_NOT_EXISTS(AuthenticationModel)
                CREATE_TABLE_IF_NOT_EXISTS(UserProfilePictureModel)
    }


    dataProcessor_ = new DataProcessor(controllerIdentifier); //auto deleted by thread pool
    incomingDataManager_ = new IncomingDataManager(dataProcessor_,this);

    //connect signals and slots
    connect(socket_, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));

    // QSslSocket emits the encrypted() signal after the encrypted connection is established
    connect(socket_,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket_,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket_,SIGNAL(hostFound()),this,SLOT(hostFound()));
    connect(socket_,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    // connect(socket_,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(socket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(showError(QAbstractSocket::SocketError)));

    //datawritter
    DWritter::instance()->configureSocket(socket_);
    incomingDataManager_->connectSignalToSocket(socket_);
    controllerIdentifier->setSocket(socket_);

    // Check for SSL support.  If SSL support is not available, show a
    // message to the user describing what to do to enable SSL support.
    if (!QSslSocket::supportsSsl())
    {
        QString noSslMsg = QString("%1\n%2")
                .arg("*** Your version of Qt does support SSL ***")
                .arg("You must obtain a version of Qt that has SSL"
                     "support enabled.  If you believe that your "
                     "version of Qt has SSL support enabeld, you may "
                     "need to install the OpenSSL run-time libraries.");

    }

    // Read the SSL certificate
    QFile file(":/sslcertificate/churajasusi.crt");
    file.open(QIODevice::ReadOnly);
    const QByteArray bytes = file.readAll();

    // Create a certificate object
    const QSslCertificate certificate(bytes);

    // Add this certificate to all SSL connections
    QSslSocket::addDefaultCaCertificate(certificate);
    //
    socket_->addCaCertificate(certificate);

    //firstregister
    ConnectionPipe::registerObject(this);

    //connect to MagabeClientApp signals
    ConnectionPipe::connectSignals(RegisteredObjects::MagabeClient::className,RegisteredObjects::MagabeClient::Signals::connectTimeout,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::connectTimeout);

    //connect slots
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeClientManager::className,RegisteredObjects::MagabeClientManager::Signals::connectToServer,this,RegisteredObjects::MagabeClient::Slots::onConnectToServer);
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeClientManager::className,RegisteredObjects::MagabeClientManager::Signals::disconnectFromServer,this,RegisteredObjects::MagabeClient::Slots::disconnect);

}

/**
 * @brief MagabeClient::~MagabeClient
 */
MagabeClient::~MagabeClient()
{
    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    if (socket_->isOpen())
    {
        socket_->close();
    }

    ConnectionPipe::unRegisterObject(this);
}

void MagabeClient::setMaxThreadCount(int maxThreadCount)
{
    QThreadPool::globalInstance()->setMaxThreadCount(maxThreadCount);

}

/**
 * @brief MagabeClient::connectToMagabeServer
 * @param ipAddress
 * @param port
 */
void MagabeClient::connectToMagabeServer(QString ipAddress, quint16 port,int msecsTimeout)
{
    socket_->connectToHostEncrypted(ipAddress,port);
    if( !socket_->waitForConnected(msecsTimeout)){
        emit connectTimeout();
        // print("Connect timeout");
    }
}

/**
 * @brief MagabeClient::connected
 */
void MagabeClient::connected()
{
    // print("Connected to server.");
    emit connectedToMagabeserver();
}

/**
 * @brief MagabeClient::disconnected
 */
void MagabeClient::disconnected()
{
    // print("Disconnected");

    DWritter::instance()->clearDataToSend();

    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    emit disconnectedFromMagabeServer();

}

/**
 * @brief MagabeClient::hostFound
 */
void MagabeClient::hostFound()
{
    //ConPipe::instance()->print("ChuraJasusi:: HostFound.");
}

/**
 * @brief MagabeClient::bytesWritten
 * @param bytes
 */
void MagabeClient::bytesWritten(qint64 bytes)
{
    //  print("Bytes: >"+QString::number(bytes)+"<>writtern");
}

/**
 * @brief MagabeClient::readyRead
 */
//void MagabeClient::readyRead()
//{
//    QByteArray data = socket_->readAll();

//}

/**
 * @brief MagabeClient::showError
 * @param e
 */
void MagabeClient::showError(QAbstractSocket::SocketError e)
{
    QString error;
    if(e == QAbstractSocket::SocketAddressNotAvailableError ){
        error =" Error :: SocketAddressNotAvailableError";
    } else if(e == QAbstractSocket::ConnectionRefusedError ){
        error =" Error :: ConnectionRefusedError";
    }else if(e == QAbstractSocket::RemoteHostClosedError ){
        error =" Error :: RemoteHostClosedError";
    }else if(e == QAbstractSocket::HostNotFoundError){
        error =" Error :: HostNotFoundError";
    }else if(e == QAbstractSocket::SocketAccessError){
        error =" Error :: SocketAccessError";
    }else if(e == QAbstractSocket::SocketResourceError){
        error =" Error :: SocketResourceError";
    }else if(e == QAbstractSocket::SocketTimeoutError){
        error =" Error :: SocketTimeoutError";
    }else if(e == QAbstractSocket::NetworkError){
        error =" Error :: NetworkError";
    }else if(e == QAbstractSocket::AddressInUseError){
        error =" Error :: AddressInUseError";
    }else{
        error ="unknown Error ";
    }
    print(error);
}

/**
 * @brief MagabeClient::isValidCertificate
 * @return
 */
bool MagabeClient::isValidCertificate()
{
    foreach (QSslCertificate Certificate, socket_->caCertificates()) {
        if(socket_->peerCertificate() == Certificate){
            return true;
        }
    }
    return false;

}

/**
 * @brief MagabeClient::sslErrors
 * @param errors
 */
void MagabeClient::sslErrors(const QList<QSslError> &errors)
{
    QString errorStrings;
    foreach (QSslError error, errors)
    {
        errorStrings += error.errorString();
        if (error != errors.last())
        {
            errorStrings += '\n';
        }
    }

    if(isValidCertificate()){
        //print("This is a Valid ssl Certificate");
        socket_->ignoreSslErrors(); //allow ssl handshake when there ssl errors
    }else{
        print("This is a not Valid ssl Certificate");
    }
}

/**
 * @brief MagabeClient::onConnectToServer
 * @param ip
 * @param port
 */
void MagabeClient::onConnectToServer(QString ip, QString port, int msecsTimeout)
{
    connectToMagabeServer(ip,port.toUInt(),msecsTimeout);
}

/**
 * @brief MagabeClient::disconnect
 */
void MagabeClient::disconnect()
{
    socket_->close();
}

void MagabeClient::onAppAboutToQuit(){
    qDebug() << "MagabeClient::onAboutToQuit called";
    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    disconnect();
}

/**
 * @brief MagabeClient::print
 * @param text
 */
void MagabeClient::print(QString text)
{
    qDebug() << text;
}




/**  -----------------DataProcessor----------------------------
 * @brief DataProcessor::DataProcessor
 * @param controllerIdentifier
 */
DataProcessor::DataProcessor(ControllerIdentifier *controllerIdentifier) : QObject(nullptr)
  ,dataFilter_(new DataFilter(this))
  ,running_(false)
{
    setAutoDelete(true);
    controllerIdentifier_ = controllerIdentifier;
}

void DataProcessor::run(){
    while (running_) {
        CleanData  d =  dataFilter_->getCleanData();
        if(!running_){
            break;
        }
        QMetaObject::invokeMethod(controllerIdentifier_
                                  ,"onDataReceivedFromDataProcessor"
                                  ,Q_ARG(QByteArray,*d.cleanData));
    }
}

void DataProcessor::start(){
    running_ = true;
    QThreadPool::globalInstance()->start(this,1);
}

void DataProcessor::stop(){
    if(running_){
        running_ = false;
        dataFilter_->stopFilter();
    }
}

DataFilter *DataProcessor::dataFilter() const
{
    return dataFilter_;
}

void DataProcessor::startDataProcessing(const QByteArray &data){
    if(!running_){
        start();
    }
    dataFilter_->appendToBuffer(data,"ActionAuthenticate");
}


/** -----------------IncomingDataManager---------------------------
 * @brief IncomingDataManager::IncomingDataManager
 * @param dataProcessor
 * @param parent
 */
IncomingDataManager::IncomingDataManager(DataProcessor *dataProcessor, QObject *parent)
    : QThread(parent)
    ,running_(false)
{
    dataProcessor_ = dataProcessor;
    filiter_ = dataProcessor->dataFilter();
}

void IncomingDataManager::connectSignalToSocket(QSslSocket *socket){
    if(socket)
        connect(socket, SIGNAL(readyRead()), this,SLOT(onMagabeClientReadyRead()));

}

void IncomingDataManager::stop(){
    dataReceiced_.wakeAll();
    running_ = false;
}

void IncomingDataManager::run(){
    while (running_) {

        mutex_.lock();

        while(receivedData_.isEmpty() && (running_))
            dataReceiced_.wait(&mutex_);

        if(!running_){
            mutex_.unlock();
            break;
        }

        QByteArray data = receivedData_.dequeue();
        if(!data.isEmpty()){
            QMetaObject::invokeMethod(dataProcessor_
                                      ,"startDataProcessing"
                                      ,Q_ARG(QByteArray,data));
        }

        mutex_.unlock();

    }
}

void IncomingDataManager::onMagabeClientReadyRead()
{
    if(!isRunning()){
        start();
    }

    if(!running_){
        running_ = isRunning();
    }

    QMutexLocker locker(&mutex_);
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    if(client){
        receivedData_.enqueue(client->readAll());
        dataReceiced_.wakeOne();
    }
}
