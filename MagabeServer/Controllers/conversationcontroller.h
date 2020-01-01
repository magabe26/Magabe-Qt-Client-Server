/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONVERSATIONCONTROLLER_H
#define CONVERSATIONCONTROLLER_H

#include <QObject>
#include <Controllers/controller.h>
#include "Models/messagesmodel.h"
#include "Helpers/errors.h"


class ConversationController : public Controller
{
    Q_OBJECT
    Q_ENUMS(Actions)

public:
    struct ExtrasNames{
        NEW_EXTRAS_NAME(userId)
        NEW_EXTRAS_NAME(messageIdentifier)
        NEW_EXTRAS_NAME(maxNoOfMessages)
        NEW_EXTRAS_NAME(includeStartMessage)
        NEW_EXTRAS_NAME(sortOrder)
        NEW_EXTRAS_NAME(conversationIdentifier)
        NEW_EXTRAS_NAME(MessageType)
        NEW_EXTRAS_NAME(jsonMessages)
        NEW_EXTRAS_NAME(OnGetMessagesError)
        NEW_EXTRAS_NAME(getMessageRequestTag)
        NEW_EXTRAS_NAME(receiver)
        NEW_EXTRAS_NAME(messageText)
        NEW_EXTRAS_NAME(sendMessageRequestTag)
        NEW_EXTRAS_NAME(jsonMessage)
        NEW_EXTRAS_NAME(OnSendMessageError)
        NEW_EXTRAS_NAME(getMessageCountRequestTag)
        NEW_EXTRAS_NAME(messageCount)
        NEW_EXTRAS_NAME(getLastMessageRequestTag)
        NEW_EXTRAS_NAME(deleteMessageRequestTag)

    };


    enum Actions{
        ActionGetMessages,
        ActionSendMessage,
        ActionGetMessageCount,
        ActionGetLastMessage,
        ActionDeleteMessage
    };

    REGISTER_ACTIONS

    NEW_ACTION_NAME(MessagesReceived)
    NEW_ACTION_NAME(GetMessageError)
    NEW_ACTION_NAME(MessageSent)
    NEW_ACTION_NAME(MessageNotSent)
    NEW_ACTION_NAME(MessageCountReceived)
    NEW_ACTION_NAME(LastMessageReceived)
    NEW_ACTION_NAME(MessageDeletionStatus)

    explicit ConversationController(QObject *parent);

    // Controller interface
    Controller *createNew();

signals:

public slots:
    void onActionGetMessagesTriggered(Extras extras);
    void onActionSendMessageTriggered(Extras extras);
    void onActionGetMessageCountTriggered(Extras extras);
    void onActionGetLastMessageTriggered(Extras extras);
    void onActionDeleteMessageTriggered(Extras extras);
    // Controller interface
    void triggerAction(MagabeData data);
private:
    MessagesModel messagesModel_;
};

#endif // CONVERSATIONCONTROLLER_H
