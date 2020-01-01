/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "MagabeServer.h"
#include <iostream>
#include <QMetaObject>
#include <sstream>
#include <cassert>
#include <QDateTime>
#include <QFileInfo>
#include "Helpers/simplecrypt.h"
#include "security.h"
#include "Server/connectedusers.h"
#include "Helpers/connectionpipe.h"
#include "Controllers/usercontroller.h"
#include "Models/statistticsmodel.h"
#include "Models/serverfilesmodel.h"
#include "Models/messagesmodel.h"
#include "debug.h"
#include "registeredobjects.h"
#include "Controllers/conversationcontroller.h"
#include <QCoreApplication>

/** ------------------------MagabeServer---------------------------
 * @brief MagabeServer::MagabeServer
 * @param parent
 */
MagabeServer::MagabeServer(ControllerIdentifier *controllerIdentifier, QObject *parent)
    :QObject(parent)
    ,key_(nullptr)
    ,certificate_(nullptr)
    ,sslServer_(new SslServer())
    ,invalidFileMsg_(Security::instance()->decryptToString(QString("AwJ4si28K93hKGYyOs5AsZoHUg0Dsz3A4zs8bWfKBfvaAwRdV/FqnLVhJX5l1BvixBVXHlHocp6lOH0ibcpVr4NXFkVP/z4=")))

{
    //set key and  certificate file
    setCertificateAndKeyFile();

    connect(QCoreApplication::instance(),&QCoreApplication::aboutToQuit,this,&MagabeServer::onAppAboutToQuit);

    //INIT RESOURCES FOR THE APP TO WORK HERE
    //register controllers
    {
        CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(UserController)
                CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(ConversationController)

                //CReate Tables
                CREATE_TABLE_IF_NOT_EXISTS(StatisticsModel)//this table first ,because other model use it
                CREATE_TABLE_IF_NOT_EXISTS(UserModel)
                CREATE_TABLE_IF_NOT_EXISTS(ServerFilesModel)
                CREATE_TABLE_IF_NOT_EXISTS(MessagesModel)

    }

    assert(controllerIdentifier);

    dataProcessor_ = new DataProcessor(controllerIdentifier); //auto deleted by thread pool
    incomingDataManager_ = new IncomingDataManager(dataProcessor_,this);
    //connect
    connect(sslServer_,SIGNAL(newConnection()),this,SLOT(newConnection()));


    connect(this,&MagabeServer::serverStoped,ConnectedUserList::instance(),&Mgb::ConnectedUsers::onServerStoped);
    connect(ConnectedUserList::instance(),&Mgb::ConnectedUsers::startAuthenticaion,this,&MagabeServer::onStartAuthenticaion);
    connect(controllerIdentifier,&ControllerIdentifier::closeClientSocket,this,&MagabeServer::onCloseClientSocket);

    //
    ConnectionPipe::registerObject(this);

    //connect to MagabeServerManager
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServerManager::className,RegisteredObjects::MagabeServerManager::Signals::startServer,this ,RegisteredObjects::MagabeServer::Slots::startServer);
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServerManager::className,RegisteredObjects::MagabeServerManager::Signals::stopServer,this ,RegisteredObjects::MagabeServer::Slots::stopServer);

}


/**
 * @brief MagabeServer::~MagabeServer
 */
MagabeServer::~MagabeServer()
{
    ConnectionPipe::unRegisterObject(this);

    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    if((sslServer_) && sslServer_->isListening()){

        sslServer_->close();
        sslServer_->deleteLater();
    }

    if(certificate_){
        delete certificate_;
        certificate_ = nullptr;
    }

    if(key_){
        delete key_;
        key_ = nullptr;
    }
}

void MagabeServer::setMaxThreadCount(int maxThreadCount){
    QThreadPool::globalInstance()->setMaxThreadCount(maxThreadCount);
}


/**
 * @brief MagabeServer::startServer
 * @param ipAddress
 * @param port
 */
void MagabeServer::startServer(QHostAddress ipAddress, quint16 port)
{
    sslServer_->setMaxPendingConnections(50);
    sslServer_->listen(ipAddress,port);


    if(sslServer_->isListening()){
        emit serverStarted(ipAddress,port);
        print("MagabeServer imeanza...");
    }else{
        print("MagabeServer imeshindwa kuanza...");
    }

}

/**
 * @brief MagabeServer::setCertificateAndKeyFile
 */
void MagabeServer::setCertificateAndKeyFile()
{
    //QString certificateFilename = ":/CertificatesAndKey/churajasusi.crt_Encrypted";
    //QString keyFilename = ":/CertificatesAndKey/churajasusi.key_Encrypted";
    QString certificateFilename = Security::instance()->decryptToString(QString("AwJ4+OIMzBA69ab1/EHNMwjQhP/+TuoQJrTworgekGVLhdeemGLuAzjauOzgRtA/BNyf"));
    QString keyFilename = Security::instance()->decryptToString(QString("AwI+s+8BwR03+Kv48UzAPgXdifLzQ+cdK7n9r7UTnWhGiNqTlW/rESfFp/P/Wc8gG8OA"));

    // Read the SSL certificate
    QFile fileCerticate(certificateFilename);
    fileCerticate.open(QIODevice::ReadOnly);
    const QByteArray DecrptedCrtbytes = Security::instance()->decryptToByteArray(fileCerticate.readAll());

    //std::cout <<DecrptedCrtbytes.toStdString() <<std::endl;

    // Create a certificate object and set
    certificate_ = new QSslCertificate(DecrptedCrtbytes);

    // Read the SSL key
    QFile fileKey(keyFilename);
    fileKey.open(QIODevice::ReadOnly);
    const QByteArray DecrptedKeybytes = Security::instance()->decryptToByteArray(fileKey.readAll()) ;

    //std::cout << DecrptedKeybytes.toStdString() <<std::endl;

    // Create a key object and set
    key_ = new  QSslKey(DecrptedKeybytes,QSsl::Rsa);

    //just show errors
    checkFileStatus(keyFilename,certificateFilename);

}


/**
 * @brief ChuraServer::checkFileStatus
 * @param KeyFile
 * @param certificateFile
 */
void MagabeServer::checkFileStatus(QString KeyFile,QString certificateFile)
{
    QFileInfo keyInfo(KeyFile);
    QFileInfo certificateInfo(certificateFile);

    if (!keyInfo.exists() && !keyInfo.isReadable() &&
            !certificateInfo.exists() && !certificateInfo.isReadable())
    {

        print(invalidFileMsg_);
    }
}


/**
 * @brief ChuraServer::newConnection
 */
void MagabeServer::newConnection()
{

    QSslSocket *client = dynamic_cast<QSslSocket *>(sslServer_->nextPendingConnection());
    assert(client);

    //print("New connection from socketDescriptor == "+QString::number(client->socketDescriptor()));

    // QSslSocket emits the encrypted() signal after the encrypted connection is established
    connect(client, SIGNAL(encrypted()), this, SLOT(handshakeComplete()));

    // Report any SSL errors that occur
    connect(client, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionFailure()));

    client->setPrivateKey(*key_);
    client->setLocalCertificate(*certificate_);

    client->setPeerVerifyMode(QSslSocket::VerifyNone);
    client->startServerEncryption();

}


/**
 * @brief ChuraServer::handshakeComplete
 */
void MagabeServer::handshakeComplete()
{
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);

    connect(client, SIGNAL(disconnected()), this,SLOT(disconnected()));
    //connect(client, SIGNAL(readyRead()), this,SLOT(readyRead()));
    incomingDataManager_->connectSignalToClient(client);


    print(QString("[%1] Accepted connection from %2:%3")
          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
          .arg(client->peerAddress().toString())
          .arg(client->peerPort()));


    //add user and authenticate
    ConnectedUserList::instance()->addClient(client);
}

void MagabeServer::onStartAuthenticaion(QString mgbSessionId)
{
    print("Start Authentication ...");
    MagabeData data;
    data.setController("AuthenticationController")
            .setAction("ActionAuthenticationRequired")
            .addExtras("note","you must autheticate first");
    DWritter::instance()->sendData(data,mgbSessionId);
}

void MagabeServer::onCloseClientSocket(QString mgbSessionId)
{
    QSslSocket *client = const_cast<QSslSocket *>(ConnectedUserList::instance()->getSocket(mgbSessionId));
    if(client)
        client->close();
}

/**
 * @brief ChuraServer::sslErrors
 * @param errors
 */
void MagabeServer::sslErrors(const QList<QSslError> &errors)
{

    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);

    QString errorStrings;
    foreach (QSslError error, errors)
    {
        errorStrings += error.errorString();
        if (error != errors.last())
        {
            errorStrings += ';';
        }
    }

    print(QString("[%1] %2:%3 reported the following SSL errors: %4")
          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
          .arg(client->peerAddress().toString())
          .arg(client->peerPort())
          .arg(errorStrings));
}


/**
 * @brief ChuraServer::connectionFailure
 */
void MagabeServer::connectionFailure()
{

    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);

    //    print(QString("[%1] Connection from %2:%3 failed: %4")
    //          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
    //          .arg(client->peerAddress().toString())
    //          .arg(client->peerPort())
    //          .arg(client->errorString()));

    client->close();

}


/**
 * @brief MagabeServer::stopMagabeServer
 */
void MagabeServer::stopServer()
{
    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    //close server
    if(sslServer_){
        if(sslServer_->isListening()){
            sslServer_->close();
        }
    }

    emit serverStoped();
}

void MagabeServer::onAppAboutToQuit()//do not call stopServer becoz it emit a signal
{
    qDebug() << "MagabeServer::onAppAboutToQuit";
    if(dataProcessor_)dataProcessor_->stop();

    if(incomingDataManager_ && incomingDataManager_->isRunning()) incomingDataManager_->stop();

    if(sslServer_){
        if(sslServer_->isListening()){
            sslServer_->close();
        }
    }
}

/**
 * @brief ChuraServer::print
 * @param text
 */
void MagabeServer::print(QString text){
    qDebug() << text;
}


/**
 * @brief ChuraServer::disconnected
 */
void MagabeServer::disconnected()
{
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);


    //    print(QString("[%1] Connection from %2:%3 closed: %4")
    //          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
    //          .arg(client->peerAddress().toString())
    //          .arg(client->peerPort())
    //          .arg(client->errorString()));

    client->close();
}


/**
 * @brief ChuraServer::aboutToClose
 */
void MagabeServer::aboutToClose()
{

    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);

}


/**
 * @brief ChuraServer::bytesWritten
 * @param bytes
 */
void MagabeServer::bytesWritten(qint64 bytes)
{
    std::cout << bytes  << " bytes zimeandikwa";
}


/**
 * @brief ChuraServer::readChannelFinished
 */
void MagabeServer::readChannelFinished()
{
    QSslSocket *client = dynamic_cast<QSslSocket *>(sender());
    assert(client);
}


/**-------------------DataProcessor---------------------
 *
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
                                  ,Q_ARG(QByteArray,*d.cleanData)
                                  ,Q_ARG(QString,d.mgbSessionId));
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

void DataProcessor::startDataProcessing(const QByteArray &data, const QString mgbSessionId){
    if(!running_){
        start();
    }
    dataFilter_->appendToBuffer(data,mgbSessionId);
}


/** ---------------------IncomingDataManager-------------------------
 *
 *
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

void IncomingDataManager::connectSignalToClient(QSslSocket *client){
    if(client)
        connect(client, SIGNAL(readyRead()), this,SLOT(onMagabeServerReadyRead()));

}

void IncomingDataManager::stop(){
    dataReceiced_.wakeAll();
    running_ = false;
}
void IncomingDataManager::run(){
    while (isRunning()) {

        mutex_.lock();

        while(receivedData_.isEmpty() && (running_))
            dataReceiced_.wait(&mutex_);

        if(!running_){
            mutex_.unlock();
            break;
        }

        ReceivedData data = receivedData_.dequeue();
        if(!data.data_.isEmpty()){
            QMetaObject::invokeMethod(dataProcessor_
                                      ,"startDataProcessing"
                                      ,Q_ARG(QByteArray,data.data_)
                                      ,Q_ARG(QString,data.mgbSessionId_));
        }
        mutex_.unlock();

    }
}

/**
 * @brief IncomingDataManager::onMagabeServerReadyRead
 */
void IncomingDataManager::onMagabeServerReadyRead()
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
        QString mgbSessionId = ConnectedUserList::instance()->getMgbSessionId(client);
        ReceivedData data = {client->readAll(),mgbSessionId};
        receivedData_.enqueue(data);
        dataReceiced_.wakeOne();
    }
}

