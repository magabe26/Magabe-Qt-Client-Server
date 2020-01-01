/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MESSAGES_H
#define MESSAGES_H
#include <QList>
#include <QJsonDocument>
#include "Helpers/conversation.h"
#include "Helpers/sort.h"


class MessagesModel;
/** ------------Message---------------
 * @brief The Message class
 */
class Message{
public:
    Message();
    Message(const Message& other);
    Message(Message&& other);
    Message &operator =(const Message &other);
    QString messageIdentifier() const;
    QString decodedText() const;
    QString encodedText() const;
    unsigned long sender() const;
    unsigned long receiver() const;
    unsigned long long timestamp() const;
    bool readByReceiver() const;
    bool deletedBySender() const;
    bool deletedByReceiver() const;
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented);
    static Message fromJson(const QString& json);
    bool operator ==(const Message& other);
    bool operator !=(const Message& other);
    bool isValid();
    static bool isValidMessageIdentifier(const QString& messageIdentifier);
    Conversation getConversation();
    bool isEmpty();

private:
    enum class SearchBy{
        MessageIdentifier,
        ConversationIdentifier,
        Message,
        Sender,
        Receiver,
        Timestamp,
        ReceiverReadStatus,
        SenderAndSenderDeletionStatus,
        ReceiverAndReceiverDeletionStatus,
        All
    };

    friend class MessagesModel;
    friend class MessageList;
    friend class SentMessages;
    friend class ReceivedMessages;
    friend class Messages;
    friend class MagabeClientApp;
    friend class ConversationController;

    QString messageIdentifier_;
    QString conversationIdentifier_;
    QString encodedText_;
    unsigned long sender_;
    unsigned long receiver_;
    unsigned long long timestamp_;
    bool senderDeletionStatus_;
    bool receiverDeletionStatus_;
    bool receiverReadStatus_;
    SearchBy searchBy_;
    void clearIfIsInvalid();
};

/** -------------MessageList------------------
 * @brief The MessageList class
 */
class MessageList:public QList<Message>{
public:

    MessageList() = default;
    explicit MessageList(const QString& json);
    QString toJson(QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Indented);
    static MessageList fromJson(const QString& json);
    void sortMessages(Sort::Order sortOrder);
    Sort::Order getSortOrder() const;
    Message getMessage(const QString& messageIdentifier);
    ConversationList getConversations();

private:
    friend class MessagesModel;
    friend class SentMessages;
    friend class ReceivedMessages;
    friend class Messages;
    Sort::Order sortOrder_;
    void setMessageSortOrder(const Sort::Order &sortOrder);
};

class SentMessages :public MessageList{
public:
    SentMessages() = default;
    explicit SentMessages(const QString& json);
};

class UnReadMessages :public MessageList{
public:
    UnReadMessages() = default;
    explicit UnReadMessages(const QString& json);
};

class ReadMessages :public MessageList{
public:
    ReadMessages() = default;
    explicit ReadMessages(const QString& json);
};


class ReceivedMessages :public MessageList{
public:
    ReceivedMessages() = default;
    explicit ReceivedMessages(const QString& json);

    UnReadMessages getUnReadMessages();
    ReadMessages getReadMessages();
};

class Messages :public MessageList{
public:
    Messages() = default;
    explicit Messages(const QString& json);

    SentMessages getSentMessages(unsigned long sender);
    ReceivedMessages getReceivedMessages(unsigned long receiver);
};

QDebug operator <<(QDebug debug, const Message& msg);
QDebug operator <<(QDebug debug, const MessageList &list);

#endif // MESSAGES_H
