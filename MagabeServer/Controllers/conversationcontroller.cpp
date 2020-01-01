/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "conversationcontroller.h"
#include "Helpers/messagetype.h"

ConversationController::ConversationController(QObject *parent) : Controller(parent)
{
    //first thing to do
    CONTROLLER_SET_NAME(ConversationController)
}

Controller *ConversationController::createNew()
{
    return new ConversationController(nullptr);
}

void ConversationController::onActionGetMessagesTriggered(Extras extras){

    auto response = user().response();
    response.setController(TO_CSTR(ConversationController));

    QString messageIdentifier = extras.getStringExtras(ExtrasNames::messageIdentifierExtras);
    const int maxNoOfMessages = extras.getIntExtras(ExtrasNames::maxNoOfMessagesExtras);
    bool includeStartMessage = extras.getBooleanExtras(ExtrasNames::includeStartMessageExtras);
    const Sort::Order sortOrder = (extras.getStringExtras(ExtrasNames::sortOrderExtras) == "ASC") ? Sort::Order::ASC : Sort::Order::DESC;
    const QString& conversationIdentifier = extras.getStringExtras(ExtrasNames::conversationIdentifierExtras);
    const MessageType messageType = (MessageType)extras.getIntExtras(ExtrasNames::MessageTypeExtras);
    const QString& requestTag = extras.getStringExtras(ExtrasNames::getMessageRequestTagExtras);

    const auto userId = user().id();
    //CHECK 1
    if((!messageIdentifier.isEmpty())
            && (!Message::isValidMessageIdentifier(messageIdentifier))){
        response.setAction(GetMessageErrorActionName)
                .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::InvalidMessageIdentifier)
                .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                .send();return;
    }

    if((!conversationIdentifier.isEmpty())
            && (!Conversation::isValidConversationIdentifier(conversationIdentifier))){
        response.setAction(GetMessageErrorActionName)
                .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::InvalidConversationIdentifier)
                .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                .send();return;
    }

    if(!messageIdentifier.isEmpty()){
        if(!messagesModel_.conversationExist(messagesModel_.generateConversationIdentifier(messageIdentifier))){
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::ConversationNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();return;
        }
    }

    if(!conversationIdentifier.isEmpty()){
        if(!messagesModel_.conversationExist(conversationIdentifier)){
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::ConversationNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();return;
        }

    }
    //CHECK 2 - if message doesnt fit the profile
    const bool messageRecordExists = (messagesModel_.messageExists(messageIdentifier));
    if(messageRecordExists){
        Message message = messagesModel_.getMessage(messageIdentifier);
        QString next;
        switch (messageType) {

        case  MessageType::Sent:{
            if(message.sender() != userId
                    || ((message.sender() == userId) && message.deletedBySender())){
                next = messagesModel_.getNextSentMessage(messageIdentifier,userId).messageIdentifier();
            }
            break;
        }
        case  MessageType::Received:{
            if((message.receiver() != userId)
                    || ((message.receiver() == userId) && message.deletedByReceiver())){
                next = messagesModel_.getNextReceivedMessage(messageIdentifier,userId).messageIdentifier();
            }
            break;
        }
        case  MessageType::SentOrReceived:{
            if(message.sender() == userId){
                if(message.deletedBySender()){
                    next = messagesModel_.getNextMessage(messageIdentifier,userId).messageIdentifier();
                }
            }else{
                if(message.deletedByReceiver()){
                    next = messagesModel_.getNextMessage(messageIdentifier,userId).messageIdentifier();
                }
            }
            break;
        }
        case  MessageType::Read:{
            if((message.receiver() != userId)
                    || (!message.readByReceiver())
                    || ((message.receiver() == userId) && message.readByReceiver() && message.deletedByReceiver())){
                next = messagesModel_.getNextReadMessage(messageIdentifier,userId).messageIdentifier();
            }
            break;
        }
        case  MessageType::UnRead:{
            if((message.receiver() != userId)
                    || (message.readByReceiver())
                    || ((message.receiver() == userId) && (!message.readByReceiver()) && message.deletedByReceiver())){
                next = messagesModel_.getNextUnReadMessage(messageIdentifier,userId).messageIdentifier();
            }
            break;
        default:
                break;
            }
        }
        if(!next.isEmpty()){
            messageIdentifier = next;
            includeStartMessage = true;//in this case include the next one
        }
    }
    //CHECK 3
    //when includeStartMessage = false;;if the start message does not exist,get the next available one
    if((!messageIdentifier.isEmpty()) && (!messageRecordExists) && (conversationIdentifier.isEmpty())){

        QString next;
        switch (messageType) {

        case  MessageType::Sent:{
            next = messagesModel_.getNextSentMessage(messageIdentifier,userId).messageIdentifier();
            break;
        }
        case  MessageType::Received:{
            next = messagesModel_.getNextReceivedMessage(messageIdentifier,userId).messageIdentifier();
            break;
        }
        case  MessageType::SentOrReceived:{
            next = messagesModel_.getNextMessage(messageIdentifier,userId).messageIdentifier();
            break;
        }
        case  MessageType::Read:{
            next = messagesModel_.getNextReadMessage(messageIdentifier,userId).messageIdentifier();
            break;
        }
        case  MessageType::UnRead:{
            next = messagesModel_.getNextUnReadMessage(messageIdentifier,userId).messageIdentifier();
            break;
        }
        }
        if(!next.isEmpty()){
            messageIdentifier = next;
            includeStartMessage = true;//in this case include the next one
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();return;
        }
    }

    Message start = messagesModel_.getMessage(messageIdentifier);

    Conversation conversation(conversationIdentifier);

    const bool useMessageIdentifier = start.getConversation().isInvolved(userId);
    const bool useConversationIdentifier = (conversation.isInvolved(userId) && messagesModel_.conversationExist(conversation));

    const bool receivedValidInput = (useMessageIdentifier || useConversationIdentifier);
    //CHECK 4
    if(!receivedValidInput){
        //error
        if(!messageIdentifier.isEmpty()){
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::ConversationNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }

    switch (messageType) {

    case  MessageType::Sent:{
        SentMessages sentMessages;

        if(useMessageIdentifier){
            sentMessages = messagesModel_.getSentMessages(start,maxNoOfMessages,userId,includeStartMessage,sortOrder);
        }else if(useConversationIdentifier){
            sentMessages = messagesModel_.getSentMessages(conversation,userId,maxNoOfMessages,sortOrder);
        }


        if(!sentMessages.isEmpty()){
            response.setAction(MessagesReceivedActionName)
                    .addExtras(ExtrasNames::jsonMessagesExtras,sentMessages.toJson())
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }
    case  MessageType::Received:{
        ReceivedMessages receivedMessages;

        if(useMessageIdentifier){
            receivedMessages = messagesModel_.getReceivedMessages(start,maxNoOfMessages,userId,includeStartMessage,sortOrder);
        }else if(useConversationIdentifier){
            receivedMessages = messagesModel_.getReceivedMessages(conversation,userId,maxNoOfMessages,sortOrder);
        }


        if(!receivedMessages.isEmpty()){
            response.setAction(MessagesReceivedActionName)
                    .addExtras(ExtrasNames::jsonMessagesExtras,receivedMessages.toJson())
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }
    case  MessageType::SentOrReceived:{
        Messages messages;

        if(useMessageIdentifier){
            messages = messagesModel_.getMessages(start,userId,maxNoOfMessages,includeStartMessage,sortOrder);
        }else if(useConversationIdentifier){
            messages = messagesModel_.getMessages(conversation,userId,maxNoOfMessages,sortOrder);
        }


        if(!messages.isEmpty()){
            response.setAction(MessagesReceivedActionName)
                    .addExtras(ExtrasNames::jsonMessagesExtras,messages.toJson())
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }
    case  MessageType::Read:{

        ReadMessages readMessages;

        if(useMessageIdentifier){
            readMessages = messagesModel_.getReadMessages(start,maxNoOfMessages,userId,includeStartMessage,sortOrder);
        }else if(useConversationIdentifier){
            readMessages = messagesModel_.getReadMessages(conversation,userId,maxNoOfMessages,sortOrder);
        }


        if(!readMessages.isEmpty()){
            response.setAction(MessagesReceivedActionName)
                    .addExtras(ExtrasNames::jsonMessagesExtras, readMessages.toJson())
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }
    case  MessageType::UnRead:{
        UnReadMessages unReadMessages;

        if(useMessageIdentifier){
            unReadMessages = messagesModel_.getUnReadMessages(start,maxNoOfMessages,userId,includeStartMessage,sortOrder);
        }else if(useConversationIdentifier){
            unReadMessages = messagesModel_.getUnReadMessages(conversation,userId,maxNoOfMessages,sortOrder);
        }

        if(!unReadMessages.isEmpty()){
            response.setAction(MessagesReceivedActionName)
                    .addExtras(ExtrasNames::jsonMessagesExtras, unReadMessages.toJson())
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }else{
            response.setAction(GetMessageErrorActionName)
                    .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::MessageNotFound)
                    .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                    .send();
        }
        return;
    }
    default:
        response.setAction(GetMessageErrorActionName)
                .addExtras(ExtrasNames::OnGetMessagesErrorExtras,(int)Errors::OnGetMessagesError::UnknownError)
                .addExtras(ExtrasNames::getMessageRequestTagExtras,requestTag)
                .send();
    }


}

void ConversationController::onActionSendMessageTriggered(Extras extras){
    const QString& receiver = extras.getStringExtras(ExtrasNames::receiverExtras);
    const QString& messageText = extras.getStringExtras(ExtrasNames::messageTextExtras);
    const QString& sendMessageRequestTag = extras.getStringExtras(ExtrasNames::sendMessageRequestTagExtras);

    auto response = user().response();
    response.setController(TO_CSTR(ConversationController));

    if(!appUsers().userRegistered(receiver)){
        response.setAction(MessageNotSentActionName)
                .addExtras(ExtrasNames::OnSendMessageErrorExtras,(int)Errors::OnSendMessageError::UserNotFound)
                .addExtras(ExtrasNames::sendMessageRequestTagExtras,sendMessageRequestTag)
                .send();
        return;
    }

    auto messageReceiver = appUsers().getUser(receiver);
    if(user().username() == messageReceiver.username()){
        response.setAction(MessageNotSentActionName)
                .addExtras(ExtrasNames::OnSendMessageErrorExtras,(int)Errors::OnSendMessageError::UnknownError)
                .addExtras(ExtrasNames::sendMessageRequestTagExtras,sendMessageRequestTag)
                .send();
        return;
    }

    QString messageIdentifier;
    if(messagesModel_.recordNewMessage(user().id(),messageReceiver.id(),messageText,&messageIdentifier)){
        auto message = messagesModel_.getMessage(messageIdentifier);
        if(!message.isEmpty()){
            response.setAction(MessageSentActionName)
                    .addExtras(ExtrasNames::jsonMessageExtras,message.toJson())
                    .addExtras(ExtrasNames::sendMessageRequestTagExtras,sendMessageRequestTag)
                    .send();
        }else{
            response.setAction(MessageNotSentActionName)
                    .addExtras(ExtrasNames::OnSendMessageErrorExtras,(int)Errors::OnSendMessageError::UnknownError)
                    .addExtras(ExtrasNames::sendMessageRequestTagExtras,sendMessageRequestTag)
                    .send();
        }
    }else{
        response.setAction(MessageNotSentActionName)
                .addExtras(ExtrasNames::OnSendMessageErrorExtras,(int)Errors::OnSendMessageError::UnknownError)
                .addExtras(ExtrasNames::sendMessageRequestTagExtras,sendMessageRequestTag)
                .send();
    }
}

void ConversationController::onActionGetMessageCountTriggered(Extras extras){

    const QString& messageCountRequestTag = extras.getStringExtras(ExtrasNames::getMessageCountRequestTagExtras);
    const MessageType messageType = (MessageType)extras.getIntExtras(ExtrasNames::MessageTypeExtras);
    Conversation conversation(extras.getStringExtras(ExtrasNames::conversationIdentifierExtras));

    const auto userId  = user().id();
    const int count = messagesModel_.getMessageCount(conversation,userId,messageType);
    auto response = user().response();

    response.setController(TO_CSTR(ConversationController))
            .setAction(MessageCountReceivedActionName)
            .addExtras(ExtrasNames::getMessageCountRequestTagExtras,messageCountRequestTag)
            .addExtras(ExtrasNames::messageCountExtras,count)
            .send();
}

void ConversationController::onActionGetLastMessageTriggered(Extras extras){
    const MessageType messageType = (MessageType)extras.getIntExtras(ExtrasNames::MessageTypeExtras);
    Conversation conversation(extras.getStringExtras(ExtrasNames::conversationIdentifierExtras));
    const QString& lastMessageRequestTag = extras.getStringExtras(ExtrasNames::getLastMessageRequestTagExtras);

    auto response = user().response();
    const auto userId  = user().id();

    Message message = messagesModel_.getLastMessage(conversation,userId,messageType);
    if((message.sender() == userId) && (message.deletedBySender())){
        message = Message();
    }

    if((message.receiver() == userId) && (message.deletedByReceiver())){
        message = Message();
    }
    response.setController(TO_CSTR(ConversationController))
            .setAction(LastMessageReceivedActionName)
            .addExtras(ExtrasNames::jsonMessageExtras,message.toJson())
            .addExtras(ExtrasNames::getLastMessageRequestTagExtras,lastMessageRequestTag)
            .send();
}

void ConversationController::onActionDeleteMessageTriggered(Extras extras){
    const QString& messageIdentifier = extras.getStringExtras(ExtrasNames::messageIdentifierExtras);
    const QString& tag = extras.getStringExtras(ExtrasNames::deleteMessageRequestTagExtras);
    const auto userId = user().id();
    const bool isInvolvedInAConversation = (messagesModel_.isSender(messageIdentifier,userId)  || messagesModel_.isReceiver(messageIdentifier,userId));
    auto response = user().response();
    response.setController(TO_CSTR(ConversationController))
            .setAction(MessageDeletionStatusActionName)
            .addExtras(ExtrasNames::deleteMessageRequestTagExtras,tag);

    Message message = messagesModel_.getMessage(messageIdentifier);
    if((message.sender() == userId) && (message.deletedBySender())){
        response.addExtras("status",false)
                .send();
        return;
    }

    if((message.receiver() == userId) && (message.deletedByReceiver())){
        response.addExtras("status",false)
                .send();
        return;
    }

    if(isInvolvedInAConversation){
        bool deleted = messagesModel_.markAsDeleted(messageIdentifier,userId);
        response.addExtras("status",deleted)
                .send();
    }else{
        response.addExtras("status",false)
                .send();
    }
}

void ConversationController::triggerAction(MagabeData data)
{
    if(user().isClientAuthenticated()){
        Controller::triggerActionDoContinue(data);
    }else{
        qWarning() << QString("%1 |Error Can not triggerAction,Client with MgbSessionId = %2 is not authenticated").arg(name()).arg(user().getThisClientMgbSessionId());
    }
}

