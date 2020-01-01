/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "conversationcontroller.h"


ConversationController::ConversationController(QObject *parent) : Controller(parent)
{
    //first thing to do
    CONTROLLER_SET_NAME(ConversationController)

}


Controller *ConversationController::createNew()
{
    return new ConversationController(nullptr);
}

void ConversationController::onActionMessagesReceivedTriggered(Extras extras){
    const QString& jsonMessages = extras.getStringExtras(ExtrasNames::jsonMessagesExtras);
    emit messagesReceived(jsonMessages,extras.getStringExtras(ExtrasNames::getMessageRequestTagExtras));
}

void ConversationController::onActionGetMessageErrorTriggered(Extras extras){
    const int onGetMessagesError = extras.getIntExtras(ExtrasNames::OnGetMessagesErrorExtras);
    emit getMessageError(onGetMessagesError,extras.getStringExtras(ExtrasNames::getMessageRequestTagExtras));

}

void ConversationController::onActionMessageSentTriggered(Extras extras){
    const QString& sendMessageRequestTag = extras.getStringExtras(ExtrasNames::sendMessageRequestTagExtras);
    const QString& jsonMessage = extras.getStringExtras(ExtrasNames::jsonMessageExtras);
    emit messageSent(jsonMessage,sendMessageRequestTag);
    // qDebug() << sendMessageRequestTag << jsonMessage;

}

void ConversationController::onActionMessageNotSentTriggered(Extras extras){
    const QString& sendMessageRequestTag = extras.getStringExtras(ExtrasNames::sendMessageRequestTagExtras);
    const int onSendMessageError = extras.getIntExtras(ExtrasNames::OnSendMessageErrorExtras);
    emit messageNotSent(onSendMessageError,sendMessageRequestTag);
    // qDebug() << sendMessageRequestTag << onSendMessageError;
}

void ConversationController::onActionUserNamesReceivedTriggered(Extras extras){
    const QString & fetchUserNamesRequestTag = extras.getStringExtras(ExtrasNames::fetchUserNamesRequestTagExtras);
    const QString & jsonUserNames =  extras.getStringExtras(ExtrasNames::jsonUserNamesExtras);
    emit userNamesReceived(jsonUserNames,fetchUserNamesRequestTag);
    //qDebug() <<fetchUserNamesRequestTag << jsonUserNames;

}

void ConversationController::onActionMessageCountReceivedTriggered(Extras extras)
{
    const int messageCount = extras.getIntExtras(ExtrasNames::messageCountExtras);
    const QString& tag = extras.getStringExtras(ExtrasNames::getMessageCountRequestTagExtras);
    emit messageCountReceived(messageCount,tag);
}

void ConversationController::onActionLastMessageReceivedTriggered(Extras extras){
    const QString& tag = extras.getStringExtras(ExtrasNames::getLastMessageRequestTagExtras);
    const QString& message = extras.getStringExtras(ExtrasNames::jsonMessageExtras);
    emit lastMessageReceived(message,tag);
}

void ConversationController::onActionMessageDeletionStatusTriggered(Extras extras){
    const bool status = extras.getBooleanExtras("status");
    const QString& tag= extras.getStringExtras(ExtrasNames::deleteMessageRequestTagExtras);
    emit messageDeletionStatus(status,tag);
}


