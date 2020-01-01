/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABE_CLIENT_APP_H
#define MAGABE_CLIENT_APP_H

#include <QObject>
#include "Helpers/magabedata.h"
#include <functional>
#include "magabeclientmanager.h"
#include "Helpers/mopaque.h"
#include "Helpers/mqmlcallback.h"
#include "Helpers/errors.h"
#include <QQuickItem>

/**  -------------ALL MAGABECLIENT-SERVER BASED APP SHOULD IMPLEMENT THIS------------------------
 * @brief The MagabeClientApp class
 */
class MagabeClientApp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool authenticated READ isAuthenticated NOTIFY authenticatedChanged)
    Q_PROPERTY(bool connected READ isConnected NOTIFY connectedChanged)

public:

    explicit MagabeClientApp(QObject *parent = nullptr);
    ~MagabeClientApp();

    Q_INVOKABLE static QString authenticationErrorToString(Errors::AuthenticationError authenticationError);
    Q_INVOKABLE void signUp(const QString& username,const QString& firstName
                            ,const QString& lastName,const QString& dateOfBirth,QChar gender
                            ,const QString& password,const QString& email);
    Q_INVOKABLE void login(const QString& username,const QString& password);
    Q_INVOKABLE void logOut();

    void uploadProfilePicture(const QString &imagePath
                              ,ProfilePictureUploadedCallback profilePictureUploadedCallback
                              ,ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback
                              ,ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback
                              ,void *opaque = (void*)0);

    Q_INVOKABLE void uploadProfilePicture(const QString &imagePath
                                          ,MQmlCallback* profilePictureUploadedCallback
                                          ,MQmlCallback* profilePictureUploadingFailedCallback
                                          ,MQmlCallback* profilePictureUploadProgressCallback);

    void uploadScaledProfilePicture(QString imagePath, int quality, int width, int height
                                    , Qt::AspectRatioMode aspectMode
                                    , ProfilePictureUploadedCallback profilePictureUploadedCallback
                                    , ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback
                                    , ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback
                                    , void *opaque = (void*)0);

    Q_INVOKABLE void uploadScaledProfilePicture(QString imagePath, int quality, int width, int height
                                                , Qt::AspectRatioMode aspectMode
                                                , MQmlCallback* profilePictureUploadedCallback
                                                , MQmlCallback* profilePictureUploadingFailedCallback
                                                , MQmlCallback* profilePictureUploadProgressCallback);

    Q_INVOKABLE void connectToServer(const QString& ipAddress,int port,int msecsTimeout);
    Q_INVOKABLE void disconnectFromServer();


    void getUserInfo(const QString& username,GetUserInfoCallback cb,void *opaque = (void*)0);
    Q_INVOKABLE void getUserInfo(const QString& username,MQmlCallback* mqmlcb);

    void getUserInfo(unsigned long userId,GetUserInfoCallback cb,void *opaque = (void*)0);
    Q_INVOKABLE void getUserInfo(unsigned long userId,MQmlCallback* mqmlcb);

    void downloadProfilePicture(const QString &username, DownloadProfilePictureCallback downloadProfilePictureCallback, ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque = (void*)0);
    Q_INVOKABLE void downloadProfilePicture(const QString &username, MQmlCallback* downloadProfilePictureCallback, MQmlCallback* profilePictureDownloadProgressCallback);

    void downloadProfilePicture(unsigned long userId, DownloadProfilePictureCallback downloadProfilePictureCallback, ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque = (void*)0);
    Q_INVOKABLE void downloadProfilePicture(unsigned long userId,MQmlCallback* downloadProfilePictureCallback, MQmlCallback* profilePictureDownloadProgressCallback);

    bool triggerControllerAction(const QString& controller, const QString& action ,Extras &extras);
    bool isConnected();
    bool isAuthenticated() const;

    Q_INVOKABLE bool isValidConversationIdentifier(const QString &conversationIdentifier);

    Q_INVOKABLE bool isValidMessageIdentifier(const QString& messageIdentifier);


    void getSentMessages(const Message &start, GetSentMessagesCallback cb
                         , int maxNoOfMessages = 5, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getSentMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);


    void getSentMessages(const Conversation& conversation, GetSentMessagesCallback cb
                         , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getSentMessages(const QVariantMap& conversation,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);

    void getReceivedMessages(const Message &start, GetReceivedMessagesCallback cb
                             , int maxNoOfMessages = 5, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getReceivedMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages
                                         ,int sortOrder,MQmlCallback* mqmlcb);

    void getReceivedMessages(const Conversation& conversation, GetReceivedMessagesCallback cb
                             , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getReceivedMessages(const QVariantMap& conversation,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);


    void getMessages(const Message &start,GetMessagesCallback cb
                     ,int maxNoOfMessages = 5 ,bool includeStartMessage = false,const Sort::Order sortOrder = Sort::Order::ASC,void *opaque = (void*)0);

    Q_INVOKABLE void getMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages
                                 ,int sortOrder,MQmlCallback* mqmlcb);

    void getMessages(const Conversation& conversation, GetMessagesCallback cb
                     , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getMessages(const QVariantMap& conversation,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);


    void getReadMessages(const Message &start ,GetReadMessagesCallback cb
                         ,int maxNoOfMessages = 5, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC,void *opaque = (void*)0);

    Q_INVOKABLE void getReadMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages
                                     ,int sortOrder,MQmlCallback* mqmlcb);


    void getReadMessages(const Conversation& conversation, GetReadMessagesCallback cb
                         , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC, void *opaque = (void*)0);

    Q_INVOKABLE void getReadMessages(const QVariantMap& conversation,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);

    void getUnReadMessages(const Message &start,GetUnReadMessagesCallback cb
                           ,int maxNoOfMessages = 5, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC,void *opaque = (void*)0);

    Q_INVOKABLE void getUnReadMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages
                                       ,int sortOrder,MQmlCallback* mqmlcb);

    void getUnReadMessages(const Conversation& conversation,GetUnReadMessagesCallback cb
                           ,int maxNoOfMessages = 5,const Sort::Order sortOrder = Sort::Order::ASC,void *opaque = (void*)0);

    Q_INVOKABLE void getUnReadMessages(const QVariantMap& conversation,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb);

    void sendMessage(const QString& receiverUserName,const QString& messageText,SendMessageCallback cb,void *opaque = (void*)0);
    Q_INVOKABLE void sendMessage(const QString& receiverUserName,const QString& messageText,MQmlCallback* mqmlcb);

    void sendMessage(unsigned long receiverUserId,const QString& messageText,SendMessageCallback cb,void *opaque = (void*)0);
    Q_INVOKABLE void sendMessage(unsigned long receiverUserId,const QString& messageText,MQmlCallback* mqmlcb);

    void fetchUserNames(const QList<unsigned long> &userIds, FetchUserNamesCallback cb,void *opaque = (void*)0);
    Q_INVOKABLE void fetchUserNames(const QVariantList &userIds, MQmlCallback* mqmlcb);

    Q_INVOKABLE void getUserName(unsigned long userId, MQmlCallback* mqmlcb);
    Q_INVOKABLE QString decodeMessage(const QString& encodedMessage);
    Q_INVOKABLE QString generateConversationIdentifier(const QString& messageIdentifier);
    Q_INVOKABLE unsigned long userId();
    Q_INVOKABLE QString userName();
    Q_INVOKABLE QString authKey();
    Q_INVOKABLE QString password();
    void getMessageCount(const Conversation& conversation, GetMessageCountCallback cb, MessageType type, void *opaque = (void*)0);
    Q_INVOKABLE void getMessageCount(const QVariantMap& conversation, MQmlCallback* mqmlcb);
    Q_INVOKABLE void getSentMessageCount(const QVariantMap& conversation, MQmlCallback* mqmlcb);
    Q_INVOKABLE void getReceivedMessageCount(const QVariantMap& conversation, MQmlCallback* mqmlcb);
    Q_INVOKABLE void getReadMessageCount(const QVariantMap& conversation, MQmlCallback* mqmlcb);
    Q_INVOKABLE void getUnReadMessageCount(const QVariantMap& conversation, MQmlCallback* mqmlcb);
    void getLastMessage(const Conversation &conversation, GetLastMessageCallback cb, MessageType type, void *opaque = (void*)0);
    Q_INVOKABLE void getLastMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb);
    Q_INVOKABLE void getLastSentMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb);
    Q_INVOKABLE void getLastReceivedMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb);
    Q_INVOKABLE void getLastReadMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb);
    Q_INVOKABLE void getLastUnReadMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb);
    //returns a valid path if the file exists on disc
    Q_INVOKABLE QString getProfilePicturePath(unsigned long userId);
    Q_INVOKABLE QString getProfilePicturePath(const QString &userName);
    void isUserProfilePictureChanged(unsigned long userId,const QString &imagePath, IsUserProfilePictureChangedCallback cb, void *opaque = (void*)0);
    Q_INVOKABLE void isUserProfilePictureChanged(unsigned long userId,const QString &imagePath, MQmlCallback *mqmlcb);

    void deleteMassage(const QString& messageIdentifier, DeleteMessageCallback cb, void *opaque = (void*)0);
    Q_INVOKABLE void deleteMassage(const QVariantMap &startMessage,MQmlCallback *mqmlcb);

signals:
    void loginRequired();
    void signUpSucceeded(unsigned long id,QString username);
    void signUpFailed(int signUpError,QString errorString);
    void authenticationSucceded(unsigned long id,QString username);
    void authenticationFailed(int authenticationError);
    void loginSucceeded(unsigned long id,QString username);
    void loginFailed(int authenticationError);
    void logOutSucceeded();
    void logOutFailed(int logOutError);
    void profilePictureRequired();
    void profilePictureChanged(QString imagePath);
    void connectTimeout();
    void connectedChanged();
    void authenticatedChanged(bool authenticated);

protected:
    bool sendRequest(MagabeData& data);

public slots:

#ifdef MAGABE_TESTS


#endif


private:
    MagabeClientManager *magabeClientManager_;

    MagabeData constructGetMessageRequest(int methodType, const Message &start, int maxNoOfMessages
                                          , bool includeStartMessage, const Sort::Order sortOrder);

    MagabeData constructGetMessageRequest(int methodType,const Conversation& conversation,int maxNoOfMessages
                                          ,const Sort::Order sortOrder);

    QString getGetMessageRequestTag(MagabeData& data);
    QString toMessageTypeHuman(int messageType);
    void getMessageCount(const QVariantMap& conversation, MessageType type, MQmlCallback* mqmlcb);
    void getLastMessage(const QVariantMap &conversation, MessageType type, MQmlCallback *mqmlcb);

};

#endif // MAGABE_CLIENT_APP_H
