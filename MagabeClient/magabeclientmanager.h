/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABECLIENTMANAGER_H
#define MAGABECLIENTMANAGER_H

#include <QObject>
#include "Helpers/messagetype.h"
#include "Helpers/userinfo.h"
#include "Helpers/messages.h"
#include "Helpers/connectionpipe.h"
#include "Helpers/errors.h"
#include "Helpers/sort.h"
#include "Helpers/mcppcallback.h"

/*CALLBACKS
  NOTE:TO AVOID MEMORY CORRUPTIONS; ALL CALLBACKS MUST ONLY ACCEPT SHARED MOpaque
*/

typedef void (*GetUserInfoCallback)(std::shared_ptr<MOpaque<QString,UserInfo,Errors::UserInfoError>> cbSharedMOpaque,void *opaque);
typedef void (*DownloadProfilePictureCallback)(std::shared_ptr<MOpaque<QString,QString,Errors::ProfilePictureDownloadError>> cbSharedMOpaque,void *opaque);
typedef void (*GetSentMessagesCallback)(std::shared_ptr<MOpaque<SentMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque);
typedef void (*GetReceivedMessagesCallback)(std::shared_ptr<MOpaque<ReceivedMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque);
typedef void (*GetMessagesCallback)(std::shared_ptr<MOpaque<Messages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque);
typedef void (*GetReadMessagesCallback)(std::shared_ptr<MOpaque<ReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque);
typedef void (*GetUnReadMessagesCallback)(std::shared_ptr<MOpaque<UnReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque);
typedef void (*SendMessageCallback)(std::shared_ptr<MOpaque<QString,Message,Errors::OnSendMessageError>> cbSharedMOpaque,void *opaque);
typedef void (*FetchUserNamesCallback)(std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque);
typedef void (*GetMessageCountCallback)(std::shared_ptr<MOpaque<int,Errors::OnGetMessageCountError>> cbSharedMOpaque,void *opaque);
typedef void (*ProfilePictureUploadedCallback)(std::shared_ptr<MOpaque<QString, QString>> cbSharedMOpaque, void *opaque);
typedef void (*ProfilePictureUploadingFailedCallback)(std::shared_ptr<MOpaque<QString,QString,QString>> cbSharedMOpaque, void *opaque);
typedef void (*ProfilePictureUploadProgressCallback)(std::shared_ptr<MOpaque<QString,float,quint64>> cbSharedMOpaque, void *opaque);
typedef void (*ProfilePictureDownloadProgressCallback)(std::shared_ptr<MOpaque<QString,float,quint64>> cbSharedMOpaque, void *opaque);
typedef void (*GetLastMessageCallback)(std::shared_ptr<MOpaque<Message,Errors::OnGetLastMessageError>> cbSharedMOpaque,void *opaque);
typedef void (*IsUserProfilePictureChangedCallback)(std::shared_ptr<MOpaque<bool>> cbSharedMOpaque,void *opaque);
typedef void (*DeleteMessageCallback)(std::shared_ptr<MOpaque<bool>> cbSharedMOpaque,void *opaque);

/** -----------------ProfilePictureUpload-------------------------*/

struct ProfilePictureUploadCallbacksWrapper : AbstractCallbackWrapper<ProfilePictureUploadedCallback>{
    ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback = 0;
    ProfilePictureUploadProgressCallback  profilePictureUploadProgressCallback = 0;
    QString filePath;
};

class ProfilePictureUploadCallbacks : ThreeParameterCallbacks<ProfilePictureUploadCallbacksWrapper,QString,QString> {
public:
    void registerCallback(const QString &tag,const QString &filePath
                          ,ProfilePictureUploadedCallback profilePictureUploadedCallback
                          ,ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback
                          ,ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback
                          , void *opaque);

    /*invockes ProfilePictureUploadedCallback*/
    void invokeCallback(const QString &tag, QString fileName, QString filePath);
    /*invockes ProfilePictureUploadingFailedCallback*/
    void invokeCallback(const QString &tag, QString fileName,QString filePath,QString errorString);
    /*invokes ProfilePictureUploadProgressCallback*/
    void invokeCallback(const QString &tag, QString filePath,float percentageCompletion,quint64 fileSize);
    void removeAllCallbacks();
    bool containsTag(const QString &tag);
};

/** -----------------UserInfo-------------------------*/
typedef AbstractCallbackWrapper<GetUserInfoCallback> UserInfoCallbackWrapper;
class UserInfoCallbacks : public FourParameterCallbacks<UserInfoCallbackWrapper,QString,UserInfo,Errors::UserInfoError>{
public:
    UserInfoCallbacks();
    void registerCallback(const QString &username, GetUserInfoCallback cb, void *opaque);
};

/** ---------------ProfilePictureDownload-----------------------*/
struct ProfilePictureDownloadCallbacksWrapper : public AbstractCallbackWrapper<DownloadProfilePictureCallback>{
    ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback;
};
class ProfilePictureDownloadCallbacks:public FourParameterCallbacks<ProfilePictureDownloadCallbacksWrapper,QString,QString,Errors::ProfilePictureDownloadError>{
public:
    ProfilePictureDownloadCallbacks();
    void registerCallback(const QString& username, DownloadProfilePictureCallback downloadProfilePictureCallback, ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque);
    void onDownloadProgressUpdate(const QString& tag, QString fileName, float percentageCompletion, quint64 fileSize);
};

/** -------------------------GetMessages-------------------------*/
struct AbstractGetMessagesCallbackWrapper: public AbstractCallbackWrapper<>{
    MessageType methodType;
};

class AbstractGetMessagesCallbacks : public TwoParameterCallbacks<AbstractGetMessagesCallbackWrapper,void*>{
public:
    void invokeCallback(const QString& tag,const QString& jsonMessages
                        ,Errors::OnGetMessagesError error = Errors::OnGetMessagesError::NoError);
    void registerCallback(const QString& tag,void *cb,MessageType methodType,void *opaque);
    void removeAllCallbacks();
};


/** ----------------------SendMessage------------------------*/
struct SendMessageCallbacksWrapper: public AbstractCallbackWrapper<SendMessageCallback>{
    QString receiver;
};

class SendMessageCallbacks: public FourParameterCallbacks<SendMessageCallbacksWrapper,QString,Message,Errors::OnSendMessageError>{
public:
    SendMessageCallbacks();
    void invokeCallback(const QString& tag,const QString& jsonMessage
                        ,Errors::OnSendMessageError error = Errors::OnSendMessageError::NoError);
    void registerCallback(const QString& tag,const QString& receiver,SendMessageCallback cb,void *opaque);
};


/** -----------------FetchUserNames--------------------------*/

typedef AbstractCallbackWrapper<FetchUserNamesCallback> FetchUserNamesCallbacksWrapper;
class FetchUserNamesCallbacks : public TwoParameterCallbacks<FetchUserNamesCallbacksWrapper,QMap<unsigned long,QString>>{
public:
    FetchUserNamesCallbacks();
    void invokeCallback(const QString& tag,const QString& jsonUserNames);
    void registerCallback(const QString& tag,FetchUserNamesCallback cb,void *opaque);
};

/*------------------------GetMessageCount----------------------------*/
typedef AbstractCallbackWrapper<GetMessageCountCallback> GetMessageCountCallbackWrapper;
class GetMessageCountCallbacks: public ThreeParameterCallbacks<GetMessageCountCallbackWrapper,int,Errors::OnGetMessageCountError>{
public:
    GetMessageCountCallbacks();
    void registerCallback(const QString &tag,GetMessageCountCallback cb, void *opaque);
};
/*-----------------------------------------------------------*/

typedef AbstractCallbackWrapper<GetLastMessageCallback> GetLastMessageCallbackWrapper;
class GetLastMessageCallbacks: public ThreeParameterCallbacks<GetLastMessageCallbackWrapper,Message,Errors::OnGetLastMessageError>{
public:
    GetLastMessageCallbacks();
    void registerCallback(const QString &tag, GetLastMessageCallback cb, void *opaque);
};

typedef AbstractCallbackWrapper<IsUserProfilePictureChangedCallback> IsUserProfilePictureChangedCallbackWrapper;
class IsUserProfilePictureChangedCallbacks : public TwoParameterCallbacks<IsUserProfilePictureChangedCallbackWrapper,bool>{
public:
    IsUserProfilePictureChangedCallbacks();
    void registerCallback(const QString &tag, IsUserProfilePictureChangedCallback cb, void *opaque);
};

typedef AbstractCallbackWrapper<DeleteMessageCallback> DeleteMessageCallbackWrapper;
class DeleteMessageCallbacks : public TwoParameterCallbacks<DeleteMessageCallbackWrapper,bool>{
public:
    DeleteMessageCallbacks();
    void registerCallback(const QString &tag, IsUserProfilePictureChangedCallback cb, void *opaque);
};

/** ---------------------MagabeClientManagerSignalsReceiver----------------------------
 *mostly receive signals from controllers( becoz have short and upredictable life span )
 * @brief The MagabeClientManagerSignalsReceiver class
 */
class MagabeClientManagerSignalsReceiver : public QObject
{
    Q_OBJECT
public:
    //NOTE:- do not send constructor And destructor method to .cpp files, cause moc problems
    explicit MagabeClientManagerSignalsReceiver(QObject *parent = nullptr)
        :QObject(parent)
    {
        ConnectionPipe::registerObject(this);
    }
    ~MagabeClientManagerSignalsReceiver(){
        ConnectionPipe::unRegisterObject(this);
    }

signals:
    /* No.<1> ------- Signals Connected to AuthenticationController signals------ */
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
    void profilePictureUploaded(QString tag,QString fileName,QString filePath);
    void profilePictureUploadingFailed(QString tag,QString fileName,QString filePath,QString errorString);//
    void profilePictureUploadingNotFinished(QString tag,QString fileName,QString filePath);
    void profilePictureUploadProgress(QString tag,QString filePath,float percentageCompletion,quint64 fileSize);
    void profilePictureChanged(QString imagePath);
    void profilePictureDownloadProgress(QString fileName,float percentageCompletion,quint64 fileSize,QString fileTag);

    /*--------------------------------------------------------------*/
    void connectTimeout();

};

/** -------------------MagabeClientManager-------------------------------
 *  Intercept signals and some necessary staff
 * @brief The MagabeClientManager class
 */
class MagabeClientManager : public QObject
{
    Q_OBJECT
public:
    explicit MagabeClientManager(QObject *parent = nullptr);
    ~MagabeClientManager();

    UserInfoCallbacks& userInfoCallbacks();
    ProfilePictureDownloadCallbacks& profilePictureDownloadCallbacks();
    AbstractGetMessagesCallbacks& abstractGetMessagesCallbacks();
    SendMessageCallbacks& sendMessageCallbacks();
    FetchUserNamesCallbacks& fetchUserNamesCallbacks();
    GetMessageCountCallbacks& getMessageCountCallbacks();
    ProfilePictureUploadCallbacks& profilePictureUploadCallbacks();
    GetLastMessageCallbacks& getLastMessageCallbacks();
    IsUserProfilePictureChangedCallbacks& isUserProfilePictureChangedCallbacks();
    DeleteMessageCallbacks&  deleteMessageCallbacks();

    void removeAllCallbacks();
    bool isAuthenticated() const;

signals:
    /*signals for connecting and disconnecting from server*/
    void connectToServer(QString ipAdress,QString port,int msecsTimeout);
    void disconnectFromServer();

    /* No.<1> ------- Signals Connected to AuthenticationController signals------ */
    void loginRequired();//
    void signUpSucceeded(unsigned long id,QString username);//
    void signUpFailed(int signUpError,QString errorString);//
    void authenticationSucceded(unsigned long id,QString username);//
    void authenticationFailed(int authenticationError);//
    void loginSucceeded(unsigned long id,QString username);//
    void loginFailed(int authenticationError);//
    void logOutSucceeded();//
    void logOutFailed(int logOutError);//
    void profilePictureRequired();//
    void profilePictureChanged(QString imagePath);
    void userProfilePictureDownloadFinished(QString username, QString imagePath); //used by magabe client App to save the path, for other users

    /*--------------------------------------------------------------*/

    void connectTimeout();
    void connectedChanged();

    void authenticatedChanged(bool isAuthenticated);

private slots:
    /**Slots connected to above signals for management **/

    /* No.<1> ------------------------------------------------------ */
    void onLoginRequired();
    void onSignUpSucceeded(unsigned long id, QString username);
    void onSignUpFailed(int signUpError, QString errorString);
    void onAuthenticationSucceded(unsigned long id, QString username);
    void onAuthenticationFailed(int authenticationError);
    void onLoginSucceeded(unsigned long id, QString username);
    void onLoginFailed(int authenticationError);
    void onLogOutSucceeded();
    void onLogOutFailed(int logOutError);
    void onProfilePictureRequired();

    void onProfilePictureUploaded(QString tag, QString fileName, QString filePath);
    void onProfilePictureUploadingFailed(QString tag, QString fileName,QString filePath,QString errorString);
    void onProfilePictureUploadingNotFinished(QString tag, QString fileName,QString filePath);
    void onProfilePictureUploadProgress(QString tag, QString filePath, float percentageCompletion, quint64 fileSize);

    void onProfilePictureChanged(QString imagePath);
    /*--------------------No.<1>---END------------------------------------------*/
    void onUserInfo(QString json);
    void onUserNotFound(QString username);

    /*--------------------------ProfilePictureDownload-------------------------------*/
    void onUserProfilePictureDownloadFinished(QString username, QString imagePath);
    void onProfilePictureDownloadFailed(QString username,int profilePictureDownloadError);
    void onProfilePictureDownloadProgress(QString fileName, float percentageCompletion, quint64 fileSize, QString fileTag);

    /*---------------------------------------------------------*/
    void onMessagesReceived(QString jsonMessages, QString tag);
    void onGetMessageError(int onGetMessageError, QString tag);

    /*---------------------------------------------------------*/
    void onMessageSent(QString jsonMessage, QString tag);
    void onMessageNotSent(int onSendMessageError, QString tag);
    /*---------------------------------------------------------*/
    void onUserNamesReceived(QString jsonUserNames, QString tag);

    /*---------------------------------------------------------*/
    void onConnectTimeout();

    void onDisconnected();
    void onConnected();

    void onMessageCountReceived(int count,QString tag);
    void onLastMessageReceived(QString jsonLastMessage, QString tag);
    void onIsUserProfilePictureChanged(bool changed,QString tag);
    void onMessageDeletionStatus(bool status, QString tag);
    void onAppAboutToQuit();


private:
    UserInfoCallbacks  *userInfoCallbacks_;
    ProfilePictureDownloadCallbacks *profilePictureDownloadCallbacks_;
    AbstractGetMessagesCallbacks *abstractGetMessagesCallbacks_;
    SendMessageCallbacks *sendMessageCallbacks_;
    FetchUserNamesCallbacks *fetchUserNamesCallbacks_;
    bool authenticated_;
    void setAuthenticated(bool isAuthenticated);
    MagabeClientManagerSignalsReceiver *authenticationControllerSignalsReceiver_;
    GetMessageCountCallbacks *getMessageCountCallbacks_;
    ProfilePictureUploadCallbacks *profilePictureUploadCallbacks_;
    GetLastMessageCallbacks *getLastMessageCallbacks_;
    IsUserProfilePictureChangedCallbacks *isUserProfilePictureChangedCallbacks_;
    DeleteMessageCallbacks  *deleteMessageCallbacks_;
};

#endif // MAGABECLIENTMANAGER_H
