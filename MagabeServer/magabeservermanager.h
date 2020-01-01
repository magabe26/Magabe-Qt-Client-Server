/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABESERVERMANAGER_H
#define MAGABESERVERMANAGER_H

#include <QObject>
#include <QHostAddress>
#include "Helpers/connectionpipe.h"
#include <QDateTime>

class MagabeServerApp;

/** ---------------------MagabeServerManagerSignalReceiver-----------------------------
 * mostly receive signals from controllers( becoz have short and upredictable life span )
 * @brief The MagabeServerManagerSignalReceiver class
 */
class MagabeServerManagerSignalReceiver : public QObject
{
    Q_OBJECT
public:
    //note do not send constructor And destructor method to .cpp files, cause moc problems
    explicit MagabeServerManagerSignalReceiver(QObject *parent = nullptr):QObject(parent)
    {
        ConnectionPipe::registerObject(this);

    }
    ~MagabeServerManagerSignalReceiver(){
        ConnectionPipe::unRegisterObject(this);
    }
signals:
    void newSignUp(unsigned long userId,QString mgbSessionId);
    void profilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                      ,QString fileExtension,QString fileLocation
                                      ,float percentageCompletion,quint64  uploaded
                                      ,quint64 fileSize);
    void profilePictureUploadError(int errorNo,QString errorString);
    void userProfilePictureChanged(unsigned long userId,QString filePath);
    void newFileUploaded(unsigned long userId,QString filePath);
    void clientLogedOut(unsigned long userId,QString mgbSessionId);

};

/** --------------MagabeServerManager----------------------
 *  Intercept signals and some necessary staff

 * @brief The MagabeServerManager class
 */
class MagabeServerManager : public QObject
{
    Q_OBJECT
public:
    explicit MagabeServerManager(QObject *parent = nullptr);
    ~MagabeServerManager();
    bool isServerStarted() const;

signals:
    //signals to start and stop server
    void startServer(QHostAddress address, quint16 port);
    void stopServer();
    /*---------------------*/

    void serverStarted(QHostAddress address, quint16 port);
    void serverStoped();
    void newConnection(QString mgbSessionId);
    void authenticedClientsCountChanged(int count);
    void unAuthenticatedClientsCountChanged(int count);
    void newAuthentication(unsigned long userId,QString mgbSessionId,QDateTime when);
    void clientDisconnected(QString mgbSessionId, unsigned long userId, QDateTime when);
    void newSignUp(unsigned long userId,QString mgbSessionId);
    void profilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                      ,QString fileExtension,QString fileLocation
                                      ,float percentageCompletion,quint64  uploaded
                                      ,quint64 fileSize);
    void profilePictureUploadError(int errorNo,QString errorString);
    void userProfilePictureChanged(unsigned long userId,QString filePath);
    void newFileUploaded(unsigned long userId,QString filePath);
    void clientLogedOut(unsigned long userId,QString mgbSessionId);

private slots:
    void onServerStarted(QHostAddress address, quint16 port);
    void onServerStoped();

    void onNewConnection(QString mgbSessionId);

    void onAuthenticedClientsCountChanged(int count);
    void onUnAuthenticatedClientsCountChanged(int count);

    void onNewAuthentication(unsigned long userId,QString mgbSessionId,QDateTime when);
    void onClientDisconnected(QString mgbSessionId, unsigned long userId, QDateTime when);

    void onNewSignUp(unsigned long userId,QString mgbSessionId);
    void onProfilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                        ,QString fileExtension,QString fileLocation
                                        ,float percentageCompletion,quint64  uploaded
                                        ,quint64 fileSize);
    void onProfilePictureUploadError(int errorNo,QString errorString);
    void onUserProfilePictureChanged(unsigned long userId,QString filePath);
    void onNewFileUploaded(unsigned long userId,QString filePath);
    void onClientLogedOut(unsigned long userId,QString mgbSessionId);

private:
    friend class MagabeServerApp;
    bool isServerStarted_;
    int authenticedClientsCount_;
    int unAuthenticatedClientsCount_;
    MagabeServerManagerSignalReceiver *magabeServerManagerSignalReceiver_;
};

#endif // MAGABESERVERMANAGER_H
