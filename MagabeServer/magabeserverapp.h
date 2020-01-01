/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABE_SERVER_APP_H
#define MAGABE_SERVER_APP_H

#include <QObject>
#include <QDateTime>
#include <Helpers/user.h>
#include "magabeservermanager.h"
#include <QHostAddress>
#include "Helpers/extras.h"
#include "Helpers/connectedclientsmodel.h"

class AppUsers;
/**  -------------ALL MAGABECLIENT-SERVER BASED APP SHOULD IMPLEMENT THIS------------------------
 * @brief The MagabeServerApp class
 */
class MagabeServerApp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int authenticedClientsCount READ authenticedClientsCount  NOTIFY authenticedClientsCountChanged)
    Q_PROPERTY(int unAuthenticatedClientsCount READ unAuthenticatedClientsCount NOTIFY unAuthenticatedClientsCountChanged)

public:
    explicit MagabeServerApp(QObject *parent = nullptr);
    ~MagabeServerApp();

    Q_INVOKABLE User getUser(unsigned long userId);
    Q_INVOKABLE User getUser(const QString &username);
    Q_INVOKABLE bool userRegistered(QString username);
    Q_INVOKABLE bool userRegistered(unsigned long userId);
    Q_INVOKABLE int  getRegisteredUsersCount();
    Q_INVOKABLE bool isClientAuthenticated(const QSslSocket* clientSocket);
    Q_INVOKABLE bool isClientAuthenticated(const QString &mgbSessionId);
    Q_INVOKABLE bool closeClientConnetion(const QString &mgbSessionId);
    Q_INVOKABLE bool closeClientConnetion(const QSslSocket* clientSocket);
    Q_INVOKABLE const QString getMgbSessionId(const QSslSocket *clientSocket);
    Q_INVOKABLE bool closeAllClientsConnetionsForUser(unsigned long userId);
    Q_INVOKABLE bool closeAllClientsConnetionsForUser(const QString &username);
    Q_INVOKABLE void startServer(const QString& address, quint16 port);
    void startServer(const QHostAddress& address, quint16 port);
    Q_INVOKABLE void stopServer();
    Q_INVOKABLE bool isServerStarted() ;
    bool triggerControllerAction(const QString &controller, const QString &action, Extras& extras);

    int authenticedClientsCount() const;
    int unAuthenticatedClientsCount() const;

    Q_INVOKABLE ConnectedClientsModel& getConnectedClientsModel();

signals:
    void authenticedClientsCountChanged(int count);
    void unAuthenticatedClientsCountChanged(int count);
    void newAuthentication(unsigned long userId,QString mgbSessionId,QDateTime when);
    void clientDisconnected(QString mgbSessionId, unsigned long userId, QDateTime when);
    void serverStarted(QHostAddress address, quint16 port);
    void serverStoped();
    void newSignUp(unsigned long userId,QString mgbSessionId);
    void profilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                      ,QString fileExtension,QString fileLocation
                                      ,float percentageCompletion,quint64  uploaded
                                      ,quint64 fileSize);
    void profilePictureUploadError(int errorNo,QString errorString);
    void newFileUploaded(unsigned long userId,QString filePath);
    void userProfilePictureChanged(unsigned long userId,QString filePath);
    void newConnection(QString mgbSessionId);
    void clientLogedOut(unsigned long userId,QString mgbSessionId);

public slots:

#ifdef MAGABE_TESTS

    void onNewAuthentication(unsigned long userId,QString mgbSessionId,QDateTime when);
    void onClientDisconnected(QString mgbSessionId, unsigned long userId, QDateTime when);
    void onNewSignUp(unsigned long userId,QString mgbSessionId);
    void onProfilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                        ,QString fileExtension,QString fileLocation
                                        ,float percentageCompletion,quint64  uploaded
                                        ,quint64 fileSize);
    void onProfilePictureUploadError(int errorNo,QString errorString);
    void onNewFileUploaded(unsigned long userId,QString filePath);
    void onUserProfilePictureChanged(unsigned long userId,QString filePath);

#endif

private slots:

private:
    AppUsers *appUsers_;
    MagabeServerManager *serverManager_;
    ConnectedClientsModel connectedClientsModel_;

};

#endif // MAGABE_SERVER_APP_H
