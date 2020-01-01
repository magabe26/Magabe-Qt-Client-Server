/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabeclientapp.h"
#include "debug.h"
#include <QDebug>
#include "Helpers/connectionpipe.h"
#include "Client/authentication.h"
#include "ControllerIdentifier/controlleridentifier.h"
#include "security.h"
#include "registeredobjects.h"
#include "Helpers/fileutils.h"
#include "Controllers/authenticationcontroller.h"
#include "Helpers/datawritter.h"
#include <QList>
#include "Models/userprofilepicturemodel.h"

/** -----------------MagabeClientApp-------------------------------
 * @brief MagabeClientApp::MagabeClientApp
 * @param parent
 */
MagabeClientApp::MagabeClientApp(QObject *parent) : QObject(parent)
  , magabeClientManager_(new MagabeClientManager(this))
{
    ConnectionPipe::registerObject(this);

    //updating profile picture path for other users
    connect(magabeClientManager_,&MagabeClientManager::userProfilePictureDownloadFinished
            ,[this](QString username, QString imagePath){
        if(userName() != username){
            auto mOpaque = MOpaque<>::createUnique(imagePath);
            getUserInfo(username,[](std::shared_ptr<MOpaque<QString,UserInfo,Errors::UserInfoError>> cbSharedMOpaque,void *opaque){
                const QString& username = std::get<0>(cbSharedMOpaque->tuple());
                const UserInfo& info = std::get<1>(cbSharedMOpaque->tuple());
                //const Errors::UserInfoError& error =  std::get<2>(cbSharedMOpaque->tuple());

                auto mOpaque = reinterpret_cast<MOpaque<QString>*>(opaque);
                if(mOpaque){
                    auto imagePath = std::get<0>(mOpaque->tuple());

                    MOpaque<>::destroy(mOpaque);//must destroy after getting the vlue

                    const auto userId = info.id();
                    UserProfilePictureModel model;
                    model.savePath(userId,username,imagePath);
                }
            },mOpaque);
        }
    });

    //connect signals
    connect(magabeClientManager_,&MagabeClientManager::loginRequired,this,&MagabeClientApp::loginRequired);
    connect(magabeClientManager_,&MagabeClientManager::signUpSucceeded,this,&MagabeClientApp::signUpSucceeded);
    connect(magabeClientManager_,&MagabeClientManager::signUpFailed,this,&MagabeClientApp::signUpFailed);
    connect(magabeClientManager_,&MagabeClientManager::authenticationSucceded,this,&MagabeClientApp::authenticationSucceded);
    connect(magabeClientManager_,&MagabeClientManager::authenticationFailed,this,&MagabeClientApp::authenticationFailed);
    connect(magabeClientManager_,&MagabeClientManager::loginSucceeded,this,&MagabeClientApp::loginSucceeded);
    connect(magabeClientManager_,&MagabeClientManager::loginFailed,this,&MagabeClientApp::loginFailed);
    connect(magabeClientManager_,&MagabeClientManager::logOutSucceeded,this,&MagabeClientApp::logOutSucceeded);
    connect(magabeClientManager_,&MagabeClientManager::logOutFailed,this,&MagabeClientApp::logOutFailed);
    connect(magabeClientManager_,&MagabeClientManager::profilePictureRequired,this,&MagabeClientApp::profilePictureRequired);
    connect(magabeClientManager_,&MagabeClientManager::profilePictureChanged,this,&MagabeClientApp::profilePictureChanged);
    connect(magabeClientManager_,&MagabeClientManager::connectTimeout,this,&MagabeClientApp::connectTimeout);
    connect(magabeClientManager_,&MagabeClientManager::connectedChanged,this,&MagabeClientApp::connectedChanged);
    connect(magabeClientManager_,&MagabeClientManager::authenticatedChanged,this,&MagabeClientApp::authenticatedChanged);





    /*--------------------------------TESTS-------------------------------------------*/

#ifdef MAGABE_TESTS


    connect(this,&MagabeClientApp::authenticationSucceded,[=](unsigned long id,QString username){


    });

#endif
    /*--------------------------------TESTS-------------------------------------------*/

}

MagabeClientApp::~MagabeClientApp()
{
    ConnectionPipe::unRegisterObject(this);

}

QString MagabeClientApp::authenticationErrorToString(Errors::AuthenticationError authenticationError){
    switch (authenticationError) {
    case Errors::AuthenticationError::NoError :return "NoError";
    case Errors::AuthenticationError::UserAlreadyAuthenticated: return "UserAlreadyAuthenticatedError";
    case Errors::AuthenticationError::IllegalAppUsersState: return "IllegalAppUsersStateError";
    case Errors::AuthenticationError::UserNotRegistered: return "UserNotRegisteredError";
    case Errors::AuthenticationError::UserIsBlacklisted: return "UserIsBlacklistedError";
    case Errors::AuthenticationError::UnknownError: return "UnknownError";
    case Errors::AuthenticationError::UserNameNotSet: return "UserNameNotSetError";
    case Errors::AuthenticationError::AuthKeyNotSet: return "AuthKeyNotSetError";
    case Errors::AuthenticationError::PasswordNotSet: return "PasswordNotSetError";
    case Errors::AuthenticationError::InvalidCredentials: return "InvalidCredentialsError";
    default: return "NoError";
    }
}

void MagabeClientApp::signUp(const QString &username, const QString &firstName, const QString &lastName, const QString &dateOfBirth, QChar gender, const QString &password, const QString &email)
{
    Authentication::signUp(username, firstName,lastName,dateOfBirth,gender, password, email);
}

void MagabeClientApp::login(const QString &username, const QString &password)
{
    Authentication::login(username,password);
}

void MagabeClientApp::logOut()
{
    Authentication:: logOut();
}

bool MagabeClientApp::isAuthenticated() const
{
    if(magabeClientManager_){
        return magabeClientManager_->isAuthenticated();
    }else{
        return false;
    }
}

bool MagabeClientApp::isValidConversationIdentifier(const QString &conversationIdentifier){
    return Conversation::isValidConversationIdentifier(conversationIdentifier);
}

bool MagabeClientApp::isValidMessageIdentifier(const QString &messageIdentifier){
    return Message::isValidMessageIdentifier(messageIdentifier);
}

void MagabeClientApp::getSentMessages(const Message &start
                                      , GetSentMessagesCallback cb
                                      , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Sent,start,maxNoOfMessages,includeStartMessage,sortOrder);
    const auto tag = getGetMessageRequestTag(data);


    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Sent,opaque);
        }else{
            cb(MOpaque<>::createShared(SentMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(SentMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }
}

void MagabeClientApp::getSentMessages(const QVariantMap& startMessage,bool includeStartMessage
                                      ,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb)
{
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Message message; message.messageIdentifier_ = messageIdentifier;

        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::InvalidMessageIdentifier);
            return;
        }

        CurrentUserIdModel model;
        Conversation conversation = message.getConversation();
        const auto id = model.getCurrentId();
        if(id == 0){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
            return;
        }else if(!conversation.isInvolved(id)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
        }else{
            getSentMessages(message,[](std::shared_ptr<MOpaque<SentMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){

                SentMessages sentMessages = std::get<0>(cbSharedMOpaque->tuple());
                Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                if(mqmlcb)
                    mqmlcb->call(sentMessages.toJson(),(int)error);
            },maxNoOfMessages,includeStartMessage,order,mqmlcb);
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getSentMessages(const QVariantMap& startMessage,bool includeStartMessage,int maxNoOfMessages,int sortOrder,MQmlCallback* mqmlcb)");
    }
}

void MagabeClientApp::getSentMessages(const Conversation &conversation
                                      , GetSentMessagesCallback cb
                                      , int maxNoOfMessages, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Sent,conversation,maxNoOfMessages,sortOrder);
    const auto tag = getGetMessageRequestTag(data);


    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Sent,opaque);
        }else{
            cb(MOpaque<>::createShared(SentMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(SentMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getSentMessages(const QVariantMap &conversation, int maxNoOfMessages
                                      , int sortOrder, MQmlCallback *mqmlcb)
{

    if(mqmlcb){

        const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Conversation conversation(conversationIdentifier);
        if(!conversation.isValid()){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::ConversationNotFound);
            return;
        }else{
            CurrentUserIdModel model;
            const auto id = model.getCurrentId();
            if(id == 0){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
                return;
            }else if(!conversation.isInvolved(id)){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
            }else{
                getSentMessages(conversation,[](std::shared_ptr<MOpaque<SentMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){

                    SentMessages sentMessages = std::get<0>(cbSharedMOpaque->tuple());
                    Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                    MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                    if(mqmlcb)
                        mqmlcb->call(sentMessages.toJson(),(int)error);
                },maxNoOfMessages,order,mqmlcb);
            }
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getSentMessages(const QVariantMap &conversation, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getReceivedMessages(const Message &start, GetReceivedMessagesCallback cb
                                          , int maxNoOfMessages
                                          , bool includeStartMessage, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Received,start,maxNoOfMessages,includeStartMessage,sortOrder);
    const auto tag = getGetMessageRequestTag(data);


    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Received,opaque);
        }else{
            cb(MOpaque<>::createShared(ReceivedMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(ReceivedMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getReceivedMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Message message; message.messageIdentifier_ = messageIdentifier;

        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::InvalidMessageIdentifier);
            return;
        }

        CurrentUserIdModel model;
        Conversation conversation = message.getConversation();
        const auto id = model.getCurrentId();
        if(id == 0){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
            return;
        }else if(!conversation.isInvolved(id)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
        }else{
            getReceivedMessages(message,[](std::shared_ptr<MOpaque<ReceivedMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                ReceivedMessages receivedMessages = std::get<0>(cbSharedMOpaque->tuple());
                Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                if(mqmlcb)
                    mqmlcb->call(receivedMessages.toJson(),(int)error);
            },maxNoOfMessages,includeStartMessage,order,mqmlcb);
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getReceivedMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }

}

void MagabeClientApp::getReceivedMessages(const Conversation &conversation
                                          , GetReceivedMessagesCallback cb
                                          , int maxNoOfMessages, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Received,conversation,maxNoOfMessages,sortOrder);
    const auto tag = getGetMessageRequestTag(data);


    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Received,opaque);
        }else{
            cb(MOpaque<>::createShared(ReceivedMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(ReceivedMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getReceivedMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Conversation conversation(conversationIdentifier);
        if(!conversation.isValid()){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::ConversationNotFound);
            return;
        }else{
            CurrentUserIdModel model;
            const auto id = model.getCurrentId();
            if(id == 0){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
                return;
            }else if(!conversation.isInvolved(id)){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
            }else{
                getReceivedMessages(conversation,[](std::shared_ptr<MOpaque<ReceivedMessages,Errors::OnGetMessagesError>> cbSharedMOpaque ,void *opaque){
                    ReceivedMessages receivedMessages = std::get<0>(cbSharedMOpaque->tuple());
                    Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                    MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                    if(mqmlcb)
                        mqmlcb->call(receivedMessages.toJson(),(int)error);
                },maxNoOfMessages,order,mqmlcb);
            }
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getReceivedMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }

}

void MagabeClientApp::getMessages(const Message &start
                                  , GetMessagesCallback cb
                                  , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::SentOrReceived,start,maxNoOfMessages,includeStartMessage,sortOrder);
    const auto tag = getGetMessageRequestTag(data);

    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::SentOrReceived,opaque);
        }else{
            cb(MOpaque<>::createShared(Messages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(Messages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Message message; message.messageIdentifier_ = messageIdentifier;

        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::InvalidMessageIdentifier);
            return;
        }

        CurrentUserIdModel model;
        Conversation conversation = message.getConversation();
        const auto id = model.getCurrentId();
        if(id == 0){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
            return;
        }else if(!conversation.isInvolved(id)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
        }else{
            getMessages(message,[](std::shared_ptr<MOpaque<Messages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                Messages messages = std::get<0>(cbSharedMOpaque->tuple());
                Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                if(mqmlcb)
                    mqmlcb->call(messages.toJson(),(int)error);
            },maxNoOfMessages,includeStartMessage,order,mqmlcb);
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getMessages(const Conversation &conversation
                                  , GetMessagesCallback cb
                                  , int maxNoOfMessages, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::SentOrReceived,conversation,maxNoOfMessages,sortOrder);
    const auto tag = getGetMessageRequestTag(data);

    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::SentOrReceived,opaque);
        }else{
            cb(MOpaque<>::createShared(Messages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(Messages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Conversation conversation(conversationIdentifier);
        if(!conversation.isValid()){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::ConversationNotFound);
            return;
        }else{
            CurrentUserIdModel model;
            const auto id = model.getCurrentId();
            if(id == 0){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
                return;
            }else if(!conversation.isInvolved(id)){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
            }else{
                getMessages(conversation,[](std::shared_ptr<MOpaque<Messages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                    Messages messages = std::get<0>(cbSharedMOpaque->tuple());
                    Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                    MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                    if(mqmlcb)
                        mqmlcb->call(messages.toJson(),(int)error);
                },maxNoOfMessages,order,mqmlcb);
            }
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getReadMessages(const Message &start
                                      , GetReadMessagesCallback cb
                                      , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Read,start,maxNoOfMessages,includeStartMessage,sortOrder);
    const auto tag = getGetMessageRequestTag(data);


    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Read,opaque);
        }else{
            cb(MOpaque<>::createShared(ReadMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(ReadMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getReadMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Message message; message.messageIdentifier_ = messageIdentifier;

        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::InvalidMessageIdentifier);
            return;
        }

        CurrentUserIdModel model;
        Conversation conversation = message.getConversation();
        const auto id = model.getCurrentId();
        if(id == 0){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
            return;
        }else if(!conversation.isInvolved(id)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
        }else{
            getReadMessages(message,[](std::shared_ptr<MOpaque<ReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                ReadMessages readMessages = std::get<0>(cbSharedMOpaque->tuple());
                Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                if(mqmlcb)
                    mqmlcb->call(readMessages.toJson(),(int)error);
            },maxNoOfMessages,includeStartMessage,order,mqmlcb);
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getReadMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getReadMessages(const Conversation &conversation
                                      , GetReadMessagesCallback cb
                                      , int maxNoOfMessages, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::Read,conversation,maxNoOfMessages,sortOrder);
    const auto tag = getGetMessageRequestTag(data);

    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::Read,opaque);
        }else{
            cb(MOpaque<>::createShared(ReadMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(ReadMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::getReadMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Conversation conversation(conversationIdentifier);
        if(!conversation.isValid()){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::ConversationNotFound);
            return;
        }else{
            CurrentUserIdModel model;
            const auto id = model.getCurrentId();
            if(id == 0){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
                return;
            }else if(!conversation.isInvolved(id)){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
            }else{
                getReadMessages(conversation,[](std::shared_ptr<MOpaque<ReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                    ReadMessages readMessages = std::get<0>(cbSharedMOpaque->tuple());
                    Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                    MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                    if(mqmlcb)
                        mqmlcb->call(readMessages.toJson(),(int)error);
                },maxNoOfMessages,order,mqmlcb);
            }
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getReadMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getUnReadMessages(const Message &start
                                        , GetUnReadMessagesCallback cb
                                        , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::UnRead,start,maxNoOfMessages,includeStartMessage,sortOrder);
    const auto tag = getGetMessageRequestTag(data);

    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::UnRead,opaque);
        }else{
            cb(MOpaque<>::createShared(UnReadMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(UnReadMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }
}

void MagabeClientApp::getUnReadMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Message message; message.messageIdentifier_ = messageIdentifier;

        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::InvalidMessageIdentifier);
            return;
        }

        CurrentUserIdModel model;
        Conversation conversation = message.getConversation();
        const auto id = model.getCurrentId();
        if(id == 0){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
            return;
        }else if(!conversation.isInvolved(id)){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
        }else{
            getUnReadMessages(message,[](std::shared_ptr<MOpaque<UnReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                UnReadMessages unReadMessages = std::get<0>(cbSharedMOpaque->tuple());
                Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                if(mqmlcb)
                    mqmlcb->call(unReadMessages.toJson(),(int)error);
            },maxNoOfMessages,includeStartMessage,order,mqmlcb);
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getUnReadMessages(const QVariantMap &startMessage, bool includeStartMessage, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }

}

void MagabeClientApp::getUnReadMessages(const Conversation &conversation
                                        , GetUnReadMessagesCallback cb
                                        , int maxNoOfMessages, const Sort::Order sortOrder, void *opaque)
{
    if(!cb){
        return;
    }

    auto data = constructGetMessageRequest((int)MessageType::UnRead,conversation,maxNoOfMessages,sortOrder);
    const auto tag = getGetMessageRequestTag(data);

    if(isConnected()){
        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->abstractGetMessagesCallbacks().registerCallback(tag,(void*)cb,MessageType::UnRead,opaque);
        }else{
            cb(MOpaque<>::createShared(UnReadMessages(),Errors::OnGetMessagesError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(UnReadMessages(),Errors::OnGetMessagesError::ConnectionErrors),opaque);
    }
}

void MagabeClientApp::getUnReadMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)
{
    if(mqmlcb){

        const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
        const Sort::Order order = ((Sort::Order)sortOrder == Sort::Order::DESC) ? Sort::Order::DESC : Sort::Order::ASC;
        Conversation conversation(conversationIdentifier);
        if(!conversation.isValid()){
            if(mqmlcb)
                mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::ConversationNotFound);
            return;
        }else{
            CurrentUserIdModel model;
            const auto id = model.getCurrentId();
            if(id == 0){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotAuthenticated);
                return;
            }else if(!conversation.isInvolved(id)){
                if(mqmlcb)
                    mqmlcb->call(QString("{}"),(int)Errors::OnGetMessagesError::UserNotInvolvedInConversation);
            }else{
                getUnReadMessages(conversation,[](std::shared_ptr<MOpaque<UnReadMessages,Errors::OnGetMessagesError>> cbSharedMOpaque,void *opaque){
                    UnReadMessages unReadMessages = std::get<0>(cbSharedMOpaque->tuple());
                    Errors::OnGetMessagesError error = std::get<1>(cbSharedMOpaque->tuple());

                    MQmlCallback *mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
                    if(mqmlcb)
                        mqmlcb->call(unReadMessages.toJson(),(int)error);
                },maxNoOfMessages,order,mqmlcb);
            }
        }

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::getUnReadMessages(const QVariantMap &conversation, int maxNoOfMessages, int sortOrder, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::sendMessage(const QString &receiverUserName, const QString &messageText, SendMessageCallback cb, void *opaque)
{
    if(!cb){
        return;
    }

    if(receiverUserName.isEmpty()){
        cb(MOpaque<>::createShared(receiverUserName,Message(),Errors::OnSendMessageError::UserNotFound),opaque);
        return;
    }

    if(isConnected()){
        QString tag;
createTag:
        tag = QString("%1:%2:%3:%4").arg(receiverUserName)
                .arg(QDateTime::currentMSecsSinceEpoch()).arg(((unsigned long long)cb))
                .arg(messageText.length());
        if(magabeClientManager_->sendMessageCallbacks().containsTag(tag)){
            goto createTag;
        }

        MagabeData data;
        data.setController("ConversationController")
                .setAction("ActionSendMessage")
                .addExtras("receiver",receiverUserName)
                .addExtras("messageText",messageText)
                .addExtras("sendMessageRequestTag",tag);

        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->sendMessageCallbacks().registerCallback(tag,receiverUserName,cb,opaque);
        }else{
            cb(MOpaque<>::createShared(receiverUserName,Message(),Errors::OnSendMessageError::UnknownError),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(receiverUserName,Message(),Errors::OnSendMessageError::ConnectionErrors),opaque);
    }

}

void MagabeClientApp::sendMessage(const QString &receiverUserName, const QString &messageText, MQmlCallback *mqmlcb)
{
    if(mqmlcb){
        sendMessage(receiverUserName,messageText,[](std::shared_ptr<MOpaque<QString,Message,Errors::OnSendMessageError>> cbSharedMOpaque,void *opaque){

            //const QString &receiver = std::get<0>(cbSharedMOpaque->tuple());
            Message sent = std::get<1>(cbSharedMOpaque->tuple());
            Errors::OnSendMessageError error = std::get<2>(cbSharedMOpaque->tuple());

            MQmlCallback *cb = reinterpret_cast<MQmlCallback *>(opaque);

            Extras extras;
            extras.addExtras("sent",sent.toJson());
            if(cb){
                cb->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) already destroyed").arg("void MagabeClientApp::sendMessage(const QString &receiverUserName, const QString &messageText, MQmlCallback *mqmlcb)");
            }
        },mqmlcb);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("void MagabeClientApp::sendMessage(const QString &receiverUserName, const QString &messageText, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::sendMessage(unsigned long receiverUserId, const QString &messageText
                                  , SendMessageCallback cb,void *opaque)
{

    if(!cb){
        return;
    }

    if(!isConnected()){
        cb(MOpaque<>::createShared(QString(""),Message(),Errors::OnSendMessageError::ConnectionErrors),opaque);
        return;
    }

    if(receiverUserId == 0){
        cb(MOpaque<>::createShared(QString(""),Message(),Errors::OnSendMessageError::UserNotFound),opaque);
        return;
    }

    QList<unsigned long> userIds;
    userIds << receiverUserId;

    typedef  MOpaque<MagabeClientApp*,SendMessageCallback,const QString&,void*> FetchUserNamesOpaqueType;
    auto fetchUserNamesOpaque =  MOpaque<>::createUnique(this,cb,messageText,opaque);

    fetchUserNames(userIds,[](std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque){
        QMap<unsigned long,QString> map = std::get<0>(cbSharedMOpaque->tuple());

        auto fetchUserNamesOpaque =  reinterpret_cast<FetchUserNamesOpaqueType*>(opaque);
        if(!fetchUserNamesOpaque){
            qWarning() << "MagabeClientApp::sendMessage | Error, fetchUserNamesOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = fetchUserNamesOpaque->tuple();
        const auto magabeClientApp = std::get<0>(tuple);
        auto sendMessageCallback = std::get<1>(tuple);
        auto messageText = std::get<2>(tuple);
        auto sendMessageOpaque = std::get<3>(tuple);

        MOpaque<>::destroy(fetchUserNamesOpaque);//after getting the value, destroy, very important

        auto keys = map.keys();
        if(keys.length() > 0){
            auto username  = map.value(keys.at(0));

            if(!username.isEmpty()){
                magabeClientApp->sendMessage(username,messageText,sendMessageCallback,sendMessageOpaque);
            }else{
                sendMessageCallback(MOpaque<>::createShared(QString(""),Message(),Errors::OnSendMessageError::UserNotFound),sendMessageOpaque);
            }
        }else{
            sendMessageCallback(MOpaque<>::createShared(QString(""),Message(),Errors::OnSendMessageError::UserNotFound),sendMessageOpaque);
        }
    },fetchUserNamesOpaque);

}

void MagabeClientApp::sendMessage(unsigned long receiverUserId, const QString &messageText, MQmlCallback *mqmlcb)
{
    if(mqmlcb){
        sendMessage(receiverUserId,messageText,[](std::shared_ptr<MOpaque<QString,Message,Errors::OnSendMessageError>> cbSharedMOpaque,void *opaque){

            //const QString &receiver = std::get<0>(cbSharedMOpaque->tuple());
            Message sent = std::get<1>(cbSharedMOpaque->tuple());
            Errors::OnSendMessageError error = std::get<2>(cbSharedMOpaque->tuple());

            MQmlCallback *cb = reinterpret_cast<MQmlCallback *>(opaque);

            Extras extras;
            extras.addExtras("sent",sent.toJson());
            if(cb){
                cb->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) already destroyed").arg("MagabeClientApp::sendMessage(unsigned long receiverUserId, const QString &messageText, MQmlCallback *mqmlcb)");
            }
        },mqmlcb);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::sendMessage(unsigned long receiverUserId, const QString &messageText, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::fetchUserNames(const QList<unsigned long>& userIds, FetchUserNamesCallback cb, void *opaque)
{

    if(!cb){
        return;
    }

    QMap<unsigned long,QString> map;

    if(userIds.isEmpty()){
        cb(MOpaque<>::createShared(map),opaque);
        return;
    }

    QStringList ids;
    QString subTag("[");
    int i=0;
    foreach (auto id, userIds) {
        const auto strId = QString::number(id);
        ids << strId;
        if((i > 0) && (i < ids.length())){
            subTag.append(',') ;
        }
        subTag.append(strId);
        i++;
    }
    subTag.append(']');

    if(isConnected()){
        QString tag;
createTag:
        tag = QString("%1:%2:%3:%4").arg(subTag)
                .arg(QDateTime::currentMSecsSinceEpoch()).arg(((unsigned long long)cb))
                .arg(ids.length());
        if(magabeClientManager_->fetchUserNamesCallbacks().containsTag(tag)){
            goto createTag;
        }

        MagabeData data;
        data.setController("UserController")
                .setAction("ActionFetchUserNames")
                .addExtras("userIds",ids)
                .addExtras("fetchUserNamesRequestTag",tag);

        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->fetchUserNamesCallbacks().registerCallback(tag,cb,opaque);
        }else{
            cb(MOpaque<>::createShared(map),opaque);
        }
    }else{
        cb(MOpaque<>::createShared(map),opaque);
    }

}

void MagabeClientApp::fetchUserNames(const QVariantList &userIds, MQmlCallback *mqmlcb)
{

    if(!mqmlcb){
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null, can not continue").arg("MagabeClientApp::fetchUserNames(const QList<unsigned long> &userIds, MQmlCallback *mqmlcb)");
        return;
    }


    if(!isConnected()){
        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        return;
    }

    if(userIds.isEmpty()){
        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        return;
    }

    QList<unsigned long> ids;
    foreach (auto variant, userIds) {
        bool ok = false;
        unsigned long id = variant.toString().toULong(&ok);
        if(ok){
            ids.append(id);
        }
    }

    if(ids.isEmpty()){
        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        return;
    }

    fetchUserNames(ids,[](std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque){
        QMap<unsigned long,QString> map = std::get<0>(cbSharedMOpaque->tuple());

        auto mqmlcb =  reinterpret_cast<MQmlCallback*>(opaque);
        auto keys = map.keys();
        if(!keys.isEmpty()){
            Extras names;
            foreach (auto key, keys) {
                names.addExtras(QString::number(key),map.value(key));
            }
            Extras rootObj;
            rootObj.addExtras("usernames",names.getJsonExtras());
            if(mqmlcb)
                mqmlcb->call(QVariant(rootObj.getJsonExtras().data()),0/*no error*/);
        }else{
            if(mqmlcb)
                mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        }

    },mqmlcb);
}

void MagabeClientApp::getUserName(unsigned long userId, MQmlCallback *mqmlcb)
{
    if(!mqmlcb){
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null, can not continue").arg("MagabeClientApp::getUserName(unsigned long userId, MQmlCallback *mqmlcb)");
        return;
    }


    if(!isConnected()){
        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        return;
    }

    if(userId == 0){
        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        return;
    }

    QList<unsigned long> userIds;
    userIds << userId;

    typedef  MOpaque<unsigned long,MQmlCallback*> FetchUserNamesOpaqueType;//remember to change,depend on stored types
    auto fetchUserNamesOpaque =  MOpaque<>::createUnique(userId,mqmlcb);

    fetchUserNames(userIds,[](std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque){
        QMap<unsigned long,QString> map = std::get<0>(cbSharedMOpaque->tuple());

        auto fetchUserNamesOpaque =  reinterpret_cast<FetchUserNamesOpaqueType*>(opaque);
        if(!fetchUserNamesOpaque){
            qWarning() << "MagabeClientApp::getUserName | Error, fetchUserNamesOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = fetchUserNamesOpaque->tuple();
        auto userId = std::get<0>(tuple);
        auto mqmlcb = std::get<1>(tuple);

        MOpaque<>::destroy(fetchUserNamesOpaque);//after getting the value, destroy, very important

        auto keys = map.keys();
        if(keys.length() > 0){
            auto receivedId = keys.at(0);
            auto username  = map.value(receivedId);
            if(!username.isEmpty()){
                if(receivedId != userId){
                    qWarning()<< "MagabeClientApp::getUserName | Error. received a name with defient userId,Server side implimentation error";
                    if(mqmlcb)
                        mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
                }else{
                    Extras extras;
                    extras.addExtras(QString::number(receivedId),username);
                    if(mqmlcb)
                        mqmlcb->call(QVariant(extras.getJsonExtras().data()),0/*no error*/);
                }
            }else{
                if(mqmlcb)
                    mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
            }
        }else{
            if(mqmlcb)
                mqmlcb->call(QVariant(QString("{}")),10/*just return 10*/);
        }

    },fetchUserNamesOpaque);
}

QString MagabeClientApp::decodeMessage(const QString &encodedMessage){
    return QByteArray::fromHex(encodedMessage.toLatin1()).data();
}

QString MagabeClientApp::generateConversationIdentifier(const QString &messageIdentifier){
    auto arry = messageIdentifier.split(':');
    if(arry.length() == 3){
        bool ok = false;
        const unsigned long id1 = arry.at(0).toULong(&ok);//check 1
        if(!ok){
            return QString("");
        }

        const unsigned id2 = arry.at(1).toULong(&ok);//check 2
        if(!ok){
            return QString("");
        }

        if((id1 == id2) || (id1 == 0) || (id2 == 0)){
            return QString("");
        }

        const unsigned smsNo = arry.at(2).toULong(&ok);//check 3
        Q_UNUSED(smsNo)
        if(!ok){
            return QString("");
        }
        return Conversation::generateConversationIdentifier(id1,id2);

    }else{
        return QString("");
    }
}

unsigned long MagabeClientApp::userId(){
    CurrentUserIdModel model;
    return model.getCurrentId();
}

QString MagabeClientApp::userName(){

    AuthenticationModel model;
    return model.getUserName(userId());
}

QString MagabeClientApp::authKey(){

    AuthenticationModel model;
    return model.getAuthKey(userId());
}

QString MagabeClientApp::password(){

    AuthenticationModel model;
    return model.getPassword(userId());
}

void MagabeClientApp::getMessageCount(const Conversation &conversation, GetMessageCountCallback cb,MessageType type, void *opaque)
{

    if(!cb){
        return;
    }

    if(!isConnected()){
        cb(MOpaque<>::createShared(0,Errors::OnGetMessageCountError::ConnectionErrors),opaque);
        return;
    }

    if(!const_cast<Conversation &>(conversation).isValid()){
        cb(MOpaque<>::createShared(0,Errors::OnGetMessageCountError::InvalidConversationIdentifier),opaque);
        return;
    }

    const auto conversationIdentifier = const_cast<Conversation &>(conversation).conversationIdentifier();
    MagabeData data;
    QString tag;
createTag:
    tag = QString("%1:%2:%3")
            .arg(QString("%1[%2]").arg(toMessageTypeHuman((int)type)).arg((int)type))
            .arg(conversationIdentifier)
            .arg(QDateTime::currentMSecsSinceEpoch());

    if(magabeClientManager_->getMessageCountCallbacks().containsTag(tag)){
        goto  createTag;
    }


    data.setController("ConversationController")
            .setAction("ActionGetMessageCount")
            .addExtras("MessageType",(int)type)
            .addExtras("conversationIdentifier", conversationIdentifier)
            .addExtras("getMessageCountRequestTag",tag);

    const bool sent = sendRequest(data);
    if(sent){
        magabeClientManager_->getMessageCountCallbacks().registerCallback(tag,cb,opaque);
    }else{
        cb(MOpaque<>::createShared(0,Errors::OnGetMessageCountError::UnknownError),opaque);
    }
}

void MagabeClientApp::getLastMessage(const Conversation &conversation, GetLastMessageCallback cb,MessageType type, void *opaque)
{

    if(!cb){
        return;
    }

    if(!isConnected()){
        cb(MOpaque<>::createShared(Message(),Errors::OnGetLastMessageError::ConnectionErrors),opaque);
        return;
    }

    if(!const_cast<Conversation &>(conversation).isValid()){
        cb(MOpaque<>::createShared(Message(),Errors::OnGetLastMessageError::InvalidConversationIdentifier),opaque);
        return;
    }

    const auto conversationIdentifier = const_cast<Conversation &>(conversation).conversationIdentifier();
    MagabeData data;
    QString tag;
createTag:
    tag = QString("%1:%2:%3")
            .arg(QString("%1[%2]").arg(toMessageTypeHuman((int)type)+"|Last").arg((int)type))
            .arg(conversationIdentifier)
            .arg(QDateTime::currentMSecsSinceEpoch());

    if(magabeClientManager_->getLastMessageCallbacks().containsTag(tag)){
        goto  createTag;
    }


    data.setController("ConversationController")
            .setAction("ActionGetLastMessage")
            .addExtras("MessageType",(int)type)
            .addExtras("conversationIdentifier", conversationIdentifier)
            .addExtras("getLastMessageRequestTag",tag);

    const bool sent = sendRequest(data);
    if(sent){
        magabeClientManager_->getLastMessageCallbacks().registerCallback(tag,cb,opaque);
    }else{
        cb(MOpaque<>::createShared(Message(),Errors::OnGetLastMessageError::UnknownError),opaque);
    }
}

void MagabeClientApp::getLastMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getLastMessage(conversation,MessageType::SentOrReceived,mqmlcb);
}

void MagabeClientApp::getLastSentMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getLastMessage(conversation,MessageType::Sent,mqmlcb);
}

void MagabeClientApp::getLastReceivedMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getLastMessage(conversation,MessageType::Received,mqmlcb);
}

void MagabeClientApp::getLastReadMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getLastMessage(conversation,MessageType::Read,mqmlcb);
}

void MagabeClientApp::getLastUnReadMessage(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getLastMessage(conversation,MessageType::UnRead,mqmlcb);
}

void MagabeClientApp::getMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getMessageCount(conversation,MessageType::SentOrReceived,mqmlcb);
}

void MagabeClientApp::getSentMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getMessageCount(conversation,MessageType::Sent,mqmlcb);
}

void MagabeClientApp::getReceivedMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getMessageCount(conversation,MessageType::Received,mqmlcb);
}

void MagabeClientApp::getReadMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getMessageCount(conversation,MessageType::Read,mqmlcb);

}

void MagabeClientApp::getUnReadMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)
{
    getMessageCount(conversation,MessageType::UnRead,mqmlcb);

}



void MagabeClientApp::getMessageCount(const QVariantMap &conversation,MessageType type, MQmlCallback *mqmlcb)
{
    if(!mqmlcb){
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null, can not continue").arg("MagabeClientApp::getMessageCount(const QVariantMap &conversation, MQmlCallback *mqmlcb)");
        return;
    }

    if(!isConnected()){
        if(mqmlcb)
            mqmlcb->call(1,(int)Errors::OnGetMessageCountError::ConnectionErrors);
        return;
    }

    const QString conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();
    getMessageCount(Conversation(conversationIdentifier),[](std::shared_ptr<MOpaque<int,Errors::OnGetMessageCountError>> cbSharedMOpaque,void *opaque){
        int messageCount = std::get<0>(cbSharedMOpaque->tuple());
        Errors::OnGetMessageCountError error = std::get<1>(cbSharedMOpaque->tuple());

        auto mqmlcb =  reinterpret_cast<MQmlCallback*>(opaque);
        if(mqmlcb)
            mqmlcb->call(messageCount,(int)error);
    },type,mqmlcb);

}

void MagabeClientApp::getLastMessage(const QVariantMap &conversation, MessageType type, MQmlCallback *mqmlcb)
{
    if(!mqmlcb){
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null, can not continue").arg("MagabeClientApp::getLastMessage");
        return;
    }

    if(!isConnected()){
        if(mqmlcb)
            mqmlcb->call(QString("{}"),(int)Errors::OnGetLastMessageError::ConnectionErrors);
        return;
    }

    const QString& conversationIdentifier =  conversation.value("conversationIdentifier",QString("")).toString();

    getLastMessage(Conversation(conversationIdentifier)
                   ,[](std::shared_ptr<MOpaque<Message,Errors::OnGetLastMessageError>> cbSharedMOpaque,void *opaque){

        auto message  = std::get<0>(cbSharedMOpaque->tuple());
        auto error  = std::get<1>(cbSharedMOpaque->tuple());
        auto mqmlcb =  reinterpret_cast<MQmlCallback*>(opaque);
        if(mqmlcb) mqmlcb->call(message.toJson(),(int)error);

    },type,mqmlcb);
}

void MagabeClientApp::uploadProfilePicture(const QString &imagePath
                                           ,ProfilePictureUploadedCallback profilePictureUploadedCallback
                                           ,ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback
                                           ,ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback
                                           ,void *opaque){

    auto path = FileUtils::generateFilePath(AuthenticationController::getProfilePicturesFolder(),FileUtils::getFileName(imagePath));
    const bool copyed =  FileUtils::copyFile(imagePath,path);
    if(!copyed){
        path = imagePath;
    }

    QString tag;
createTag:
    tag = QString("%1:%2")
            .arg(imagePath)
            .arg(QDateTime::currentMSecsSinceEpoch());

    if(magabeClientManager_->profilePictureUploadCallbacks().containsTag(tag)){
        goto  createTag;
    }

    magabeClientManager_->profilePictureUploadCallbacks().registerCallback(tag,imagePath
                                                                           ,profilePictureUploadedCallback
                                                                           ,profilePictureUploadingFailedCallback
                                                                           ,profilePictureUploadProgressCallback,opaque);
    Extras extras;
    extras.addExtras("imagePath",path);
    extras.addExtras("profilePictureTag",tag);
    triggerControllerAction(TO_CSTR(AuthenticationController),"ActionStartProfilePictureUploading",extras);
}


void MagabeClientApp::uploadProfilePicture(const QString &imagePath
                                           , MQmlCallback *profilePictureUploadedCallback
                                           , MQmlCallback *profilePictureUploadingFailedCallback
                                           , MQmlCallback *profilePictureUploadProgressCallback)
{

    typedef  MOpaque<MQmlCallback*,MQmlCallback*,MQmlCallback*> UploadProfilePictureOpaqueType;//remember to change,depend on stored types
    auto uploadProfilePictureOpaque =  MOpaque<>::createUnique(profilePictureUploadedCallback
                                                               ,profilePictureUploadingFailedCallback
                                                               ,profilePictureUploadProgressCallback);




    uploadProfilePicture(imagePath
                         ,[](std::shared_ptr<MOpaque<QString, QString>> cbSharedMOpaque, void *opaque){
        const QString& fileName = std::get<0>(cbSharedMOpaque->tuple());
        const QString& filePath = std::get<1>(cbSharedMOpaque->tuple());

        auto uploadProfilePictureOpaque =  reinterpret_cast<UploadProfilePictureOpaqueType*>(opaque);
        if(!uploadProfilePictureOpaque){
            qWarning() << "MagabeClientApp::uploadProfilePicture | Error, uploadProfilePictureOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = uploadProfilePictureOpaque->tuple();
        auto profilePictureUploadedCallback = std::get<0>(tuple);

        MOpaque<>::destroy(uploadProfilePictureOpaque);//after getting the value, destroy, very important

        Extras extras;
        extras.addExtras("fileName",fileName)
                .addExtras("filePath",filePath);
        if(profilePictureUploadedCallback){
            profilePictureUploadedCallback->call(extras.getJsonExtras(),0);
        }
    }
    ,[](std::shared_ptr<MOpaque<QString,QString,QString>> cbSharedMOpaque, void *opaque){
        const QString& fileName = std::get<0>(cbSharedMOpaque->tuple());
        const QString& filePath = std::get<1>(cbSharedMOpaque->tuple());
        const QString& errorString = std::get<2>(cbSharedMOpaque->tuple());

        auto uploadProfilePictureOpaque =  reinterpret_cast<UploadProfilePictureOpaqueType*>(opaque);
        if(!uploadProfilePictureOpaque){
            qWarning() << "MagabeClientApp::uploadProfilePicture | Error, uploadProfilePictureOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = uploadProfilePictureOpaque->tuple();
        auto profilePictureUploadingFailedCallback = std::get<1>(tuple);

        MOpaque<>::destroy(uploadProfilePictureOpaque);//after getting the value, destroy, very important

        Extras extras;
        extras.addExtras("fileName",fileName)
                .addExtras("errorString",errorString)
                .addExtras("filePath",filePath);
        if(profilePictureUploadingFailedCallback){
            profilePictureUploadingFailedCallback->call(extras.getJsonExtras(),10);
        }
    }
    ,[](std::shared_ptr<MOpaque<QString,float,quint64>> cbSharedMOpaque, void *opaque){
        auto uploadProfilePictureOpaque =  reinterpret_cast<UploadProfilePictureOpaqueType*>(opaque);
        if(!uploadProfilePictureOpaque){
            qWarning() << "MagabeClientApp::uploadProfilePicture | Error, uploadProfilePictureOpaque is null, this is not supporsed to happen";
            return;
        }

        auto cbSharedTuple = cbSharedMOpaque->tuple();
        QString filePath = std::get<0>(cbSharedTuple);
        float  percentageCompletion = std::get<1>(cbSharedTuple);
        quint64 fileSize = std::get<2>(cbSharedTuple);

        auto tuple = uploadProfilePictureOpaque->tuple();
        auto profilePictureUploadProgressCallback = std::get<2>(tuple);
        //NOTE : do not delete uploadProfilePictureOpaque in here

        Extras extras;
        extras.addExtras("percentageCompletion",percentageCompletion)
                .addExtras("filePath",filePath)
                .addExtras("fileSize",fileSize);
        if(profilePictureUploadProgressCallback){
            profilePictureUploadProgressCallback->call(extras.getJsonExtras(),0);
        }
    }
    ,uploadProfilePictureOpaque);
}

void MagabeClientApp::uploadScaledProfilePicture(QString imagePath
                                                 , int quality, int width, int height
                                                 , Qt::AspectRatioMode aspectMode
                                                 ,ProfilePictureUploadedCallback profilePictureUploadedCallback
                                                 ,ProfilePictureUploadingFailedCallback profilePictureUploadingFailedCallback
                                                 ,ProfilePictureUploadProgressCallback profilePictureUploadProgressCallback
                                                 ,void *opaque){
    bool saved = false;
    auto path  = FileUtils::scaleImageAndSaveToFolder(imagePath,AuthenticationController::getProfilePicturesFolder(),FileUtils::getFileName(imagePath)
                                                      ,quality,width,height,aspectMode,&saved);
    if(!saved){
        imagePath = path;
    }

    uploadProfilePicture(imagePath,profilePictureUploadedCallback
                         ,profilePictureUploadingFailedCallback
                         ,profilePictureUploadProgressCallback
                         ,opaque);
}

void MagabeClientApp::uploadScaledProfilePicture(QString imagePath, int quality, int width, int height, Qt::AspectRatioMode aspectMode
                                                 , MQmlCallback *profilePictureUploadedCallback, MQmlCallback *profilePictureUploadingFailedCallback, MQmlCallback *profilePictureUploadProgressCallback)
{

    bool saved = false;
    auto path  = FileUtils::scaleImageAndSaveToFolder(imagePath,AuthenticationController::getProfilePicturesFolder(),FileUtils::getFileName(imagePath)
                                                      ,quality,width,height,aspectMode,&saved);
    if(!saved){
        imagePath = path;
    }

    uploadProfilePicture(imagePath
                         ,profilePictureUploadedCallback
                         ,profilePictureUploadingFailedCallback
                         ,profilePictureUploadProgressCallback);
}

bool MagabeClientApp::isConnected()
{
    auto cRef = CONTROLLER_IDENTIFIER_REF;
    if(cRef){
        return cRef->isSocketConnected();
    }else{
        return false;
    }
}

void MagabeClientApp::connectToServer(const QString &ipAddress, int port, int msecsTimeout)
{
    emit magabeClientManager_->connectToServer(ipAddress,QString::number(port),msecsTimeout);
}

void MagabeClientApp::disconnectFromServer()
{
    emit magabeClientManager_->disconnectFromServer();
}

void MagabeClientApp::getUserInfo(const QString& username,GetUserInfoCallback cb, void *opaque)
{
    UserInfo info;
    info.setUsername(username);
    if((!username.isEmpty()) && cb){
        if(isConnected()){

            MagabeData data;
            data.setController("UserController")
                    .setAction("ActionGetUserInfo")
                    .addExtras("username",username);

            const bool sent = sendRequest(data);
            if(sent){
                magabeClientManager_->userInfoCallbacks().registerCallback(username,cb,opaque);
            }else{
                cb(MOpaque<>::createShared(username,info,Errors::UserInfoError::UnknownError),opaque);
            }
        }else{
            cb(MOpaque<>::createShared(username,info,Errors::UserInfoError::ConnectionErrors),opaque);
        }
    }

    if((!isConnected()) && cb){
        cb(MOpaque<>::createShared(username,info,Errors::UserInfoError::ConnectionErrors),opaque);
        return;
    }
}

void MagabeClientApp::getUserInfo(const QString &username, MQmlCallback *mqmlcb)
{
    if(mqmlcb){
        getUserInfo(username,[](std::shared_ptr<MOpaque<QString,UserInfo,Errors::UserInfoError>> cbSharedMOpaque,void *opaque){
            const QString& username = std::get<0>(cbSharedMOpaque->tuple());
            const UserInfo& info = std::get<1>(cbSharedMOpaque->tuple());
            const Errors::UserInfoError& error =  std::get<2>(cbSharedMOpaque->tuple());

            MQmlCallback *cb = reinterpret_cast<MQmlCallback *>(opaque);

            Extras extras; auto jsonInfo = const_cast<UserInfo&>(info).toJson();
            extras.addExtras("username",username)
                    .addExtras("userInfo",jsonInfo);
            if(cb){//call imediately after a  null check
                cb->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) already destroyed").arg("MagabeClientApp::getUserInfo(const QString &username, MQmlCallback *mqmlcb)");
            }
        },(void*)mqmlcb);
    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null ,can not continue").arg("MagabeClientApp::getUserInfo(const QString &username, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::getUserInfo(unsigned long userId, GetUserInfoCallback cb
                                  , void *opaque)
{
    if(!cb){
        return;
    }

    if(!isConnected()){
        cb(MOpaque<>::createShared(QString(""),UserInfo(),Errors::UserInfoError::ConnectionErrors),opaque);
        return;
    }

    if(userId == 0){
        cb(MOpaque<>::createShared(QString(""),UserInfo(),Errors::UserInfoError::UserNotFound),opaque);
        return;
    }

    QList<unsigned long> userIds;
    userIds << userId;

    typedef  MOpaque<MagabeClientApp*,GetUserInfoCallback,unsigned long,void*> FetchUserNamesOpaqueType;
    auto fetchUserNamesOpaque =  MOpaque<>::createUnique(this,cb,userId,opaque);

    fetchUserNames(userIds,[](std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque){
        QMap<unsigned long,QString> map = std::get<0>(cbSharedMOpaque->tuple());

        auto fetchUserNamesOpaque =  reinterpret_cast<FetchUserNamesOpaqueType*>(opaque);
        if(!fetchUserNamesOpaque){
            qWarning() << "MagabeClientApp::sendMessage | Error, fetchUserNamesOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = fetchUserNamesOpaque->tuple();
        const auto magabeClientApp = std::get<0>(tuple);
        auto getUserInfoCallback = std::get<1>(tuple);
        auto userId = std::get<2>(tuple);
        auto getUserInfoOpaque = std::get<3>(tuple);

        MOpaque<>::destroy(fetchUserNamesOpaque);//after getting the value, destroy, very important

        auto keys = map.keys();
        if(keys.length() > 0){
            auto receivedId = keys.at(0);
            auto username  = map.value(receivedId);
            if(!username.isEmpty()){
                if(receivedId != userId){
                    qWarning()<< "MagabeClientApp::getUserInfo | Error. received a name with defient userId,Server side implimentation error";
                    getUserInfoCallback(MOpaque<>::createShared(QString(""),UserInfo(),Errors::UserInfoError::UnknownError),getUserInfoOpaque);
                }else{
                    magabeClientApp->getUserInfo(username,getUserInfoCallback,getUserInfoOpaque);
                }
            }else{
                getUserInfoCallback(MOpaque<>::createShared(QString(""),UserInfo(),Errors::UserInfoError::UserNotFound),getUserInfoOpaque);
            }
        }else{
            getUserInfoCallback(MOpaque<>::createShared(QString(""),UserInfo(),Errors::UserInfoError::UserNotFound),getUserInfoOpaque);
        }
    },fetchUserNamesOpaque);
}

void MagabeClientApp::getUserInfo(unsigned long userId, MQmlCallback *mqmlcb)
{
    if(mqmlcb){
        getUserInfo(userId,[](std::shared_ptr<MOpaque<QString,UserInfo,Errors::UserInfoError>> cbSharedMOpaque,void *opaque){
            const QString& username = std::get<0>(cbSharedMOpaque->tuple());
            const UserInfo& info = std::get<1>(cbSharedMOpaque->tuple());
            const Errors::UserInfoError& error =  std::get<2>(cbSharedMOpaque->tuple());

            MQmlCallback *cb = reinterpret_cast<MQmlCallback *>(opaque);

            Extras extras; auto jsonInfo = const_cast<UserInfo&>(info).toJson();
            extras.addExtras("username",username)
                    .addExtras("userInfo",jsonInfo);
            if(cb){//call imediately after null check
                cb->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) already destroyed").arg("MagabeClientApp::getUserInfo(unsigned long userId, MQmlCallback *mqmlcb)");
            }
        },(void*)mqmlcb);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null, can not continue").arg("MagabeClientApp::getUserInfo(unsigned long userId, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::downloadProfilePicture(const QString& username, DownloadProfilePictureCallback downloadProfilePictureCallback,ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque){
    if((!username.isEmpty()) && downloadProfilePictureCallback){
        if(isConnected()){
            magabeClientManager_->profilePictureDownloadCallbacks().registerCallback(username,downloadProfilePictureCallback,profilePictureDownloadProgressCallback,opaque);
            Extras extras;
            extras.addExtras("username",username);
            triggerControllerAction(TO_CSTR(AuthenticationController),"ActionStartProfilePictureDownloading",extras);
        }else{
            downloadProfilePictureCallback(MOpaque<>::createShared(username,QString(""),Errors::ProfilePictureDownloadError::ConnectionErrors),opaque);
        }
    }

    if((!isConnected()) && downloadProfilePictureCallback){
        downloadProfilePictureCallback(MOpaque<>::createShared(username,QString(""),Errors::ProfilePictureDownloadError::ConnectionErrors),opaque);
        return;
    }
}

void MagabeClientApp::downloadProfilePicture(const QString &username, MQmlCallback *downloadProfilePictureCallback,MQmlCallback* profilePictureDownloadProgressCallback)
{
    if(downloadProfilePictureCallback){
        auto callbacks = MOpaque<>::createUnique(downloadProfilePictureCallback,profilePictureDownloadProgressCallback);

        downloadProfilePicture(username
                               ,[](std::shared_ptr<MOpaque<QString,QString,Errors::ProfilePictureDownloadError>> cbSharedMOpaque,void *opaque){

            const QString& username = std::get<0>(cbSharedMOpaque->tuple());
            const QString& profilePicturePath = std::get<1>(cbSharedMOpaque->tuple());
            const Errors::ProfilePictureDownloadError error  = std::get<2>(cbSharedMOpaque->tuple());

            auto callbacks = reinterpret_cast<MOpaque<MQmlCallback *,MQmlCallback *>*>(opaque);
            if(!callbacks){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, callbacks is null, this is not supporsed to happen";
                return;
            }

            MQmlCallback *downloadProfilePictureCallback = std::get<0>(callbacks->tuple());

            MOpaque<>::destroy(callbacks);//destroy mopaque

            Extras extras;
            extras.addExtras("username",username)
                    .addExtras("profilePicturePath",profilePicturePath);
            if(downloadProfilePictureCallback){//null check,and call imediately,the object can be deleted any time by the timer
                downloadProfilePictureCallback->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *downloadProfilePictureCallback) already destroyed").arg("MagabeClientApp::downloadProfilePicture");
            }
        }
        ,[](std::shared_ptr<MOpaque<QString,float,quint64>> cbSharedMOpaque, void *opaque){

            auto callbacks = reinterpret_cast<MOpaque<MQmlCallback *,MQmlCallback *>*>(opaque);
            if(!callbacks){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, callbacks is null, this is not supporsed to happen";
                return;
            }

            MQmlCallback *profilePictureDownloadProgressCallback = std::get<1>(callbacks->tuple());
            if(!profilePictureDownloadProgressCallback){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, profilePictureDownloadProgressCallback is null, this is not supporsed to happen";
                return;
            }

            //note do not delete mopaque ->callbcks in here/progress cb

            auto fileName = std::get<0>(cbSharedMOpaque->tuple());
            auto percentageCompletion = std::get<1>(cbSharedMOpaque->tuple());
            auto fileSize = std::get<2>(cbSharedMOpaque->tuple());

            Extras extras;
            extras.addExtras("fileName",fileName)
                    .addExtras("percentageCompletion",percentageCompletion)
                    .addExtras("fileSize",fileSize);
            if(profilePictureDownloadProgressCallback){//null check,and call imediately,the object can be deleted any time by the timer
                profilePictureDownloadProgressCallback->call(QVariant(extras.getJsonExtras().data()),0);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *profilePictureDownloadProgressCallback) already destroyed").arg("MagabeClientApp::downloadProfilePicture");
            }

        }
        ,callbacks);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null ,can not continue").arg("MagabeClientApp::downloadProfilePicture(const QString &username, MQmlCallback *mqmlcb)");
    }
}

void MagabeClientApp::downloadProfilePicture(unsigned long userId, DownloadProfilePictureCallback downloadProfilePictureCallback, ProfilePictureDownloadProgressCallback profilePictureDownloadProgressCallback, void *opaque)
{
    if(!downloadProfilePictureCallback){
        return;
    }

    if(!isConnected()){
        downloadProfilePictureCallback(MOpaque<>::createShared(QString(""),QString(""),Errors::ProfilePictureDownloadError::ConnectionErrors),opaque);
        return;
    }

    if(userId == 0){
        downloadProfilePictureCallback(MOpaque<>::createShared(QString(""),QString(""),Errors::ProfilePictureDownloadError::UserNotFound),opaque);
        return;
    }

    QList<unsigned long> userIds;
    userIds << userId;

    typedef  MOpaque<MagabeClientApp*,DownloadProfilePictureCallback,ProfilePictureDownloadProgressCallback,unsigned long,void*> FetchUserNamesOpaqueType;
    auto fetchUserNamesOpaque =  MOpaque<>::createUnique(this,downloadProfilePictureCallback,profilePictureDownloadProgressCallback,userId,opaque);

    fetchUserNames(userIds,[](std::shared_ptr<MOpaque<QMap<unsigned long,QString>>> cbSharedMOpaque,void *opaque){
        QMap<unsigned long,QString> map = std::get<0>(cbSharedMOpaque->tuple());

        auto fetchUserNamesOpaque =  reinterpret_cast<FetchUserNamesOpaqueType*>(opaque);
        if(!fetchUserNamesOpaque){
            qWarning() << "MagabeClientApp::downloadProfilePicture | Error, fetchUserNamesOpaque is null, this is not supporsed to happen";
            return;
        }

        auto tuple = fetchUserNamesOpaque->tuple();
        const auto magabeClientApp = std::get<0>(tuple);
        auto downloadProfilePictureCallback = std::get<1>(tuple);
        auto profilePictureDownloadProgressCallback = std::get<2>(tuple);
        auto userId = std::get<3>(tuple);
        auto downloadProfilePictureOpaque = std::get<4>(tuple);

        MOpaque<>::destroy(fetchUserNamesOpaque);//after getting the value, destroy, very important

        auto keys = map.keys();
        if(keys.length() > 0){
            auto receivedId = keys.at(0);
            auto username  = map.value(receivedId);
            if(!username.isEmpty()){
                if(receivedId != userId){
                    qWarning()<< "MagabeClientApp::downloadProfilePicture | Error. received a name with defient userId,Server side implimentation error";
                    downloadProfilePictureCallback(MOpaque<>::createShared(QString(""),QString(""),Errors::ProfilePictureDownloadError::UnknownError),downloadProfilePictureOpaque);
                }else{
                    magabeClientApp->downloadProfilePicture(username,downloadProfilePictureCallback,profilePictureDownloadProgressCallback,downloadProfilePictureOpaque);
                }
            }else{
                downloadProfilePictureCallback(MOpaque<>::createShared(QString(""),QString(""),Errors::ProfilePictureDownloadError::UserNotFound),downloadProfilePictureOpaque);
            }
        }else{
            downloadProfilePictureCallback(MOpaque<>::createShared(QString(""),QString(""),Errors::ProfilePictureDownloadError::UserNotFound),downloadProfilePictureOpaque);
        }

    },fetchUserNamesOpaque);
}

void MagabeClientApp::downloadProfilePicture(unsigned long userId, MQmlCallback *downloadProfilePictureCallback, MQmlCallback* profilePictureDownloadProgressCallback)
{
    if(downloadProfilePictureCallback){
        auto callbacks = MOpaque<>::createUnique(downloadProfilePictureCallback,profilePictureDownloadProgressCallback);

        downloadProfilePicture(userId,
                               [](std::shared_ptr<MOpaque<QString,QString,Errors::ProfilePictureDownloadError>> cbSharedMOpaque,void *opaque){

            const QString& username = std::get<0>(cbSharedMOpaque->tuple());
            const QString& profilePicturePath = std::get<1>(cbSharedMOpaque->tuple());
            const Errors::ProfilePictureDownloadError error  = std::get<2>(cbSharedMOpaque->tuple());

            auto callbacks = reinterpret_cast<MOpaque<MQmlCallback *,MQmlCallback *>*>(opaque);
            if(!callbacks){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, callbacks is null, this is not supporsed to happen";
                return;
            }

            MQmlCallback *downloadProfilePictureCallback = std::get<0>(callbacks->tuple());

            MOpaque<>::destroy(callbacks);//destroy mopaque

            Extras extras;
            extras.addExtras("username",username)
                    .addExtras("profilePicturePath",profilePicturePath);
            if(downloadProfilePictureCallback){//null check,and call imediately,the object can be deleted any time by the timer
                downloadProfilePictureCallback->call(QVariant(extras.getJsonExtras().data()),(int)error);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *downloadProfilePictureCallback) already destroyed").arg("MagabeClientApp::downloadProfilePicture");
            }
        }
        ,[=](std::shared_ptr<MOpaque<QString,float,quint64>> cbSharedMOpaque, void *opaque){
            auto callbacks = reinterpret_cast<MOpaque<MQmlCallback *,MQmlCallback *>*>(opaque);
            if(!callbacks){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, callbacks is null, this is not supporsed to happen";
                return;
            }

            MQmlCallback *profilePictureDownloadProgressCallback = std::get<1>(callbacks->tuple());
            if(!profilePictureDownloadProgressCallback){
                qWarning() << "MagabeClientApp::downloadProfilePicture | Error, profilePictureDownloadProgressCallback is null, this is not supporsed to happen";
                return;
            }
            //note do not delete mopaque ->callbcks in here/progress cb

            auto fileName = std::get<0>(cbSharedMOpaque->tuple());
            auto percentageCompletion = std::get<1>(cbSharedMOpaque->tuple());
            auto fileSize = std::get<2>(cbSharedMOpaque->tuple());

            Extras extras;
            extras.addExtras("fileName",fileName)
                    .addExtras("percentageCompletion",percentageCompletion)
                    .addExtras("fileSize",fileSize);
            if(profilePictureDownloadProgressCallback){//null check,and call imediately,the object can be deleted any time by the timer
                profilePictureDownloadProgressCallback->call(QVariant(extras.getJsonExtras().data()),(int)0);
            }else{
                qWarning() << QString("%1 | Error (MQmlCallback *downloadProfilePictureCallback) already destroyed").arg("MagabeClientApp::downloadProfilePicture(const QString &username, MQmlCallback *mqmlcb)");
            }
        },callbacks);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::downloadProfilePicture(unsigned long userId, MQmlCallback *mqmlcb)");
    }
}


bool MagabeClientApp::sendRequest(MagabeData &data)
{
    if(!isConnected()){
        qWarning() << "MagabeClientApp::sendRequest |   Error , Failed to send, Not connected ..!";
        return false;
    }
    bool invocked =  QMetaObject::invokeMethod(DWritter::instance()
                                               ,"sendData"
                                               ,Q_ARG(QString,data.toByteArray()));
    if(!invocked){
        qWarning() << "Authentication::sendRequest :: Critical Error , Invocation Failed" ;
    }
    return invocked;
}

bool MagabeClientApp::triggerControllerAction(const QString &controller
                                              , const QString &action, Extras& extras)
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

MagabeData MagabeClientApp::constructGetMessageRequest(int methodType, const Message &start, int maxNoOfMessages
                                                       , bool includeStartMessage, const Sort::Order sortOrder)
{
    const QString strOrder = (sortOrder == Sort::Order::ASC ? "ASC" : "DESC");
    const auto messageIdentifier = start.messageIdentifier();
    MagabeData data;
    QString tag;
createTag:
    tag = QString("%1:%2:%3:%4:%5:%6")
            .arg(QString("%1[%2]").arg(toMessageTypeHuman(methodType)).arg(methodType))
            .arg( messageIdentifier).arg(maxNoOfMessages)
            .arg(includeStartMessage).arg(strOrder)
            .arg(QDateTime::currentMSecsSinceEpoch());

    if(magabeClientManager_->abstractGetMessagesCallbacks().containsTag(tag)){
        goto  createTag;
    }

    data.setController("ConversationController")
            .setAction("ActionGetMessages")
            .addExtras("MessageType",methodType)
            .addExtras("messageIdentifier", messageIdentifier)
            .addExtras("maxNoOfMessages",maxNoOfMessages)
            .addExtras("includeStartMessage",includeStartMessage)
            .addExtras("sortOrder",strOrder)
            .addExtras("getMessageRequestTag",tag);

    return data;
}

MagabeData MagabeClientApp::constructGetMessageRequest(int methodType, const Conversation &conversation
                                                       , int maxNoOfMessages, const Sort::Order sortOrder)
{
    const QString strOrder = (sortOrder == Sort::Order::ASC ? "ASC" : "DESC");
    const auto conversationIdentifier = const_cast<Conversation &>(conversation).conversationIdentifier();
    MagabeData data;
    QString tag;
createTag:
    tag = QString("%1:%2:%3:%4:%5:%6")
            .arg(QString("%1[%2]").arg(toMessageTypeHuman(methodType)).arg(methodType))
            .arg(conversationIdentifier).arg(maxNoOfMessages)
            .arg(true).arg(strOrder)
            .arg(QDateTime::currentMSecsSinceEpoch());

    if(magabeClientManager_->abstractGetMessagesCallbacks().containsTag(tag)){
        goto  createTag;
    }

    data.setController("ConversationController")
            .setAction("ActionGetMessages")
            .addExtras("MessageType",methodType)
            .addExtras("conversationIdentifier",conversationIdentifier)
            .addExtras("maxNoOfMessages",maxNoOfMessages)
            .addExtras("includeStartMessage",true)
            .addExtras("sortOrder",strOrder)
            .addExtras("getMessageRequestTag",tag);

    return data;
}

QString MagabeClientApp::getGetMessageRequestTag(MagabeData &data)
{
    return data.getStringExtras("getMessageRequestTag");
}


QString MagabeClientApp::toMessageTypeHuman(int messageType){
    switch ((MessageType)messageType) {
    case MessageType::Sent: return "GetSentMessages";
    case MessageType::Received: return "GetReceivedMessages";
    case MessageType::SentOrReceived: return "GetMessages";
    case MessageType::Read: return "GetReadMessages";
    case MessageType::UnRead: return "GetUnReadMessages";
    default: return QString("");
    }
}


QString MagabeClientApp::getProfilePicturePath(const QString& userName){
    UserProfilePictureModel model;
    return model.getPath(userName);
}

void MagabeClientApp::isUserProfilePictureChanged(unsigned long userId,const QString& imagePath, IsUserProfilePictureChangedCallback cb, void *opaque)
{
    if(!cb){
        return;
    }

    if((userId == 0) || imagePath.isEmpty()){
        cb(false,opaque);
        return;
    }

    const QString& imgName = FileUtils::getFileName(imagePath);

    if(isConnected()){
        MagabeData data;

        QString tag;
createTag:
        tag = QString("%1:%2:%3:%4").arg(data.getDataId()).arg(imgName)
                .arg(QDateTime::currentMSecsSinceEpoch()).arg(((unsigned long long)cb));

        if(magabeClientManager_->isUserProfilePictureChangedCallbacks().containsTag(tag)){
            goto createTag;
        }

        data.setController("UserController")
                .setAction("ActionIsUserProfilePictureChanged")
                .addExtras("imgName",imgName)
                .addExtras("userId",userId)
                .addExtras("isUserProfilePictureChangedRequestTag",tag);

        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->isUserProfilePictureChangedCallbacks().registerCallback(tag,cb,opaque);
        }else{
            cb(false,opaque);
        }
    }else{
        cb(false,opaque);
    }
}

void MagabeClientApp::isUserProfilePictureChanged(unsigned long userId,const QString &imagePath, MQmlCallback *mqmlcb)
{
    if(!mqmlcb){
        return;
    }

    if((userId == 0) || imagePath.isEmpty()){
        if(mqmlcb)
            mqmlcb->call("{\"changed\" : \"0\"}",0);
        return;
    }
    isUserProfilePictureChanged(userId,imagePath,[](std::shared_ptr<MOpaque<bool>> cbSharedMOpaque,void *opaque){
        bool changed = std::get<0>(cbSharedMOpaque->tuple());

        MQmlCallback * mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);
        if(changed){
            if(mqmlcb)
                mqmlcb->call("{\"changed\" : \"1\"}",0);
        }else{
            if(mqmlcb)
                mqmlcb->call("{\"changed\" : \"0\"}",0);
        }

    },mqmlcb);
}

void MagabeClientApp::deleteMassage(const QString &messageIdentifier, DeleteMessageCallback cb, void *opaque)
{
    if(!cb){
        return;
    }

    if(messageIdentifier.isEmpty()){
        cb(false,opaque);
        return;
    }


    if(isConnected()){
        QString tag;
createTag:
        tag = QString("%1:%2:%3").arg(messageIdentifier)
                .arg(QDateTime::currentMSecsSinceEpoch()).arg(((unsigned long long)cb));

        if(magabeClientManager_->deleteMessageCallbacks().containsTag(tag)){
            goto createTag;
        }

        MagabeData data;
        data.setController("ConversationController")
                .setAction("ActionDeleteMessage")
                .addExtras("messageIdentifier",messageIdentifier)
                .addExtras("deleteMessageRequestTag",tag);

        const bool sent = sendRequest(data);
        if(sent){
            magabeClientManager_->deleteMessageCallbacks().registerCallback(tag,cb,opaque);
        }else{
            cb(false,opaque);
        }
    }else{
        cb(false,opaque);
    }
}

void MagabeClientApp::deleteMassage(const QVariantMap &startMessage, MQmlCallback *mqmlcb){
    if(mqmlcb){

        const QString messageIdentifier =  startMessage.value("messageIdentifier",QString("")).toString();
        if(!isValidMessageIdentifier(messageIdentifier)){
            if(mqmlcb)
                mqmlcb->call("{\"deleted\" : \"0\"}",0);
            return;
        }

        deleteMassage(messageIdentifier,[](std::shared_ptr<MOpaque<bool>> cbSharedMOpaque,void *opaque){
            bool deleted = std::get<0>(cbSharedMOpaque->tuple());

            MQmlCallback * mqmlcb = reinterpret_cast<MQmlCallback *>(opaque);

            if(deleted){
                if(mqmlcb)
                    mqmlcb->call("{\"deleted\" : \"1\"}",0);
            }else{
                if(mqmlcb)
                    mqmlcb->call("{\"deleted\" : \"0\"}",0);
            }

        },mqmlcb);

    }else{
        qWarning() << QString("%1 | Error (MQmlCallback *mqmlcb) is null can not continue").arg("MagabeClientApp::deleteMassage");
    }
}


QString MagabeClientApp::getProfilePicturePath(unsigned long userId){
    UserProfilePictureModel model;
    return model.getPath(userId);
}


/*--------------------------------TESTS-------------------------------------------*/
#ifdef MAGABE_TESTS





#endif

/*--------------------------------TESTS-------------------------------------------*/
