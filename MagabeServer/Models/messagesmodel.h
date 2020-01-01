/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include <QObject>
#include <Models/model.h>
#include "Models/usermodel.h"
#include "Helpers/messages.h"
#include "Helpers/sort.h"
#include "Helpers/messagetype.h"


/** ------------------MessageModel------------
 * @brief The MessageModel class
 */
class  MessagesModel : public Model
{
    Q_OBJECT
public:


    explicit MessagesModel(QObject *parent = nullptr);
    bool markAsRead(const QString &messageIdentifier);
    bool markAsUnread(const QString &messageIdentifier);
    bool isSender(const QString &messageIdentifier, unsigned long userId);
    bool isReceiver(const QString &messageIdentifier, unsigned long userId);
    bool hasSentOrReceivedAMessage(unsigned long userId);
    bool markAsUndeleted(const QString &messageIdentifier, unsigned long userId);
    bool markAsDeleted(const QString &messageIdentifier, unsigned long userId);
    bool messageExists(const QString &messageIdentifier);
    bool messageExists(const Message& message);
    unsigned long getOtherUser(const QString &messageIdentifier, unsigned long userId);
    bool recordNewMessage(unsigned long sender, unsigned long receiver, const QString &message,QString *messageIdentifier);
    void eraseMessageRecord(Message * const message, unsigned long userId);
    long long getMessageTimestamp(const QString &messageIdentifier);
    int getMessageCount(const QString &conversationIdentifier);
    QString getMessageIdentifierOfNextMessage(const QString &messageIdentifier);
    Message getMessage(const QString &messageIdentifier);
    Message getLastMessage(const QString &conversationIdentifier);
    ConversationList getConversations(unsigned long userId);
    bool conversationExist(const Conversation& conversation);



    /*--------getSentMessages--------*/
    SentMessages getSentMessages(const Message &start, int maxNoOfMessages
                                 , unsigned long userId, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);

    SentMessages getSentMessages(const Conversation& conversation, unsigned long userId
                                 , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC);
    /*---------getReceivedMessages-------*/

    ReceivedMessages getReceivedMessages(const Message &start, int maxNoOfMessages
                                         , unsigned long userId, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);

    ReceivedMessages getReceivedMessages(const Conversation& conversation, unsigned long userId
                                         , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC);
    /*--------getMessages--------*/

    Messages getMessages(const Message &start, unsigned long  userId, int maxNoOfMessages = 5
            , bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);

    Messages getMessages(const Conversation& conversation, unsigned long userId, int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC);

    /*--------getReadMessages--------*/

    ReadMessages getReadMessages(const Message &start, int maxNoOfMessages
                                 , unsigned long userId, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);

    ReadMessages getReadMessages(const Conversation& conversation, unsigned long userId
                                 , int maxNoOfMessages = 5, const Sort::Order sortOrder = Sort::Order::ASC);
    /*--------getUnReadMessages--------*/

    UnReadMessages getUnReadMessages(const Message &start, int maxNoOfMessages
                                     , unsigned long userId, bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);

    UnReadMessages getUnReadMessages(const Conversation& conversation,unsigned long userId
                                     ,int maxNoOfMessages = 5,const Sort::Order sortOrder = Sort::Order::ASC);

    /*----------------*/

    Message getFirstMessage(const QString &conversationIdentifier);

    Message getNextSentMessage(const QString& currentMessageIndentifier, unsigned long sender);
    Message getNextReceivedMessage(const QString &currentMessageIndentifier, unsigned long receiver);
    Message getNextReadMessage(const QString &currentMessageIndentifier, unsigned long receiver);
    Message getNextUnReadMessage(const QString &currentMessageIndentifier, unsigned long receiver);
    Message getNextMessage(const QString &currentMessageIndentifier,unsigned long userId);
    //message count of 'un-deleted messages'
    int getMessageCount(Conversation conversation,unsigned long userId,MessageType messageType = MessageType::SentOrReceived);
    Message getLastMessage(Conversation conversation, unsigned long userId, MessageType messageType = MessageType::SentOrReceived);

signals:

public slots:

private:
    friend class ConversationController;

    UserModel userModel_;

    MODEL_COLUMN_NAME(messageIdentifier)
    MODEL_COLUMN_NAME(conversationIdentifier)
    MODEL_COLUMN_NAME(message)
    MODEL_COLUMN_NAME(sender)
    MODEL_COLUMN_NAME(receiver)
    MODEL_COLUMN_NAME(timestamp)
    MODEL_COLUMN_NAME(senderDeletionStatus)
    MODEL_COLUMN_NAME(receiverDeletionStatus)
    MODEL_COLUMN_NAME(receiverReadStatus)

    void createTable();
    QString generateConversationIdentifier(unsigned long sender, unsigned long receiver);
    QString generateConversationIdentifier(const QString &messageIdentifier);
    QString generateMessageIdentifier(unsigned long sender, unsigned long receiver);
    QString getLastMessageIdentifier(const QString &convesationIdentifier);
    bool isValidMessageIdentifier(const QString &messageIdentifier);
    Messages getMessages(const QString &startMessageIdentifier, unsigned long  userId, int maxNoOfMessages = 5
            , bool includeStartMessage = false, const Sort::Order sortOrder = Sort::Order::ASC);
    int getMessageRowId(const Message& message);
    QString getMessageIdentifier(const int rowId);

    SentMessages getSentMessages(const QString &startMessageIdentifier, unsigned long sender
                                 , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder);
    ReceivedMessages getReceivedMessages(const QString &startMessageIdentifier, unsigned long receiver
                                         , int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder);
    ReadMessages getReadMessages(const QString &startMessageIdentifier, unsigned long receiver, int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder);
    UnReadMessages getUnReadMessages(const QString &startMessageIdentifier, unsigned long receiver, int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder);
};

#endif // MESSAGESMODEL_H
