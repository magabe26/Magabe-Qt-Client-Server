/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeservermanager.h"
#include "registeredobjects.h"
#include "debug.h"

MagabeServerManager::MagabeServerManager(QObject *parent) : QObject(parent)
  ,isServerStarted_(false)
  ,authenticedClientsCount_(0)
  ,unAuthenticatedClientsCount_(0)
  ,magabeServerManagerSignalReceiver_(new MagabeServerManagerSignalReceiver(this))
{
    ConnectionPipe::registerObject(this);

    //when new connection
    ConnectionPipe::connectSlot(RegisteredObjects::Mgb_ConnectedUsers::className,RegisteredObjects::Mgb_ConnectedUsers::Signals::newConnection,this,RegisteredObjects::MagabeServerManager::Slots::onNewConnection);

    /*when the server is started*/
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServer::className,RegisteredObjects::MagabeServer::Signals::serverStarted,this,RegisteredObjects::MagabeServerManager::Slots::onServerStarted);
    /*when the server is stoped*/
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeServer::className,RegisteredObjects::MagabeServer::Signals::serverStoped,this,RegisteredObjects::MagabeServerManager::Slots::onServerStoped);

    /*------when authenticedClientsCountChanged--*/
    ConnectionPipe::connectSlot(RegisteredObjects::Mgb_ConnectedUsers::className,RegisteredObjects::Mgb_ConnectedUsers::Signals::authenticedClientsCountChanged,this,RegisteredObjects::MagabeServerManager::Slots::onAuthenticedClientsCountChanged);
    /*------when unAuthenticatedClientsCountChanged---*/
    ConnectionPipe::connectSlot(RegisteredObjects::Mgb_ConnectedUsers::className,RegisteredObjects::Mgb_ConnectedUsers::Signals::unAuthenticatedClientsCountChanged,this,RegisteredObjects::MagabeServerManager::Slots::onUnAuthenticatedClientsCountChanged);

    //when client is authenticated
    ConnectionPipe::connectSlot(RegisteredObjects::Mgb_ConnectedUsers::className,RegisteredObjects::Mgb_ConnectedUsers::Signals::newAuthentication,this,RegisteredObjects::MagabeServerManager::Slots::onNewAuthentication);

    /* when client clientDisconnected*/
    ConnectionPipe::connectSlot(RegisteredObjects::Mgb_ConnectedUsers::className,RegisteredObjects::Mgb_ConnectedUsers::Signals::clientDisconnected,this,RegisteredObjects::MagabeServerManager::Slots::onClientDisconnected);

    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::newSignUp,this,&MagabeServerManager::onNewSignUp);
    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::profilePictureUploadProgress,this,&MagabeServerManager::onProfilePictureUploadProgress);
    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::profilePictureUploadError,this,&MagabeServerManager::onProfilePictureUploadError);
    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::userProfilePictureChanged,this,&MagabeServerManager::onUserProfilePictureChanged);
    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::newFileUploaded,this,&MagabeServerManager::onNewFileUploaded);
    connect(magabeServerManagerSignalReceiver_,&MagabeServerManagerSignalReceiver::clientLogedOut,this,&MagabeServerManager::onClientLogedOut);


}

MagabeServerManager::~MagabeServerManager()
{
    ConnectionPipe::unRegisterObject(this);
}

void MagabeServerManager::onServerStarted(QHostAddress address, quint16 port)
{
    Q_UNUSED(address)
    Q_UNUSED(port)

    isServerStarted_ = true;

#ifdef MAGABE_TESTS
    qDebug() << "MagabeServerManager::::onServerStarted"  << port;
#endif
    emit authenticedClientsCountChanged(authenticedClientsCount_);
    emit unAuthenticatedClientsCountChanged(unAuthenticatedClientsCount_);

    emit serverStarted(address,port);
}

void MagabeServerManager::onServerStoped()
{
    isServerStarted_ = false;
    authenticedClientsCount_ = 0;
    unAuthenticatedClientsCount_ = 0;

#ifdef MAGABE_TESTS
    qDebug() << "MagabeServerManager::::onServerStoped" ;
#endif
    emit authenticedClientsCountChanged(authenticedClientsCount_);
    emit unAuthenticatedClientsCountChanged(unAuthenticatedClientsCount_);
    //then
    emit serverStoped();
}

void MagabeServerManager::onAuthenticedClientsCountChanged(int count){
    if(authenticedClientsCount_ != count){

#ifdef MAGABE_TESTS
        qDebug() << "MagabeServerManager::onAuthenticedClientsCountChanged | count == " << count;
#endif

        authenticedClientsCount_ = count;
        emit authenticedClientsCountChanged(authenticedClientsCount_);
    }
}

void MagabeServerManager::onUnAuthenticatedClientsCountChanged(int count){
    if(unAuthenticatedClientsCount_ != count){

#ifdef MAGABE_TESTS
        qDebug() << "MagabeServerManager::onAuthenticedClientsCountChanged | count == " << count;
#endif

        unAuthenticatedClientsCount_ = count;
        emit unAuthenticatedClientsCountChanged(unAuthenticatedClientsCount_);
    }
}

void MagabeServerManager::onNewAuthentication(unsigned long userId, QString mgbSessionId, QDateTime when)
{

    emit newAuthentication(userId,mgbSessionId,when);
}

void MagabeServerManager::onClientDisconnected(QString mgbSessionId, unsigned long userId, QDateTime when)
{

    emit clientDisconnected(mgbSessionId,userId,when);
}

void MagabeServerManager::onNewSignUp(unsigned long userId, QString mgbSessionId)
{
    emit newSignUp(userId,mgbSessionId);
}

void MagabeServerManager::onProfilePictureUploadProgress(unsigned long userId, QString fileBaseName
                                                         , QString fileExtension, QString fileLocation
                                                         , float percentageCompletion, quint64 uploaded, quint64 fileSize)
{

    emit profilePictureUploadProgress(userId,fileBaseName,fileExtension,fileLocation,percentageCompletion,uploaded,fileSize);
}

void MagabeServerManager::onProfilePictureUploadError(int errorNo, QString errorString)
{

    emit profilePictureUploadError(errorNo,errorString);
}

void MagabeServerManager::onUserProfilePictureChanged(unsigned long userId, QString filePath)
{

    emit userProfilePictureChanged(userId,filePath);
}

void MagabeServerManager::onNewFileUploaded(unsigned long userId, QString filePath)
{
    emit newFileUploaded(userId,filePath);
}

void MagabeServerManager::onClientLogedOut(unsigned long userId, QString mgbSessionId)
{
#ifdef MAGABE_TESTS
    qDebug() << "MagabeServerManager::onClientLogedOut" << userId << mgbSessionId;
#endif

    emit clientLogedOut(userId,mgbSessionId);
}

void MagabeServerManager::onNewConnection(QString mgbSessionId)
{
#ifdef MAGABE_TESTS
    qDebug() << "MagabeServerManager::onNewConnection" << mgbSessionId;
#endif
    emit newConnection(mgbSessionId);
}

bool MagabeServerManager::isServerStarted() const
{
    return isServerStarted_;
}
