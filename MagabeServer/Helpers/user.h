/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef USER_H
#define USER_H
#include <QObject>
#include <QString>
#include <QChar>



class UserModel;
class UserObjectEditor;
class Controller;
class QSslSocket;
/** Must not contain UserModel object, it will cause recursio during
object instatiation*/

class User: public QObject
{
    Q_OBJECT


    Q_PROPERTY(unsigned long id READ id)
    Q_PROPERTY(QString firstName READ firstName)
    Q_PROPERTY(QString lastName READ lastName)
    Q_PROPERTY(QString dateOfBirth READ dateOfBirth)
    Q_PROPERTY(Gender gender READ gender)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString authKey READ authKey)
    Q_PROPERTY(QString password READ password)
    Q_PROPERTY(QString email READ email)
    Q_PROPERTY(QString phoneNumber READ phoneNumber)
    Q_PROPERTY(qint64 createdAt READ createdAt)
    Q_PROPERTY(qint64 updatedAt READ updatedAt)
    Q_PROPERTY(QString profilePicturePath READ profilePicturePath)
    Q_PROPERTY(QString position READ position)
    Q_PROPERTY(QString education READ education)
    Q_PROPERTY(QString location READ location)

public:

    enum  Gender { MALE = 0x7c5,FEMALE };
    enum  ConnectionState{
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ListeningState,
        ClosingState
    };

    Q_ENUM(Gender)
    Q_ENUM(ConnectionState)


    User();
    ~User();
    User(User &&other);
    User(const User &other);
    User &operator = (User &&other);
    User &operator = (const User &other);
    bool operator == (const  User &user);

    unsigned long id() const;
    QString firstName() const;
    QString lastName() const;
    QString dateOfBirth() const;
    User::Gender gender() const;
    QString username() const;
    QString authKey() const;
    QString password() const;
    QString email() const;
    qint64 createdAt() const;
    qint64 updatedAt() const;
    QString profilePicturePath() const;
    QString education() const;
    QString position() const;
    QString location() const;
    QString phoneNumber() const;

    Q_INVOKABLE bool closethisClientConnection();
    Q_INVOKABLE bool isActive() const;//same as isConnected
    Q_INVOKABLE bool isBlackListed() const;
    /*return a connection id of one device that is managed this user instance*/
    Q_INVOKABLE QString getThisClientMgbSessionId() const;
    Q_INVOKABLE bool thisClientConnectionIsActive();
    Q_INVOKABLE ConnectionState getThisClientConnectionState();
    Q_INVOKABLE QString getConnectionStateHuman();
    /*returns all connection ids of all connected devices that belong to this user*/
    Q_INVOKABLE QList<QString> getAllClientsMgbSessionIdsForThisUser() const;
    /*returns socket list that represent all connected devices that belong to this user*/
    Q_INVOKABLE bool hasAtLeastOneConnectedClient();
    Q_INVOKABLE bool hasMoreThanOneConnectedClients();
    QList<const QSslSocket *> getAllConnectedClientsSocketsForThisUser() const;
    Q_INVOKABLE QString getJsonUserInfo();

private slots:
    void onDisconnected();

private:
    //Note:do not add other friends,expecially Controller
    friend class UserModel;
    friend class UserObjectEditor;
    QSslSocket *thisClientSocket_;//represent user's one connected device
    Controller *thisClientController_;
    unsigned long id_;
    QString firstName_;
    QString lastName_;
    QString dateOfBirth_;
    Gender gender_;
    QString username_;
    QString authKey_;
    QString password_;
    QString email_;
    bool isActive_; //is currently using the app
    bool isBlackListed_;
    qint64 createdAt_;
    qint64 updatedAt_;
    QString profilePicturePath_;
    QString education_;
    QString position_;
    QString location_;
    QString phoneNumber_;
    QString thisClientMgbSessionId_;

    void setThisClientSocket(const QSslSocket *socket);
    void print(QString text);
    void exitMESAEExecution();

};

Q_DECLARE_METATYPE(User::Gender)

#endif // USER_H
