/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONVERSATION_H
#define CONVERSATION_H
#include <QString>
#include <QList>

class Conversation{
public:
    Conversation(const QString& conversationIdentifier);
    Conversation(const Conversation& other);
    QString conversationIdentifier();
    bool operator ==(const Conversation& other );
    bool operator !=(const Conversation& other );
    bool isValid();
    unsigned long getOtherUser(unsigned long userId);
    bool isInvolved(unsigned long userId);
    static bool isValidConversationIdentifier(const QString& conversationIdentifier);
    static QString generateConversationIdentifier(unsigned long userId1,unsigned long userId2);

private:
    friend class MessagesModel;
    QString conversationIdentifier_;
};

class ConversationList:public QList<Conversation>{
public:
    Conversation getConversation(const QString& conversationIdentifier);
};

QDebug operator <<(QDebug debug, const Conversation& conversation);
QDebug operator <<(QDebug debug, const ConversationList &list);

#endif // CONVERSATION_H
