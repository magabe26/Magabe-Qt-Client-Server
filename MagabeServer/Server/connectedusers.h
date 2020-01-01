/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONNECTEDUSERS_H
#define CONNECTEDUSERS_H

#include <QObject>
#include <QList>
#include <QSslSocket>
#include "Helpers/magabedata.h"
#include "Helpers/magabesingleton.h"
#include "Models/usermodel.h"
#include <QHostAddress>

class DataWritter;
class SimpleCrypt;
class MSessions;


namespace Mgb {
class ConnectedUsers : public QObject
{
    Q_OBJECT
public:
    explicit ConnectedUsers(QObject *parent = nullptr);
    ~ ConnectedUsers();

    void addClient(QSslSocket *client);
    unsigned long  getUserId(const QString& mgbSessionId);
    const QSslSocket* getSocket(const QString& mgbSessionId);
    void sendToAllClient(const QString& message);
    bool userExists(const QString& mgbSessionId);
    bool userExists(const QSslSocket *client);
    QString getMgbSessionId(const QSslSocket *client);
    QList<QString> getMgbSessionIds(unsigned long  userId);
    QList<const QSslSocket *> getSockets(unsigned long  userId);
    bool hasConnectedClients(unsigned long userId);
signals:
    void authenticedClientsCountChanged(int count);
    void unAuthenticatedClientsCountChanged(int count);
    void startAuthenticaion(QString mgbSessionId);
    void clientDisconnected(QString mgbSessionId,unsigned long userId,QDateTime when);
    void newAuthentication(unsigned long userId,QString mgbSessionId,QDateTime when);
    void newConnection(QString mgbSessionId);

public slots:
    bool isAuthenticated(QString mgbSessionId);
    void onDisconnected();
    void onServerStarted(QHostAddress address, quint16 port);
    void onServerStoped();
    bool onAuthenticationProcessSucceded(QString mgbSessionId, unsigned long  userId);
    bool onLogOut(QString mgbSessionId);

private:
    friend class DataWritter;
    //only data writter and magabe server can acsess send functions
    void send(QString data,unsigned long  userId);
    void send(MagabeData data,const QString& mgbSessionId);
    bool removeUser(const QString& mgbSessionId);
    bool removeUser(QSslSocket *client);
    bool addAuthenticatedUser(unsigned long  userId,const QString& mgbSessionId);
    bool addAuthenticatedUser(unsigned long  userId,QSslSocket *client);
    bool removeAuthenticatedUser(QSslSocket* client);
    bool removeAuthenticatedUser(const QString& mgbSessionId);
    void print(const QString& text);
    int authenticedClientsCount();
    int unAuthenticatedClient();

protected:
    UserModel usermodel_;
    SimpleCrypt * crypto_;
    MSessions * sessions_;
};

}
typedef MagabeSingleton<Mgb::ConnectedUsers> ConnectedUserList;

#endif // CONNECTEDUSERS_H
