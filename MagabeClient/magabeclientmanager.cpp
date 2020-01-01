/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include "magabeclientmanager.h"
#include <QDebug>
#include "Helpers/extras.h"
#include "registeredobjects.h"
#include "Helpers/fileutils.h"
#include <QCoreApplication>

/** --------------------UserInfoCallbacks--------------------------------- */
UserInfoCallbacks::UserInfoCallbacks() {
    setDefaultCallbackParamValues(generateCallbackParamValues(QString(""),UserInfo(),Errors::UserInfoError::ConnectionErrors));
}

void UserInfoCallbacks::registerCallback(const QString &username, GetUserInfoCallback cb, void *opaque){
    if((!username.isEmpty()) && (cb)){
        UserInfoCallbackWrapper cbWrapper;
        cbWrapper.tag = username;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        FourParameterCallbacks<UserInfoCallbackWrapper,QString,UserInfo,Errors::UserInfoError>::registerCallback(cbWrapper);
    }
}

/** ------------------ProfilePictureDownloadCallbacks------------------------------*/
ProfilePictureDownloadCallbacks::ProfilePictureDownloadCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(QString(""),QString(""),Errors::ProfilePictureDownloadError::ConnectionErrors));
}

void ProfilePictureDownloadCallbacks::registerCallback(const QString &username, DownloadProfilePictureCallback downloadProfilePictureCallback, ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque){
    if(containsTag(username)){
        return;
    }
    if((!username.isEmpty()) && (downloadProfilePictureCallback)){
        ProfilePictureDownloadCallbacksWrapper cbWrapper;
        cbWrapper.tag = username;
        cbWrapper.cb = downloadProfilePictureCallback;
        cbWrapper.profilePictureDownloadProgressCallback = profilePictureDownloadProgressCallback;
        cbWrapper.opaque = opaque;
        FourParameterCallbacks<ProfilePictureDownloadCallbacksWrapper,QString,QString,Errors::ProfilePictureDownloadError>::registerCallback(cbWrapper);
    }
}

void ProfilePictureDownloadCallbacks::onDownloadProgressUpdate(const QString &tag, QString fileName, float percentageCompletion, quint64 fileSize)
{
    ProfilePictureDownloadCallbacksWrapper cbWrapper = getCallbackWrapper(tag);
    if(cbWrapper.profilePictureDownloadProgressCallback){
        cbWrapper.profilePictureDownloadProgressCallback(MOpaque<>::createShared(fileName,percentageCompletion,fileSize),cbWrapper.opaque);
    }
}





/** ------------------AbstractGetMessagesCallbacks-------------------------------
 * @brief AbstractGetMessagesCallbacks::invokeCallback
 * @param tag
 * @param jsonMessages
 * @param error
 */
void AbstractGetMessagesCallbacks::invokeCallback(const QString &tag, const QString &jsonMessages, Errors::OnGetMessagesError error){

    auto cbWrapper = removeAndReturnCallbackWrapper(tag);
    if(cbWrapper.cb){
        switch (cbWrapper.methodType) {
        case  MessageType::Sent:{

            auto cb = reinterpret_cast<GetSentMessagesCallback>(cbWrapper.cb);
            if(cb){
                SentMessages sentMessages(jsonMessages);
                cb(MOpaque<>::createShared(sentMessages,error),cbWrapper.opaque);
            }else{
                qWarning() << QString("AbstractGetMessagesCallbacks::invokeCallback | Error, Cast to %1 failed").arg("GetSentMessagesCallback");
            }
            break;
        }
        case  MessageType::Received:{
            auto cb = reinterpret_cast<GetReceivedMessagesCallback>(cbWrapper.cb);
            if(cb){
                ReceivedMessages receivedMessages(jsonMessages);
                cb(MOpaque<>::createShared(receivedMessages,error),cbWrapper.opaque);
            }else{
                qWarning() << QString("AbstractGetMessagesCallbacks::invokeCallback | Error, Cast to %1 failed").arg("GetReceivedMessagesCallback");
            }
            break;
        }
        case  MessageType::SentOrReceived:{
            auto cb = reinterpret_cast<GetMessagesCallback>(cbWrapper.cb);
            if(cb){
                Messages messages(jsonMessages);
                cb(MOpaque<>::createShared(messages,error),cbWrapper.opaque);
            }else{
                qWarning() << QString("AbstractGetMessagesCallbacks::invokeCallback | Error, Cast to %1 failed").arg("GetMessagesCallback");
            }
            break;
        }
        case  MessageType::Read:{
            auto cb = reinterpret_cast<GetReadMessagesCallback>(cbWrapper.cb);
            if(cb){
                ReadMessages readMessages(jsonMessages);
                cb(MOpaque<>::createShared(readMessages,error),cbWrapper.opaque);
            }else{
                qWarning() << QString("AbstractGetMessagesCallbacks::invokeCallback | Error, Cast to %1 failed").arg("GetReadMessagesCallback");
            }
            break;
        }
        case  MessageType::UnRead:{
            auto cb = reinterpret_cast<GetUnReadMessagesCallback>(cbWrapper.cb);
            if(cb){
                UnReadMessages unReadMessages(jsonMessages);
                cb(MOpaque<>::createShared(unReadMessages,error),cbWrapper.opaque);
            }else{
                qWarning() << QString("AbstractGetMessagesCallbacks::invokeCallback | Error, Cast to %1 failed").arg("GetUnReadMessagesCallback");
            }
            break;
        }

        }//switch
    }//if

}

void AbstractGetMessagesCallbacks::registerCallback(const QString &tag, void *cb, MessageType methodType, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        AbstractGetMessagesCallbackWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.methodType = methodType;
        cbWrapper.opaque = opaque;
        TwoParameterCallbacks<AbstractGetMessagesCallbackWrapper,void*>::registerCallback(cbWrapper);
    }
}

void AbstractGetMessagesCallbacks::removeAllCallbacks(){
    auto cbs = callbacks();
    foreach (auto cbWrapper, cbs) {
        invokeCallback(cbWrapper.tag,"",Errors::OnGetMessagesError::ConnectionErrors);
    }
    cbs.clear();
}


/** ---------------------------------------------------
 * @brief SendMessageCallbacks::invokeCallback
 * @param tag
 * @param jsonMessage
 * @param error
 */
SendMessageCallbacks::SendMessageCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(QString(""),Message(),Errors::OnSendMessageError::ConnectionErrors));
}

void SendMessageCallbacks::invokeCallback(const QString &tag, const QString &jsonMessage, Errors::OnSendMessageError error){
    SendMessageCallbacksWrapper cbWrapper = getCallbackWrapper(tag);
    FourParameterCallbacks<SendMessageCallbacksWrapper,QString,Message,Errors::OnSendMessageError>::invokeCallback(tag,generateCallbackParamValues(cbWrapper.receiver,Message::fromJson(jsonMessage),error));
}

void SendMessageCallbacks::registerCallback(const QString &tag, const QString &receiver, SendMessageCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        SendMessageCallbacksWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.receiver = receiver;
        cbWrapper.opaque = opaque;
        FourParameterCallbacks<SendMessageCallbacksWrapper,QString,Message,Errors::OnSendMessageError>::registerCallback(cbWrapper);
    }
}


/** ---------------------------------------------------------
 * @brief FetchUserNamesCallbacks::invokeCallback
 * @param tag
 * @param jsonUserNames
 */
FetchUserNamesCallbacks::FetchUserNamesCallbacks(){
    QMap<unsigned long,QString> map;
    setDefaultCallbackParamValues(generateCallbackParamValues(map));
}

void FetchUserNamesCallbacks::invokeCallback(const QString &tag, const QString &jsonUserNames){
    QMap<unsigned long,QString> map;

    Extras extras; extras.addExtras("jobj",jsonUserNames);
    auto obj = extras.getJsonObjectExtras("jobj");
    if(!obj.isEmpty()){
        auto itr = obj.begin();
        while (itr != obj.end()) {
            auto id = itr.key().toULong();
            auto username = itr.value().toString();
            map.insert(id,username);
            itr++;
        }
    }
    TwoParameterCallbacks<FetchUserNamesCallbacksWrapper,QMap<unsigned long,QString>>::invokeCallback(tag,generateCallbackParamValues(map));
}

void FetchUserNamesCallbacks::registerCallback(const QString &tag, FetchUserNamesCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        FetchUserNamesCallbacksWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        TwoParameterCallbacks<FetchUserNamesCallbacksWrapper,QMap<unsigned long,QString>>::registerCallback(cbWrapper);
    }
}

/**
 * @brief GetMessageCountCallbacks::invokeCallback
 * @param tag
 * @param messageCount
 * @param error
 */
GetMessageCountCallbacks::GetMessageCountCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(0,Errors::OnGetMessageCountError::ConnectionErrors));
}

void GetMessageCountCallbacks::registerCallback(const QString &tag, GetMessageCountCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        GetMessageCountCallbackWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        ThreeParameterCallbacks<GetMessageCountCallbackWrapper,int,Errors::OnGetMessageCountError>::registerCallback(cbWrapper);
    }
}

/*------------------ProfilePictureUpload-----------------------*/
void ProfilePictureUploadCallbacks::registerCallback(const QString &tag, const QString &filePath, ProfilePictureUploadedCallback profilePictureUploadedCallback, ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback, ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback, void *opaque){
    if(!tag.isEmpty()){
        ProfilePictureUploadCallbacksWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.filePath = filePath;
        cbWrapper.cb = profilePictureUploadedCallback;
        cbWrapper.profilePictureUploadingFailedCallback = profilePictureUploadingFailedCallback;
        cbWrapper.profilePictureUploadProgressCallback = profilePictureUploadProgressCallback;
        cbWrapper.opaque = opaque;
        ThreeParameterCallbacks<ProfilePictureUploadCallbacksWrapper,QString,QString>::registerCallback(cbWrapper);
    }
}

void ProfilePictureUploadCallbacks::invokeCallback(const QString &tag, QString fileName, QString filePath){
    ProfilePictureUploadCallbacksWrapper cbWrapper = removeAndReturnCallbackWrapper(tag);//remove the wrapper, i dont need  it anymore
    if(cbWrapper.cb){
        cbWrapper.cb(MOpaque<>::createShared(fileName,filePath),cbWrapper.opaque);
    }
}

void ProfilePictureUploadCallbacks::invokeCallback(const QString &tag, QString fileName, QString filePath, QString errorString){
    ProfilePictureUploadCallbacksWrapper cbWrapper = removeAndReturnCallbackWrapper(tag);//remove the wrapper, i dont need  it anymore
    if(cbWrapper.profilePictureUploadingFailedCallback){
        cbWrapper.profilePictureUploadingFailedCallback(MOpaque<>::createShared(fileName,filePath,errorString),cbWrapper.opaque);
    }
}

void ProfilePictureUploadCallbacks::invokeCallback(const QString &tag, QString filePath, float percentageCompletion, quint64 fileSize){
    ProfilePictureUploadCallbacksWrapper cbWrapper = getCallbackWrapper(tag); //keeps the wrapper

    std::shared_ptr<MOpaque<QString,float,quint64>> params = MOpaque<>::createShared(filePath,percentageCompletion,fileSize);
    if(cbWrapper.profilePictureUploadProgressCallback){
        cbWrapper.profilePictureUploadProgressCallback(params,cbWrapper.opaque);
    }
}

void ProfilePictureUploadCallbacks::removeAllCallbacks(){
    auto profilePictureUploadCallbacks = callbacks();
    foreach (auto cbWrapper, profilePictureUploadCallbacks) {
        if(cbWrapper.profilePictureUploadingFailedCallback){
            cbWrapper.profilePictureUploadingFailedCallback(MOpaque<>::createShared(QString(""),QString(""),QString("Failed to download %1, Connection errors").arg(cbWrapper.filePath)),cbWrapper.opaque);
        }
    }
    profilePictureUploadCallbacks.clear();
}

bool ProfilePictureUploadCallbacks::containsTag(const QString &tag){
    return ThreeParameterCallbacks<ProfilePictureUploadCallbacksWrapper,QString,QString>::containsTag(tag);

}

GetLastMessageCallbacks::GetLastMessageCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(Message(),Errors::OnGetLastMessageError::ConnectionErrors));
}

void GetLastMessageCallbacks::registerCallback(const QString &tag, GetLastMessageCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        GetLastMessageCallbackWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        ThreeParameterCallbacks<GetLastMessageCallbackWrapper,Message,Errors::OnGetLastMessageError>::registerCallback(cbWrapper);
    }
}

IsUserProfilePictureChangedCallbacks::IsUserProfilePictureChangedCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(false));
}

void IsUserProfilePictureChangedCallbacks::registerCallback(const QString &tag, IsUserProfilePictureChangedCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        IsUserProfilePictureChangedCallbackWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        TwoParameterCallbacks<IsUserProfilePictureChangedCallbackWrapper,bool>::registerCallback(cbWrapper);
    }
}

DeleteMessageCallbacks::DeleteMessageCallbacks(){
    setDefaultCallbackParamValues(generateCallbackParamValues(false));
}

void DeleteMessageCallbacks::registerCallback(const QString &tag, IsUserProfilePictureChangedCallback cb, void *opaque){
    if((!tag.isEmpty()) && (cb)){
        DeleteMessageCallbackWrapper cbWrapper;
        cbWrapper.tag = tag;
        cbWrapper.cb = cb;
        cbWrapper.opaque = opaque;
        TwoParameterCallbacks<DeleteMessageCallbackWrapper,bool>::registerCallback(cbWrapper);
    }
}






#define DELETE_CALLBACKS(CALLBACKS)\
    if(CALLBACKS){\
    delete CALLBACKS;\
    CALLBACKS = nullptr;\
    }

/** -------------------MagabeClientManager----------------------------
 * @brief MagabeClientManager::MagabeClientManager
 * @param parent
 */
MagabeClientManager::MagabeClientManager(QObject *parent)
    : QObject(parent)
    , userInfoCallbacks_(new UserInfoCallbacks)
    , profilePictureDownloadCallbacks_(new ProfilePictureDownloadCallbacks)
    , abstractGetMessagesCallbacks_(new AbstractGetMessagesCallbacks)
    , sendMessageCallbacks_(new SendMessageCallbacks)
    , fetchUserNamesCallbacks_(new FetchUserNamesCallbacks)
    , authenticated_(false)
    , authenticationControllerSignalsReceiver_(new MagabeClientManagerSignalsReceiver(this))
    , getMessageCountCallbacks_(new GetMessageCountCallbacks)
    , profilePictureUploadCallbacks_(new ProfilePictureUploadCallbacks)
    , getLastMessageCallbacks_(new GetLastMessageCallbacks)
    , isUserProfilePictureChangedCallbacks_(new IsUserProfilePictureChangedCallbacks)
    , deleteMessageCallbacks_(new DeleteMessageCallbacks)


{
    ConnectionPipe::registerObject(this);
    connect(QCoreApplication::instance(),&QCoreApplication::aboutToQuit,this,&MagabeClientManager::onAppAboutToQuit);

    /*----------------------Notice Two connection techniques used----------------------------------------------*/
    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::loginRequired,
            this,&MagabeClientManager::onLoginRequired);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::signUpSucceeded,
            this,&MagabeClientManager::onSignUpSucceeded);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::signUpFailed,
            this,&MagabeClientManager::onSignUpFailed);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::authenticationSucceded,
            this,&MagabeClientManager::onAuthenticationSucceded);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::authenticationFailed,
            this,&MagabeClientManager::onAuthenticationFailed);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::loginSucceeded,
            this,&MagabeClientManager::onLoginSucceeded);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::loginFailed,
            this,&MagabeClientManager::onLoginFailed);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::logOutSucceeded,
            this,&MagabeClientManager::onLogOutSucceeded);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::logOutFailed,
            this,&MagabeClientManager::onLogOutFailed);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureRequired,
            this,&MagabeClientManager::onProfilePictureRequired);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureUploaded,
            this,&MagabeClientManager::onProfilePictureUploaded);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureUploadingFailed,
            this,&MagabeClientManager::onProfilePictureUploadingFailed);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureUploadingNotFinished,
            this,&MagabeClientManager::onProfilePictureUploadingNotFinished);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureChanged,
            this,&MagabeClientManager::onProfilePictureChanged);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::connectTimeout,
            this,&MagabeClientManager::onConnectTimeout);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureUploadProgress,
            this,&MagabeClientManager::onProfilePictureUploadProgress);

    connect(authenticationControllerSignalsReceiver_,&MagabeClientManagerSignalsReceiver::profilePictureDownloadProgress,
            this,&MagabeClientManager::onProfilePictureDownloadProgress);

    //connect GetuserInfo call back slot
    ConnectionPipe::connectSlot(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::userInfo,this,RegisteredObjects::MagabeClientManager::Slots::onUserInfo);
    ConnectionPipe::connectSlot(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::userNotFound,this,RegisteredObjects::MagabeClientManager::Slots::onUserNotFound);

    //connect ProfilePictureDownload call back slot
    ConnectionPipe::connectSlot(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::userProfilePictureDownloadFinished,this,RegisteredObjects::MagabeClientManager::Slots::onUserProfilePictureDownloadFinished);
    ConnectionPipe::connectSlot(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureDownloadFailed,this,RegisteredObjects::MagabeClientManager::Slots::onProfilePictureDownloadFailed);

    //connect GetMessages call back slot
    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::messagesReceived,this,RegisteredObjects::MagabeClientManager::Slots::onMessagesReceived);
    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::getMessageError,this,RegisteredObjects::MagabeClientManager::Slots::onGetMessageError);

    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::messageSent,this,RegisteredObjects::MagabeClientManager::Slots::onMessageSent);
    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::messageNotSent,this,RegisteredObjects::MagabeClientManager::Slots::onMessageNotSent);

    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::userNamesReceived,this,RegisteredObjects::MagabeClientManager::Slots::onUserNamesReceived);

    ConnectionPipe::connectSlot(RegisteredObjects::MagabeClient::className,RegisteredObjects::MagabeClient::Signals::connectedToMagabeserver,this,RegisteredObjects::MagabeClientManager::Slots::onConnected);
    ConnectionPipe::connectSlot(RegisteredObjects::MagabeClient::className,RegisteredObjects::MagabeClient::Signals::disconnectedFromMagabeServer,this,RegisteredObjects::MagabeClientManager::Slots::onDisconnected);

    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::messageCountReceived,this,RegisteredObjects::MagabeClientManager::Slots::onMessageCountReceived);

    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::lastMessageReceived,this,RegisteredObjects::MagabeClientManager::Slots::onLastMessageReceived);

    ConnectionPipe::connectSlot(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::isUserProfilePictureChanged,this,RegisteredObjects::MagabeClientManager::Slots::onIsUserProfilePictureChanged);

    ConnectionPipe::connectSlot(RegisteredObjects::ConversationController::className,RegisteredObjects::ConversationController::Signals::messageDeletionStatus,this,RegisteredObjects::MagabeClientManager::Slots::onMessageDeletionStatus);

}

MagabeClientManager::~MagabeClientManager()
{
    ConnectionPipe::unRegisterObject(this);


    //first
    removeAllCallbacks();//this call callbacks and cause MOpaque to be deleted//make sure u called destroy

    DELETE_CALLBACKS(userInfoCallbacks_)

            DELETE_CALLBACKS(profilePictureDownloadCallbacks_)

            DELETE_CALLBACKS(abstractGetMessagesCallbacks_)

            DELETE_CALLBACKS(sendMessageCallbacks_)

            DELETE_CALLBACKS(fetchUserNamesCallbacks_)

            DELETE_CALLBACKS(getMessageCountCallbacks_)

            DELETE_CALLBACKS(profilePictureUploadCallbacks_)

            DELETE_CALLBACKS(getLastMessageCallbacks_)

            DELETE_CALLBACKS(isUserProfilePictureChangedCallbacks_)

            DELETE_CALLBACKS(deleteMessageCallbacks_)
}

UserInfoCallbacks &MagabeClientManager::userInfoCallbacks() {
    return *userInfoCallbacks_;
}

ProfilePictureDownloadCallbacks &MagabeClientManager::profilePictureDownloadCallbacks(){
    return *profilePictureDownloadCallbacks_;
}

AbstractGetMessagesCallbacks &MagabeClientManager::abstractGetMessagesCallbacks(){
    return *abstractGetMessagesCallbacks_;
}

SendMessageCallbacks &MagabeClientManager::sendMessageCallbacks(){
    return *sendMessageCallbacks_;
}

FetchUserNamesCallbacks &MagabeClientManager::fetchUserNamesCallbacks(){
    return *fetchUserNamesCallbacks_;
}

GetMessageCountCallbacks &MagabeClientManager::getMessageCountCallbacks(){
    return *getMessageCountCallbacks_;
}

ProfilePictureUploadCallbacks &MagabeClientManager::profilePictureUploadCallbacks(){
    return *profilePictureUploadCallbacks_;
}

GetLastMessageCallbacks &MagabeClientManager::getLastMessageCallbacks(){
    return *getLastMessageCallbacks_;
}

IsUserProfilePictureChangedCallbacks &MagabeClientManager::isUserProfilePictureChangedCallbacks(){
    return *isUserProfilePictureChangedCallbacks_;
}

DeleteMessageCallbacks &MagabeClientManager::deleteMessageCallbacks(){
    return *deleteMessageCallbacks_;

}

void MagabeClientManager::removeAllCallbacks(){
    //this call callbacks and cause MOpaque to be deleted//make sure u called MOpaque<>destroy
    userInfoCallbacks_->removeAllCallbacks();
    profilePictureDownloadCallbacks_->removeAllCallbacks();
    abstractGetMessagesCallbacks_->removeAllCallbacks();
    sendMessageCallbacks_->removeAllCallbacks();
    fetchUserNamesCallbacks_->removeAllCallbacks();
    getMessageCountCallbacks_->removeAllCallbacks();
    profilePictureUploadCallbacks_->removeAllCallbacks();
    isUserProfilePictureChangedCallbacks_->removeAllCallbacks();
    deleteMessageCallbacks_->removeAllCallbacks();

}

void MagabeClientManager::onLoginRequired(){//TO ADD
    //do management
    //then
    emit loginRequired();
}

void MagabeClientManager::onSignUpSucceeded(unsigned long id, QString username)
{
    //  qDebug() << "MagabeClientManager::onSignUpSucceeded | userId == " << id << ", username == "<< username;
    setAuthenticated(true);

    emit signUpSucceeded(id,username);
}

void MagabeClientManager::onSignUpFailed(int signUpError, QString errorString)
{
    setAuthenticated(false);

    // qDebug() << "MagabeClientManager::onSignUpFailed | signUpError == " << signUpError << ", errorString == " << errorString;
    emit signUpFailed(signUpError,errorString);
}

void MagabeClientManager::onAuthenticationSucceded(unsigned long id, QString username)
{

    setAuthenticated(true);

    emit authenticationSucceded(id,username);
}

void MagabeClientManager::onAuthenticationFailed(int authenticationError)
{
    setAuthenticated(false);
    // qDebug() << "MagabeClientManager::onAuthenticationFailed | authenticationErro == " << authenticationError;

    emit authenticationFailed(authenticationError);
}

void MagabeClientManager::onLoginSucceeded(unsigned long id, QString username)
{
    setAuthenticated(true);
    // qDebug() << "MagabeClientManager::loginSucceeded | userId == " << id << ", username == "<< username;

    emit loginSucceeded(id,username);
}

void MagabeClientManager::onLoginFailed(int authenticationError)
{
    setAuthenticated(false);
    // qDebug() << "MagabeClientManager::onLoginFailed== " << authenticationErrorToString((Errors::AuthenticationError)authenticationError);
    emit loginFailed(authenticationError);
}

void MagabeClientManager::onLogOutSucceeded()
{
    setAuthenticated(false);

    removeAllCallbacks();
    //qDebug() << " MagabeClientManager::onLogOutSucceeded";

    emit logOutSucceeded();
}

void MagabeClientManager::onLogOutFailed(int logOutError){//To ADD
    //do stoff,
    //then
    emit logOutFailed(logOutError);
}

void MagabeClientManager::onProfilePictureRequired(){
    //do stoff,
    //then
    emit profilePictureRequired();
}

void MagabeClientManager::onProfilePictureUploaded(QString tag,QString fileName, QString filePath){
    profilePictureUploadCallbacks_->invokeCallback(tag,fileName,filePath);

}

void MagabeClientManager::onProfilePictureUploadingFailed(QString tag,QString fileName, QString filePath, QString errorString){

    profilePictureUploadCallbacks_->invokeCallback(tag,fileName,filePath,errorString);
}

void MagabeClientManager::onProfilePictureUploadingNotFinished(QString tag, QString fileName, QString filePath){

    profilePictureUploadCallbacks_->invokeCallback(tag,fileName,filePath,"Unknown Error | Upload not Finished");

}

void MagabeClientManager::onProfilePictureUploadProgress(QString tag, QString filePath, float percentageCompletion, quint64 fileSize)
{
    profilePictureUploadCallbacks_->invokeCallback(tag,filePath,percentageCompletion,fileSize);
}

void MagabeClientManager::onProfilePictureChanged(QString imagePath){
    //do stoff,
    //then
    emit profilePictureChanged(imagePath);
}

void MagabeClientManager::onUserInfo(QString json)
{
    // qDebug() << " MagabeClientManager::onUserInfo" << json;
    auto info = UserInfo::fromJson(json);
    auto values = userInfoCallbacks_->generateCallbackParamValues(info.username(),info,Errors::UserInfoError::NoError);
    userInfoCallbacks_->invokeCallback(info.username(),values);
}

void MagabeClientManager::onUserNotFound(QString username)
{
    //qDebug() << " MagabeClientManager::onUserNotFound" << username;
    UserInfo info;
    info.setUsername(username);
    auto values = userInfoCallbacks_->generateCallbackParamValues(username,info,Errors::UserInfoError::UserNotFound);
    userInfoCallbacks_->invokeCallback(username,values);
}

void MagabeClientManager::onUserProfilePictureDownloadFinished(QString username, QString imagePath)
{
    //qDebug() << "MagabeClientManager::onUserProfilePictureDownloadFinished" <<username << imagePath;
    auto values = profilePictureDownloadCallbacks_->generateCallbackParamValues(username,imagePath,Errors::ProfilePictureDownloadError::NoError);
    profilePictureDownloadCallbacks_->invokeCallback(username,values);
    //used by magabe client App to save the path, for other users
    if((!username.isEmpty()) && (!imagePath.isEmpty())){
        emit userProfilePictureDownloadFinished(username,imagePath);
    }
}

void MagabeClientManager::onProfilePictureDownloadFailed(QString username, int profilePictureDownloadError)
{
    //qDebug() << "MagabeClientManager::onProfilePictureDownloadFailed"<<username <<profilePictureDownloadError;
    auto values = profilePictureDownloadCallbacks_->generateCallbackParamValues(username,"",(Errors::ProfilePictureDownloadError)profilePictureDownloadError);
    profilePictureDownloadCallbacks_->invokeCallback(username,values);
}

void MagabeClientManager::onProfilePictureDownloadProgress(QString fileName, float percentageCompletion, quint64 fileSize,QString fileTag)
{
    profilePictureDownloadCallbacks_->onDownloadProgressUpdate(fileTag,fileName,percentageCompletion,fileSize);
}

void MagabeClientManager::onMessagesReceived(QString jsonMessages, QString tag)
{
    abstractGetMessagesCallbacks_->invokeCallback(tag,jsonMessages);
}

void MagabeClientManager::onGetMessageError(int onGetMessageError, QString tag)
{
    abstractGetMessagesCallbacks_->invokeCallback(tag,"",(Errors::OnGetMessagesError)onGetMessageError);
}

void MagabeClientManager::onMessageSent(QString jsonMessage, QString tag)
{
    sendMessageCallbacks_->invokeCallback(tag,jsonMessage,Errors::OnSendMessageError::NoError);
}

void MagabeClientManager::onMessageNotSent(int onSendMessageError, QString tag)
{
    sendMessageCallbacks_->invokeCallback(tag,"",(Errors::OnSendMessageError)onSendMessageError);
}

void MagabeClientManager::onUserNamesReceived(QString jsonUserNames, QString tag)
{
    fetchUserNamesCallbacks().invokeCallback(tag,jsonUserNames);
}

void MagabeClientManager::onConnectTimeout()
{
    setAuthenticated(false);
    emit connectTimeout();

    // qDebug() << "MagabeClientManager::onConnectTimeout";
}

void MagabeClientManager::onDisconnected()
{
    //qDebug() << "MagabeClientManager::onDisconnected";

    removeAllCallbacks();

    setAuthenticated(false);

    emit connectedChanged();

}

void MagabeClientManager::onConnected()
{
    //qDebug() << "MagabeClientManager::onConnected:";
    emit connectedChanged();
}

void MagabeClientManager::onMessageCountReceived(int count, QString tag)
{
    auto values = getMessageCountCallbacks_->generateCallbackParamValues(count,Errors::OnGetMessageCountError::NoError);
    getMessageCountCallbacks_->invokeCallback(tag,values);
}

void MagabeClientManager::onLastMessageReceived(QString jsonLastMessage, QString tag)
{
    auto value = getLastMessageCallbacks_->generateCallbackParamValues(Message::fromJson(jsonLastMessage),Errors::OnGetLastMessageError::NoError);
    getLastMessageCallbacks_->invokeCallback(tag,value);
}

void MagabeClientManager::onIsUserProfilePictureChanged(bool changed, QString tag)
{
    auto value = isUserProfilePictureChangedCallbacks_->generateCallbackParamValues(changed);
    isUserProfilePictureChangedCallbacks_->invokeCallback(tag,value);
}

void MagabeClientManager::onMessageDeletionStatus(bool status, QString tag)
{
    auto value = deleteMessageCallbacks_->generateCallbackParamValues(status);
    deleteMessageCallbacks_->invokeCallback(tag,value);
}

void MagabeClientManager::onAppAboutToQuit()
{
    //qDebug() << "MagabeClientManager::onAppAboutToQuit";
    removeAllCallbacks();
}

bool MagabeClientManager::isAuthenticated() const
{
    return authenticated_;
}

void MagabeClientManager::setAuthenticated(bool authenticated)
{
    if(authenticated_ != authenticated){
        authenticated_ = authenticated;
        emit authenticatedChanged(authenticated_);
    }
}

