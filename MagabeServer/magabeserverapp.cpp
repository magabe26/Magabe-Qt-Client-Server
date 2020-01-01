/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeserverapp.h"
#include <QDebug>
#include "Helpers/connectionpipe.h"
#include "debug.h"
#include "Controllers/controller.h"
#include "ControllerIdentifier/controlleridentifier.h"
#include "registeredobjects.h"


MagabeServerApp::MagabeServerApp(QObject *parent) : QObject(parent)
  ,appUsers_(new AppUsers())
  ,serverManager_(new MagabeServerManager(this))
{

    //connect signals and slots to the  lest of the app
    ConnectionPipe::registerObject(this);

    //connect signal to   MagabeServerManager, all signal should be connected this way
    connect(serverManager_,&MagabeServerManager::authenticedClientsCountChanged,this,&MagabeServerApp::authenticedClientsCountChanged);
    connect(serverManager_,&MagabeServerManager::unAuthenticatedClientsCountChanged,this,&MagabeServerApp::unAuthenticatedClientsCountChanged);
    connect(serverManager_,&MagabeServerManager::newAuthentication,this,&MagabeServerApp::newAuthentication);
    connect(serverManager_,&MagabeServerManager::clientDisconnected,this,&MagabeServerApp::clientDisconnected);
    connect(serverManager_,&MagabeServerManager::serverStarted,this,&MagabeServerApp::serverStarted);
    connect(serverManager_,&MagabeServerManager::serverStoped,this,&MagabeServerApp::serverStoped);
    connect(serverManager_,&MagabeServerManager::newSignUp,this,&MagabeServerApp::newSignUp);
    connect(serverManager_,&MagabeServerManager::profilePictureUploadProgress,this,&MagabeServerApp::profilePictureUploadProgress);
    connect(serverManager_,&MagabeServerManager::profilePictureUploadError,this,&MagabeServerApp::profilePictureUploadError);
    connect(serverManager_,&MagabeServerManager::newFileUploaded,this,&MagabeServerApp::newFileUploaded);
    connect(serverManager_,&MagabeServerManager::userProfilePictureChanged,this,&MagabeServerApp::userProfilePictureChanged);
    connect(serverManager_,&MagabeServerManager::newConnection,this,&MagabeServerApp::newConnection);
    connect(serverManager_,&MagabeServerManager::clientLogedOut,this,&MagabeServerApp::clientLogedOut);


    ConnectedClientsModel *pConnectedClientsModel = &connectedClientsModel_;
    auto swapClient =  [pConnectedClientsModel](QString mgbSessionId,unsigned long userId = 0){
        // qDebug() << "swap in ConnectedClientsModel -> " << userId << mgbSessionId;
        return  pConnectedClientsModel->swap(mgbSessionId,userId);
    };

    auto removeClient =  [pConnectedClientsModel](QString mgbSessionId){
        //qDebug() << "remove in ConnectedClientsModel -> " << mgbSessionId;
        return  pConnectedClientsModel->remove(mgbSessionId);
    };

    QObject::connect(this,&MagabeServerApp::newConnection,[=](QString mgbSessionId){
        swapClient(mgbSessionId);
    });

    QObject::connect(this,&MagabeServerApp::newAuthentication,[=](unsigned long userId,QString mgbSessionId){
        swapClient(mgbSessionId,userId);
    });

    QObject::connect(this,&MagabeServerApp::clientDisconnected,[=](QString mgbSessionId){
        removeClient(mgbSessionId);
    });


#ifdef MAGABE_TESTS

    ConnectionPipe::connectSlot(this,"newAuthentication",this,"onNewAuthentication");
    ConnectionPipe::connectSlot(this,"clientDisconnected",this,"onClientDisconnected");
    ConnectionPipe::connectSlot(this,"newSignUp",this,"onNewSignUp");
    ConnectionPipe::connectSlot(this,"profilePictureUploadProgress",this,"onProfilePictureUploadProgress");
    ConnectionPipe::connectSlot(this,"profilePictureUploadError",this,"onProfilePictureUploadError");
    ConnectionPipe::connectSlot(this,"newFileUploaded",this,"onNewFileUploaded");
    ConnectionPipe::connectSlot(this,"userProfilePictureChanged",this,"onUserProfilePictureChanged");

#endif

}

MagabeServerApp::~MagabeServerApp()
{
    ConnectionPipe::unRegisterObject(this);

    if(appUsers_) {
        delete appUsers_;
        appUsers_ = nullptr;
    }
}

User MagabeServerApp::getUser(unsigned long userId)
{
    return appUsers_->getUser(userId);
}

User MagabeServerApp::getUser(const QString &username)
{
    return appUsers_->getUser(username);
}

bool MagabeServerApp::userRegistered(QString username)
{
    return appUsers_->userRegistered(username);
}

bool MagabeServerApp::userRegistered(unsigned long userId)
{
    return appUsers_->userRegistered(userId);
}

int MagabeServerApp::getRegisteredUsersCount()
{
    return appUsers_->getTotal();
}

bool MagabeServerApp::isClientAuthenticated(const QSslSocket *clientSocket)
{
    return appUsers_->isClientAuthenticated(clientSocket);
}

bool MagabeServerApp::isClientAuthenticated(const QString &mgbSessionId)
{
    return appUsers_->isClientAuthenticated(mgbSessionId);
}

bool MagabeServerApp::closeClientConnetion(const QString &mgbSessionId)
{
    return appUsers_->closeClientConnetion(mgbSessionId);
}

bool MagabeServerApp::closeClientConnetion(const QSslSocket *clientSocket)
{
    return appUsers_->closeClientConnetion(clientSocket);
}

const QString MagabeServerApp::getMgbSessionId(const QSslSocket *clientSocket)
{
    return appUsers_->getMgbSessionId(clientSocket);
}

bool MagabeServerApp::closeAllClientsConnetionsForUser(unsigned long userId)
{
    auto idList = getUser(userId).getAllClientsMgbSessionIdsForThisUser();
    if(idList.isEmpty()){
        return false;
    }else{
        bool closed = false;
        foreach (auto mgbSessionId, idList) {
            if(closeClientConnetion(mgbSessionId)){
                closed = true;
            }
        }
        return closed;
    }
}

bool MagabeServerApp::closeAllClientsConnetionsForUser(const QString &username)
{
    auto idList = getUser(username).getAllClientsMgbSessionIdsForThisUser();
    if(idList.isEmpty()){
        return false;
    }else{
        bool closed = false;
        foreach (auto mgbSessionId, idList) {
            if(closeClientConnetion(mgbSessionId)){
                closed = true;
            }
        }
        return closed;
    }
}

void MagabeServerApp::startServer(const QString &address, quint16 port)
{
    startServer(QHostAddress(address),port);
}

void MagabeServerApp::startServer(const QHostAddress& address, quint16 port)
{
    emit serverManager_->startServer(address,port);
}

void MagabeServerApp::stopServer()
{
    emit serverManager_->stopServer();
}

bool MagabeServerApp::isServerStarted()
{
    return  serverManager_->isServerStarted();
}

bool MagabeServerApp::triggerControllerAction(const QString &controller, const QString &action, Extras &extras)
{

    ControllerIdentifier *controllerIdentifier =  CONTROLLER_IDENTIFIER_REF;
    if(controllerIdentifier){
        if(controllerIdentifier->isActionRegistered(controller,action)){
            MagabeData data(extras.toByteArray());
            data.setController(controller).setAction(action);
            QString bytesArray  =  Security::instance()->encryptToString(data.toByteArray());//only encrypt to string, ecryption to bytearray cause los of data
            return QMetaObject::invokeMethod(controllerIdentifier
                                             ,"onDataReceivedFromDataProcessor"
                                             ,Q_ARG(QByteArray,bytesArray.toLatin1()));
        }else{
            return false;
        }
    }else{
        return false;
    }
}

int MagabeServerApp::unAuthenticatedClientsCount() const
{
    return serverManager_->unAuthenticatedClientsCount_;
}

int MagabeServerApp::authenticedClientsCount() const
{
    return serverManager_->authenticedClientsCount_;
}

ConnectedClientsModel &MagabeServerApp::getConnectedClientsModel()
{
    return connectedClientsModel_;
}





#ifdef MAGABE_TESTS

void MagabeServerApp::onNewAuthentication(unsigned long userId, QString mgbSessionId, QDateTime when)
{
    qDebug() << "MagabeServerApp::onNewAuthentication | userId == " << userId <<", mgbSessionId == " << mgbSessionId << ", When =="<< when.toString("hh:mm:ss.zzz ap");
    //    stopServer();

    //     startServer(QHostAddress::Any,12345);
}

void MagabeServerApp::onClientDisconnected(QString mgbSessionId,unsigned long userId, QDateTime when)
{
    qDebug() << "MagabeServerApp::onClientDisconnected | mgbSessionId == " << mgbSessionId <<",  userId == " << userId << ", When =="<< when.toString("hh:mm:ss.zzz ap");

}

void MagabeServerApp::onNewSignUp(unsigned long userId, QString mgbSessionId)
{
    qDebug() << "MagabeServerApp::onNewSignUp | userId == " << userId <<", mgbSessionId == " << mgbSessionId;

}

void MagabeServerApp::onProfilePictureUploadProgress(unsigned long userId, QString fileBaseName, QString fileExtension, QString fileLocation, float percentageCompletion, quint64 uploaded, quint64 fileSize)
{
    qDebug() << "MagabeServerApp::onProfilePictureUploadProgress | userId == " << userId <<", percentageCompletion == " << percentageCompletion <<"%";

}

void MagabeServerApp::onProfilePictureUploadError(int errorNo, QString errorString)
{
    qDebug() << " onProfilePictureUploadError | errorNo == " << errorNo << ", errorString ==" << errorString;
}

void MagabeServerApp::onNewFileUploaded(unsigned long userId, QString filePath)
{
    qDebug() << "MagabeServerApp::onNewFileUploaded | userId == " << userId << ", filePath ==" << filePath;

}

void MagabeServerApp::onUserProfilePictureChanged(unsigned long userId, QString filePath)
{
    qDebug() << "MagabeServerApp::onUserProfilePictureChanged | userId == " << userId << ", filePath ==" << filePath;

}


#endif


