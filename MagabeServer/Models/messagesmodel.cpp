/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */



#include "messagesmodel.h"
#include "Db/createtablestatement.h"
#include "Db/insertstatement.h"
#include <QSqlRecord>
#define INIT_MESSAGE_MODEL_FIELDS\
    int messageIdentifierFieldNo = sqlQuery->record().indexOf(messageIdentifierColumn);\
    int conversationIdentifierFieldNo = sqlQuery->record().indexOf(conversationIdentifierColumn);\
    int messageFieldNo = sqlQuery->record().indexOf(messageColumn);\
    int senderFieldNo = sqlQuery->record().indexOf(senderColumn);\
    int receiverFieldNo = sqlQuery->record().indexOf(receiverColumn);\
    int timestampFieldNo = sqlQuery->record().indexOf(timestampColumn);\
    int senderDeletionStatusFieldNo = sqlQuery->record().indexOf(senderDeletionStatusColumn);\
    int receiverDeletionStatusFieldNo = sqlQuery->record().indexOf(receiverDeletionStatusColumn);\
    int receiverReadStatusFieldNo = sqlQuery->record().indexOf(receiverReadStatusColumn);


#define  CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES\
    Message msg;\
    msg.messageIdentifier_  = sqlQuery->value(messageIdentifierFieldNo).toString();\
    msg.conversationIdentifier_ = sqlQuery->value(conversationIdentifierFieldNo).toString();\
    msg.encodedText_ = sqlQuery->value(messageFieldNo).toString();\
    msg.sender_  = sqlQuery->value(senderFieldNo).toString().toULong();\
    msg.receiver_ = sqlQuery->value(receiverFieldNo).toString().toULong();\
    msg.timestamp_ = sqlQuery->value(timestampFieldNo).toString().toULongLong();\
    msg.senderDeletionStatus_ = ((sqlQuery->value(senderDeletionStatusFieldNo).toString().toInt() == 1) ? true :false);\
    msg.receiverDeletionStatus_ = ((sqlQuery->value(receiverDeletionStatusFieldNo).toString().toInt() == 1) ? true :false);\
    msg.receiverReadStatus_ = ((sqlQuery->value(receiverReadStatusFieldNo).toString().toInt() == 1) ? true : false);



MessagesModel::MessagesModel(QObject *parent) : Model(parent)
{
    setTablename("messages");
}


bool MessagesModel::recordNewMessage(unsigned long sender, unsigned long receiver, const QString& message,QString *messageIdentifier)
{
    if(!userModel_.userExists(sender)){
        qWarning() << QString("MessageModel::recordNewMessage |Error user(sender) with id = %1 does not exists").arg(sender);
        return false;
    }

    if(!userModel_.userExists(sender)){
        qWarning() << QString("MessageModel::recordNewMessage |Error user(receiver) with id = %1 does not exists").arg(receiver);
        return false;
    }



    const QString &msgIdentifier = generateMessageIdentifier(sender,receiver);
    const QString &conversationIdentifier = generateConversationIdentifier(sender,receiver);

    InsertStatement insertStatement(tablename());
    insertStatement.insert(messageIdentifierColumn,msgIdentifier)
            .insert(conversationIdentifierColumn,conversationIdentifier)
            .insert(messageColumn,QString(message.toLatin1().toHex()))
            .insert(senderColumn,sender)
            .insert(receiverColumn,receiver)
            .insert(timestampColumn,getTimestamp())
            .insert(senderDeletionStatusColumn,0)
            .insert(receiverDeletionStatusColumn,0)
            .insert(receiverReadStatusColumn,0);


    const bool recorded = m_db->excuteSqlQuery(insertStatement.toString(),QString("New Message -> %1 from %2 to  %3 ,message recored to db ").arg(msgIdentifier).arg(sender).arg(receiver),QString("New Message -> %1 from %2 to  %3 ,recording message to db  Failed ").arg(msgIdentifier).arg(sender).arg(receiver));
    if(messageIdentifier){
        if(recorded){
            *messageIdentifier = msgIdentifier;
        }else{
            *messageIdentifier = "";
        }
    }
    return recorded;
}

void MessagesModel::eraseMessageRecord(Message *const message, unsigned long userId){
    if(!message){
        return;
    }
    if(!message->isEmpty()){
        if(message->sender_ == userId){
            message->senderDeletionStatus_ = markAsDeleted(message->messageIdentifier_,userId);
        }else if(message->receiver_ == userId){
            message->receiverDeletionStatus_ = markAsDeleted(message->messageIdentifier_,userId);
        }
        if(message->senderDeletionStatus_ && message->receiverDeletionStatus_){
            deleteRecord(messageIdentifierColumn,message->messageIdentifier_);
        }
    }
}

long long MessagesModel::getMessageTimestamp(const QString& messageIdentifier){
    auto v = getRecord(timestampColumn,messageIdentifierColumn,messageIdentifier,"MessageModel::getMessageTimestamp(const QString& messageIdentifier)");
    if(v.isEmpty()){
        return 0;
    }else{
        return v.toLongLong();
    }
}

unsigned long MessagesModel::getOtherUser(const QString& messageIdentifier,unsigned long userId){
    if(messageIdentifier.isEmpty()){
        return 0;
    }

    QStringList arry = messageIdentifier.split(':',QString::SkipEmptyParts);
    if(arry.length() != 3){
        return 0;
    }else{
        if(arry.at(0).toUInt() == userId){
            return arry.at(1).toUInt();
        }else {
            return arry.at(0).toUInt();
        }
    }
}


bool MessagesModel::messageExists(const QString& messageIdentifier)
{
    if(!isValidMessageIdentifier(messageIdentifier)){
        return false;
    }
    const QString& value = getRecord(messageIdentifierColumn,messageIdentifierColumn,messageIdentifier,"MessageModel::isMessageExist(const QString& messageIdentifier)");
    return (value == messageIdentifier);
}

bool MessagesModel::messageExists(const Message &message){
    return messageExists(message.messageIdentifier_);
}

bool MessagesModel::markAsDeleted(const QString& messageIdentifier,unsigned long userId)
{
    if(isSender(messageIdentifier,userId)){
        return  updateRecord(senderDeletionStatusColumn,QString::number(1),messageIdentifierColumn,messageIdentifier);
    }else if(isReceiver(messageIdentifier,userId)){
        return  updateRecord(receiverDeletionStatusColumn,QString::number(1),messageIdentifierColumn,messageIdentifier);
    }else{
        return false;
    }
}

bool MessagesModel::markAsUndeleted(const QString& messageIdentifier,unsigned long userId)
{
    if(isSender(messageIdentifier,userId)){
        return  updateRecord(senderDeletionStatusColumn,QString::number(0),messageIdentifierColumn,messageIdentifier);
    }else if(isReceiver(messageIdentifier,userId)){
        return  updateRecord(receiverDeletionStatusColumn,QString::number(0),messageIdentifierColumn,messageIdentifier);
    }else{
        return false;
    }
}

bool MessagesModel::isSender(const QString& messageIdentifier,unsigned long userId){
    if((userId <= 0) || messageIdentifier.isEmpty()){
        return false;
    }

    QStringList selectColumns; selectColumns << senderColumn;
    QStringList whereColumns; whereColumns << messageIdentifierColumn << senderColumn;
    QStringList likeValues; likeValues << messageIdentifier << QString::number(userId);

    auto query = generateSelectQuery(selectColumns,whereColumns,likeValues,0,1);
    auto sqlQuery = m_db->getSqlQuery();

    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {

        QString idStr = sqlQuery->value(0).toString();
        if(idStr.isEmpty()){
            return false;
        }else{
            bool ok =false;
            unsigned long id = idStr.toULong(&ok);
            if(ok){
                return (id == userId);
            }else{
                return ok;
            }
        }
    }else{
        return false;
    }
}

bool MessagesModel::isReceiver(const QString& messageIdentifier,unsigned long userId){
    if((userId <= 0) || (messageIdentifier.isEmpty())){
        return false;
    }

    QStringList selectColumns; selectColumns << receiverColumn;
    QStringList whereColumns; whereColumns << messageIdentifierColumn << receiverColumn;
    QStringList likeValues; likeValues << messageIdentifier << QString::number(userId);

    auto query = generateSelectQuery(selectColumns,whereColumns,likeValues,0,1);
    auto sqlQuery = m_db->getSqlQuery();

    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {

        QString idStr = sqlQuery->value(0).toString();
        if(idStr.isEmpty()){
            return false;
        }else{
            bool ok =false;
            unsigned long id = idStr.toULong(&ok);
            if(ok){
                return (id == userId);
            }else{
                return ok;
            }
        }
    }else{
        return false;
    }
}

bool MessagesModel::hasSentOrReceivedAMessage(unsigned long userId){
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\""
                            " OR \"%5\" LIKE \"%6\""
                            " LIMIT 1")
            .arg(messageIdentifierColumn).arg(tablename())
            .arg(senderColumn).arg(QString::number(userId))
            .arg(receiverColumn).arg(QString::number(userId));

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        return (!sqlQuery->value(0).toString().isEmpty());
    }else{
        return false;
    }
}

bool MessagesModel::markAsUnread(const QString& messageIdentifier)
{
    return updateRecord(receiverReadStatusColumn,QString::number(0),messageIdentifierColumn,messageIdentifier);
}

bool MessagesModel::markAsRead(const QString& messageIdentifier)
{
    return updateRecord(receiverReadStatusColumn,QString::number(1),messageIdentifierColumn,messageIdentifier);
}

QString MessagesModel::generateConversationIdentifier(unsigned long sender,unsigned long receiver)
{
    return Conversation::generateConversationIdentifier(sender,receiver);
}

QString MessagesModel::generateConversationIdentifier(const QString& messageIdentifier)
{

    if(Message::isValidMessageIdentifier(messageIdentifier)){
        Message message; message.messageIdentifier_ = messageIdentifier;
        return message.getConversation().conversationIdentifier();
    }else{
        return "";
    }
}

int MessagesModel::getMessageCount(const QString& conversationIdentifier){
    QStringList whereColumns; whereColumns << conversationIdentifierColumn;
    QStringList likeValues; likeValues << conversationIdentifier;
    return getRecordCount(whereColumns,likeValues);
}

Messages MessagesModel::getMessages(const Message &start, unsigned long userId, int maxNoOfMessages, bool includeStartMessage, const Sort::Order sortOrder){
    return getMessages(start.messageIdentifier_,userId,maxNoOfMessages,includeStartMessage,sortOrder);
}

Messages MessagesModel::getMessages(const Conversation &conversation, unsigned long userId, int maxNoOfMessages, const Sort::Order sortOrder)
{
    auto firstMessage = getNextMessage(QString("%1:0").arg(conversation.conversationIdentifier_),userId);
    return getMessages(firstMessage,userId,maxNoOfMessages,true,sortOrder);
}

ReadMessages MessagesModel::getReadMessages(const Message &start, int maxNoOfMessages, unsigned long userId, bool includeStartMessage, const Sort::Order sortOrder)
{
    return getReadMessages(start.messageIdentifier_,userId,maxNoOfMessages,includeStartMessage,sortOrder);
}

ReadMessages MessagesModel::getReadMessages(const Conversation &conversation, unsigned long userId
                                            , int maxNoOfMessages, const Sort::Order sortOrder)
{
    auto firstMessage = getNextReadMessage(QString("%1:0").arg(conversation.conversationIdentifier_),userId);
    return getReadMessages(firstMessage,maxNoOfMessages,userId,true,sortOrder);
}

UnReadMessages MessagesModel::getUnReadMessages(const Message &start, int maxNoOfMessages, unsigned long userId, bool includeStartMessage, const Sort::Order sortOrder)
{
    return getUnReadMessages(start.messageIdentifier_,userId,maxNoOfMessages,includeStartMessage,sortOrder);
}

UnReadMessages MessagesModel::getUnReadMessages(const Conversation &conversation, unsigned long userId, int maxNoOfMessages, const Sort::Order sortOrder)
{
    auto firstMessage = getNextUnReadMessage(QString("%1:0").arg(conversation.conversationIdentifier_),userId);
    return getUnReadMessages(firstMessage,maxNoOfMessages,userId,true,sortOrder);
}

Message MessagesModel::getMessage(const QString &messageIdentifier){
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\";")
            .arg('*').arg(tablename())
            .arg(messageIdentifierColumn).arg(messageIdentifier);

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        INIT_MESSAGE_MODEL_FIELDS
                CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                return msg;
    }else{
        return Message();
    }
}

Message MessagesModel::getLastMessage(const QString& conversationIdentifier){
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\""
                            " ORDER BY \"%5\" DESC LIMIT 1;")
            .arg('*').arg(tablename())
            .arg(conversationIdentifierColumn).arg(conversationIdentifier)
            .arg(timestampColumn);

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        INIT_MESSAGE_MODEL_FIELDS
                CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                return msg;
    }else{
        return Message();
    }
}

Message MessagesModel::getFirstMessage(const QString& conversationIdentifier){
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\""
                            " ORDER BY \"%5\" ASC LIMIT 1;")
            .arg('*').arg(tablename())
            .arg(conversationIdentifierColumn).arg(conversationIdentifier)
            .arg(timestampColumn);

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        INIT_MESSAGE_MODEL_FIELDS
                CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                return msg;
    }else{
        return Message();
    }
}
ConversationList MessagesModel::getConversations(unsigned long userId){
    ConversationList list;
    if(userId == 0){
        return list;
    }
    QString query = QString("SELECT DISTINCT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\""
                            " OR \"%5\" LIKE \"%6\"")/*avoiding duplicates in this case*/
            .arg(conversationIdentifierColumn).arg(tablename())
            .arg(senderColumn).arg(QString::number(userId))
            .arg(receiverColumn).arg(QString::number(userId));

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);
    if(!sqlQuery->isActive()){
        qWarning() << "MessageModel::getConversations | Error , sqlQuery is not active";
        return list;
    }

    int conversationIdentifierFieldNo = sqlQuery->record().indexOf(conversationIdentifierColumn);\
    while (sqlQuery->next()) {
        const QString& conversationIdentifier = sqlQuery->value(conversationIdentifierFieldNo).toString();\
        list.append(Conversation(conversationIdentifier));
    }
    return list;
}

bool MessagesModel::conversationExist(const Conversation &conversation){
    if(const_cast<Conversation &>(conversation).isValid()){
        QString record = getRecord(conversationIdentifierColumn,conversationIdentifierColumn,conversation.conversationIdentifier_,"MessageModel::conversationExist(const Conversation &conversation)");
        return (record == conversation.conversationIdentifier_);
    }else{
        return false;
    }
}

SentMessages MessagesModel::getSentMessages(const Message &start, int maxNoOfMessages
                                            , unsigned long userId,bool includeStartMessage
                                            , const Sort::Order sortOrder)
{
    return getSentMessages(start.messageIdentifier_,userId,maxNoOfMessages,includeStartMessage,sortOrder);

}

SentMessages MessagesModel::getSentMessages(const Conversation &conversation, unsigned long userId, int maxNoOfMessages
                                            , const Sort::Order sortOrder)
{
    auto firstMessage = getNextSentMessage(QString("%1:0").arg(conversation.conversationIdentifier_),userId);
    return getSentMessages(firstMessage,maxNoOfMessages,userId,true,sortOrder);
}


ReceivedMessages MessagesModel::getReceivedMessages(const Message &start, int maxNoOfMessages
                                                    , unsigned long userId, bool includeStartMessage, const Sort::Order sortOrder){
    return getReceivedMessages(start.messageIdentifier_,userId,maxNoOfMessages,includeStartMessage,sortOrder);
}

ReceivedMessages MessagesModel::getReceivedMessages(const Conversation &conversation, unsigned long userId
                                                    , int maxNoOfMessages, const Sort::Order sortOrder)
{
    auto firstMessage = getNextReceivedMessage(QString("%1:0").arg(conversation.conversationIdentifier_),userId);
    return getReceivedMessages(firstMessage,maxNoOfMessages,userId,true,sortOrder);
}


Messages MessagesModel::getMessages(const QString &startMessageIdentifier, unsigned long userId, int maxNoOfMessages
                                    , bool includeStartMessage, const Sort::Order sortOrder)
{

    if(maxNoOfMessages <= 0){
        maxNoOfMessages = 5;
    }

    if(maxNoOfMessages > 20){//must not execeed 20 messages
        maxNoOfMessages = 20;
    }

    Messages messages;
    (sortOrder == Sort::Order::ASC) ? messages.setMessageSortOrder(Sort::Order::ASC)
                                    : messages.setMessageSortOrder(Sort::Order::DESC);

    Messages tmp;
    int count = 0;
    QString startFrom = startMessageIdentifier;

    auto message = getMessage(startMessageIdentifier);

    if((!message.isEmpty())
            && (includeStartMessage)
            && (message.messageIdentifier() == startMessageIdentifier)){

        if(message.receiver() == userId){
            if(!message.deletedByReceiver()){
                startFrom = message.messageIdentifier();
                tmp.append(message);

                count++;
            }
        }
        if(message.sender() == userId){
            if(!message.deletedBySender()){
                startFrom = message.messageIdentifier();
                tmp.append(message);

                count++;
            }
        }
    }

    for(; count < maxNoOfMessages; count++){
        auto message = getNextMessage(startFrom,userId);
        if(message.isEmpty()){
            break;
        }
        tmp.append(message);

        startFrom = message.messageIdentifier();
    }

    if(sortOrder == Sort::Order::ASC){
        tmp.setMessageSortOrder(Sort::Order::ASC);
        return tmp;
    }else{
        foreach (auto message, tmp) {
            messages.prepend(message);
        }
        return messages;
    }
}

UnReadMessages MessagesModel::getUnReadMessages(const QString &startMessageIdentifier, unsigned long receiver, int maxNoOfMessages
                                                , bool includeStartMessage, const Sort::Order sortOrder)
{
    if(maxNoOfMessages <= 0){
        maxNoOfMessages = 5;
    }

    if(maxNoOfMessages > 20){//must not execeed 20 messages
        maxNoOfMessages = 20;
    }

    UnReadMessages unReadMessages;
    (sortOrder == Sort::Order::ASC) ? unReadMessages.setMessageSortOrder(Sort::Order::ASC)
                                    : unReadMessages.setMessageSortOrder(Sort::Order::DESC);

    if(messageExists(startMessageIdentifier)){

        const unsigned long long startTimestamp = getMessageTimestamp(startMessageIdentifier);
        const QString& convesationIdentifier = generateConversationIdentifier(startMessageIdentifier);
        const QString query = QString("SELECT * FROM \"%1\""
                                      " WHERE \"%2\" %3 \"%4\""
                                      " %5"
                                      " AND \"%6\" = \"%7\""
                                      " AND \"%8\" = \"%9\""
                                      " AND \"%10\" = \"%11\""
                                      " AND \"%12\" = \"%13\""
                                      " ORDER BY %14 ASC"
                                      " LIMIT %15;")
                .arg(tablename())
                .arg(timestampColumn).arg(((includeStartMessage)? "=":">")).arg(startTimestamp)
                .arg(((includeStartMessage)? QString("OR \"%1\" > \"%2\"").arg(timestampColumn).arg(startTimestamp):""))
                .arg(conversationIdentifierColumn).arg(convesationIdentifier)
                .arg(receiverColumn).arg(QString::number(receiver))
                .arg(receiverReadStatusColumn).arg(QString::number(0))
                .arg(receiverDeletionStatusColumn).arg(QString::number(0))//not deleted
                .arg(timestampColumn)
                .arg(maxNoOfMessages);

        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        //after exec
        INIT_MESSAGE_MODEL_FIELDS

                auto takeValue = [&](){//capture by reference

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES

                    if((msg.receiver() == receiver)
                       && (!msg.readByReceiver())
                       && (!msg.deletedByReceiver())){
                unReadMessages.append(msg);
            }
        };

        if(sqlQuery->isActive()){
            if(sortOrder == Sort::Order::ASC){
                while (sqlQuery->next()) {
                    takeValue();//take that next value
                }
            }else{
                if(sqlQuery->last()){
                    takeValue();//take that last value
                    while (sqlQuery->previous()) {
                        takeValue();//take that previous value
                    }
                }
            }
        }else{
            qWarning() << "MessageModel::getUnReadMessages | Error sqlQuery is not Active";
        }
    }
    return unReadMessages;
}


ReadMessages MessagesModel::getReadMessages(const QString &startMessageIdentifier, unsigned long receiver, int maxNoOfMessages
                                            , bool includeStartMessage, const Sort::Order sortOrder)
{
    if(maxNoOfMessages <= 0){
        maxNoOfMessages = 5;
    }

    if(maxNoOfMessages > 20){//must not execeed 20 messages
        maxNoOfMessages = 20;
    }

    ReadMessages readMessages;
    (sortOrder == Sort::Order::ASC) ? readMessages.setMessageSortOrder(Sort::Order::ASC)
                                    : readMessages.setMessageSortOrder(Sort::Order::DESC);

    if(messageExists(startMessageIdentifier)){

        const unsigned long long startTimestamp = getMessageTimestamp(startMessageIdentifier);
        const QString& convesationIdentifier = generateConversationIdentifier(startMessageIdentifier);
        const QString query = QString("SELECT * FROM \"%1\""
                                      " WHERE \"%2\" %3 \"%4\""
                                      " %5"
                                      " AND \"%6\" = \"%7\""
                                      " AND \"%8\" = \"%9\""
                                      " AND \"%10\" = \"%11\""
                                      " AND \"%12\" = \"%13\""
                                      " ORDER BY %14 ASC"
                                      " LIMIT %15;")
                .arg(tablename())
                .arg(timestampColumn).arg(((includeStartMessage)? "=":">")).arg(startTimestamp)
                .arg(((includeStartMessage)? QString("OR \"%1\" > \"%2\"").arg(timestampColumn).arg(startTimestamp):""))
                .arg(conversationIdentifierColumn).arg(convesationIdentifier)
                .arg(receiverColumn).arg(QString::number(receiver))
                .arg(receiverReadStatusColumn).arg(QString::number(1))
                .arg(receiverDeletionStatusColumn).arg(QString::number(0))//not deleted
                .arg(timestampColumn)
                .arg(maxNoOfMessages);

        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        //after exec
        INIT_MESSAGE_MODEL_FIELDS

                auto takeValue = [&](){//capture by reference
            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if((msg.receiver() == receiver)
                       && (msg.readByReceiver())
                       && (!msg.deletedByReceiver())){
                readMessages.append(msg);
            }
        };

        if(sqlQuery->isActive()){
            if(sortOrder == Sort::Order::ASC){
                while (sqlQuery->next()) {
                    takeValue();//take that next value
                }
            }else{
                if(sqlQuery->last()){
                    takeValue();//take that last value
                    while (sqlQuery->previous()) {
                        takeValue();//take that previous value
                    }
                }
            }
        }else{
            qWarning() << "MessageModel::getReadMessages | Error sqlQuery is not Active";
        }
    }
    return readMessages;
}

ReceivedMessages MessagesModel::getReceivedMessages(const QString &startMessageIdentifier, unsigned long receiver, int maxNoOfMessages
                                                    , bool includeStartMessage, const Sort::Order sortOrder)
{
    if(maxNoOfMessages <= 0){
        maxNoOfMessages = 5;
    }

    if(maxNoOfMessages > 20){//must not execeed 20 messages
        maxNoOfMessages = 20;
    }

    ReceivedMessages receivedMessages;
    (sortOrder == Sort::Order::ASC) ? receivedMessages.setMessageSortOrder(Sort::Order::ASC)
                                    : receivedMessages.setMessageSortOrder(Sort::Order::DESC);

    if(messageExists(startMessageIdentifier)){

        const unsigned long long startTimestamp = getMessageTimestamp(startMessageIdentifier);
        const QString& convesationIdentifier = generateConversationIdentifier(startMessageIdentifier);
        const QString query = QString("SELECT * FROM \"%1\""
                                      " WHERE \"%2\" %3 \"%4\""
                                      " %5"
                                      " AND \"%6\" = \"%7\""
                                      " AND \"%8\" = \"%9\""
                                      " AND \"%10\" = \"%11\""
                                      " ORDER BY %12 ASC"
                                      " LIMIT %13;")
                .arg(tablename())
                .arg(timestampColumn).arg(((includeStartMessage)? "=":">")).arg(startTimestamp)
                .arg(((includeStartMessage)? QString("OR \"%1\" > \"%2\"").arg(timestampColumn).arg(startTimestamp):""))
                .arg(conversationIdentifierColumn).arg(convesationIdentifier)
                .arg(receiverColumn).arg(QString::number(receiver))
                .arg(receiverDeletionStatusColumn).arg(QString::number(0))//not mark as deleted
                .arg(timestampColumn)
                .arg(maxNoOfMessages);

        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        //after exec
        INIT_MESSAGE_MODEL_FIELDS

                auto takeValue = [&](){//capture by reference
            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES

                    if((msg.receiver() == receiver)
                       && (!msg.deletedByReceiver())){ //bcoz we re not sure if a start message i
                receivedMessages.append(msg);
            }
        };

        if(sqlQuery->isActive()){
            if(sortOrder == Sort::Order::ASC){
                while (sqlQuery->next()) {
                    takeValue();//take that next value
                }
            }else{
                if(sqlQuery->last()){
                    takeValue();//take that last value
                    while (sqlQuery->previous()) {
                        takeValue();//take that previous value
                    }
                }
            }
        }else{
            qWarning() << "MessageModel::getReceivedMessages | Error sqlQuery is not Active";
        }
    }

    return receivedMessages;
}

SentMessages MessagesModel::getSentMessages(const QString &startMessageIdentifier, unsigned long sender, int maxNoOfMessages
                                            , bool includeStartMessage, const Sort::Order sortOrder)
{
    if(maxNoOfMessages <= 0){
        maxNoOfMessages = 5;
    }

    if(maxNoOfMessages > 20){//must not execeed 20 messages
        maxNoOfMessages = 20;
    }

    SentMessages sentMessages;
    (sortOrder == Sort::Order::ASC) ? sentMessages.setMessageSortOrder(Sort::Order::ASC)
                                    : sentMessages.setMessageSortOrder(Sort::Order::DESC);

    if(messageExists(startMessageIdentifier)){

        const unsigned long long startTimestamp = getMessageTimestamp(startMessageIdentifier);
        const QString& convesationIdentifier = generateConversationIdentifier(startMessageIdentifier);
        const QString query = QString("SELECT * FROM \"%1\""
                                      " WHERE \"%2\" %3 \"%4\""
                                      " %5"
                                      " AND \"%6\" = \"%7\""
                                      " AND \"%8\" = \"%9\""
                                      " AND \"%10\" = \"%11\""
                                      " ORDER BY %12 ASC"
                                      " LIMIT %13;")
                .arg(tablename())
                .arg(timestampColumn).arg(((includeStartMessage)? "=":">")).arg(startTimestamp)
                .arg(((includeStartMessage)? QString("OR \"%1\" > \"%2\"").arg(timestampColumn).arg(startTimestamp):""))
                .arg(conversationIdentifierColumn).arg(convesationIdentifier)
                .arg(senderColumn).arg(QString::number(sender))
                .arg(senderDeletionStatusColumn).arg(QString::number(0))//marked as not deleted
                .arg(timestampColumn)
                .arg(maxNoOfMessages);

        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        //after exec
        INIT_MESSAGE_MODEL_FIELDS

                auto takeValue = [&](){//capture by reference
            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if((msg.sender() == sender)
                       && (!msg.deletedBySender())){
                sentMessages.append(msg);
            }
        };

        if(sqlQuery->isActive()){
            if(sortOrder == Sort::Order::ASC){
                while (sqlQuery->next()) {
                    takeValue();//take that next value
                }
            }else{
                if(sqlQuery->last()){
                    takeValue();//take that last value
                    while (sqlQuery->previous()) {
                        takeValue();//take that previous value
                    }
                }
            }
        }else{
            qWarning() << "MessageModel::getSentMessages | Error sqlQuery is not Active";
        }
    }
    return sentMessages;
}


int MessagesModel::getMessageRowId(const Message &message)
{
    return getRecord(rowidColumn,messageIdentifierColumn,message.messageIdentifier_,"MessageModel::getMessageRowId").toInt();
}

QString MessagesModel::getMessageIdentifier(const int rowId)
{
    return getRecord(messageIdentifierColumn,rowidColumn,QString::number(rowId),"MessageModel::getMessageIdentifier");
}


Message MessagesModel::getNextSentMessage(const QString& currentMessageIndentifier,unsigned long sender){

    Message currentMessage; currentMessage.messageIdentifier_ = currentMessageIndentifier;
    const auto conversationIdentifier = currentMessage.getConversation().conversationIdentifier();//getConversation() will initilize conversationIdentifier_ if valid messageIdentifier is set, and conversationIdentifier_ is not

    if(!conversationExist(conversationIdentifier)){
        return Message();
    }

    auto arry = currentMessage.messageIdentifier_.split(':');
    if(arry.length() != 3){
        return Message();
    }

    const QString& lastMessageIdentifier = getLastMessageIdentifier(conversationIdentifier);
    if(lastMessageIdentifier.isEmpty()){
        return Message();
    }
    const unsigned long id1 =  arry.at(0).toULong();
    const unsigned long id2 =  arry.at(1).toULong();

    QString nextMessageIdentifier;
    QString query;

    int messageNo = arry.at(2).toInt();
    auto updateIdentifier = [&]() {
        messageNo++;
        nextMessageIdentifier = QString("%1:%2:%3").arg(id1).arg(id2).arg(messageNo);
        query = QString("SELECT * FROM \"%1\""
                        " WHERE \"%2\" = \"%3\"")
                .arg(tablename())
                .arg(messageIdentifierColumn).arg(nextMessageIdentifier);
    };

    auto sqlQuery = m_db->getSqlQuery();
    while(nextMessageIdentifier != lastMessageIdentifier ){
        updateIdentifier();

        sqlQuery->exec(query);

        INIT_MESSAGE_MODEL_FIELDS

                if(sqlQuery->isActive() && sqlQuery->first()){

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if((msg.sender() == sender) && (!msg.deletedBySender())){
                return msg;
            }
        }

        sqlQuery->clear();
    }

    return Message();
}


Message MessagesModel::getNextReceivedMessage(const QString& currentMessageIndentifier,unsigned long receiver){

    Message currentMessage; currentMessage.messageIdentifier_ = currentMessageIndentifier;
    const auto conversationIdentifier = currentMessage.getConversation().conversationIdentifier();//getConversation() will initilize conversationIdentifier_ if valid messageIdentifier is set, and conversationIdentifier_ is not

    if(!conversationExist(conversationIdentifier)){
        return Message();
    }

    auto arry = currentMessage.messageIdentifier_.split(':');
    if(arry.length() != 3){
        return Message();
    }

    const QString& lastMessageIdentifier = getLastMessageIdentifier(conversationIdentifier);
    if(lastMessageIdentifier.isEmpty()){
        return Message();
    }
    const unsigned long id1 =  arry.at(0).toULong();
    const unsigned long id2 =  arry.at(1).toULong();

    QString nextMessageIdentifier;
    QString query;

    int messageNo = arry.at(2).toInt();
    auto updateIdentifier = [&]() {
        messageNo++;
        nextMessageIdentifier = QString("%1:%2:%3").arg(id1).arg(id2).arg(messageNo);
        query = QString("SELECT * FROM \"%1\""
                        " WHERE \"%2\" = \"%3\"")
                .arg(tablename())
                .arg(messageIdentifierColumn).arg(nextMessageIdentifier);
    };

    auto sqlQuery = m_db->getSqlQuery();
    while(nextMessageIdentifier != lastMessageIdentifier ){
        updateIdentifier();

        sqlQuery->exec(query);

        INIT_MESSAGE_MODEL_FIELDS

                if(sqlQuery->isActive() && sqlQuery->first()){

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES

                    if((msg.receiver() == receiver) && (!msg.deletedByReceiver())){
                return msg;
            }
        }

        sqlQuery->clear();
    }

    return Message();
}

Message MessagesModel::getNextReadMessage(const QString& currentMessageIndentifier,unsigned long receiver){

    Message currentMessage; currentMessage.messageIdentifier_ = currentMessageIndentifier;
    const auto conversationIdentifier = currentMessage.getConversation().conversationIdentifier();//getConversation() will initilize conversationIdentifier_ if valid messageIdentifier is set, and conversationIdentifier_ is not

    if(!conversationExist(conversationIdentifier)){
        return Message();
    }

    auto arry = currentMessage.messageIdentifier_.split(':');
    if(arry.length() != 3){
        return Message();
    }

    const QString& lastMessageIdentifier = getLastMessageIdentifier(conversationIdentifier);
    if(lastMessageIdentifier.isEmpty()){
        return Message();
    }
    const unsigned long id1 =  arry.at(0).toULong();
    const unsigned long id2 =  arry.at(1).toULong();

    QString nextMessageIdentifier;
    QString query;

    int messageNo = arry.at(2).toInt();
    auto updateIdentifier = [&]() {
        messageNo++;
        nextMessageIdentifier = QString("%1:%2:%3").arg(id1).arg(id2).arg(messageNo);
        query = QString("SELECT * FROM \"%1\""
                        " WHERE \"%2\" = \"%3\"")
                .arg(tablename())
                .arg(messageIdentifierColumn).arg(nextMessageIdentifier);
    };

    auto sqlQuery = m_db->getSqlQuery();
    while(nextMessageIdentifier != lastMessageIdentifier ){
        updateIdentifier();

        sqlQuery->exec(query);

        INIT_MESSAGE_MODEL_FIELDS

                if(sqlQuery->isActive() && sqlQuery->first()){

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if((msg.receiver() == receiver)
                       && (msg.readByReceiver())
                       && (!msg.deletedByReceiver())){
                return msg;
            }
        }

        sqlQuery->clear();
    }

    return Message();

}

Message MessagesModel::getNextMessage(const QString& currentMessageIndentifier,unsigned long userId){

    Message currentMessage; currentMessage.messageIdentifier_ = currentMessageIndentifier;
    const auto conversationIdentifier = currentMessage.getConversation().conversationIdentifier();//getConversation() will initilize conversationIdentifier_ if valid messageIdentifier is set, and conversationIdentifier_ is not

    if(!conversationExist(conversationIdentifier)){
        return Message();
    }

    auto arry = currentMessage.messageIdentifier_.split(':');
    if(arry.length() != 3){
        return Message();
    }

    const QString& lastMessageIdentifier = getLastMessageIdentifier(conversationIdentifier);
    if(lastMessageIdentifier.isEmpty()){
        return Message();
    }
    const unsigned long id1 =  arry.at(0).toULong();
    const unsigned long id2 =  arry.at(1).toULong();

    QString nextMessageIdentifier;
    QString query;

    int messageNo = arry.at(2).toInt();
    auto updateIdentifier = [&]() {
        messageNo++;
        nextMessageIdentifier = QString("%1:%2:%3").arg(id1).arg(id2).arg(messageNo);
        query = QString("SELECT * FROM \"%1\""
                        " WHERE \"%2\" = \"%3\"")
                .arg(tablename())
                .arg(messageIdentifierColumn).arg(nextMessageIdentifier);
    };

    auto sqlQuery = m_db->getSqlQuery();
    while(nextMessageIdentifier != lastMessageIdentifier ){
        updateIdentifier();

        sqlQuery->exec(query);

        INIT_MESSAGE_MODEL_FIELDS

                if(sqlQuery->isActive() && sqlQuery->first()){

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if(msg.receiver() == userId){
                if(!msg.deletedByReceiver()){
                    return msg;
                }

            }
            if(msg.sender() == userId){
                if(!msg.deletedBySender()){
                    return msg;
                }

            }


        }

        sqlQuery->clear();
    }

    return Message();

}

int MessagesModel::getMessageCount(Conversation conversation, unsigned long userId, MessageType messageType){
    QStringList whereColumns; whereColumns << conversationIdentifierColumn;
    QStringList likeValues; likeValues << conversation.conversationIdentifier();


    switch (messageType) {

    case  MessageType::Sent:{
        whereColumns << senderColumn << senderDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0);
        break;
    }
    case  MessageType::Received:{
        whereColumns << receiverColumn << receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0);
        break;
    }
    case  MessageType::SentOrReceived:{
        QStringList selectColumns; selectColumns <<"*";
        QString query = generateSelectQuery(selectColumns,whereColumns,likeValues,nullptr,0);
        int count = 0;

        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);

        if(sqlQuery->isActive()){

            INIT_MESSAGE_MODEL_FIELDS

                    while(sqlQuery->next()){

                CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES

                        if(msg.receiver() == userId){
                    if(!msg.deletedByReceiver()){
                        count++;
                    }
                }else{
                    if(!msg.deletedBySender()){
                        count++;
                    }
                }
            }
        }else{
            return 0;
        }

        return count;
    }
    case  MessageType::Read:{
        whereColumns << receiverColumn << receiverReadStatusColumn << receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(1) << QString::number(0);
        break;
    }
    case  MessageType::UnRead:{
        whereColumns << receiverColumn << receiverReadStatusColumn << receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0) << QString::number(0);
        break;
    }
    }

    return getRecordCount(whereColumns,likeValues);
}

Message MessagesModel::getLastMessage(Conversation conversation, unsigned long userId, MessageType messageType){
    QStringList selectColumns; selectColumns << "*";
    QStringList whereColumns; whereColumns << conversationIdentifierColumn;
    QStringList likeValues; likeValues << conversation.conversationIdentifier();

    switch (messageType) {

    case  MessageType::Sent:{
        whereColumns << senderColumn << senderDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0);
        break;
    }
    case  MessageType::Received:{
        whereColumns << receiverColumn << receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0);
        break;
    }
    case  MessageType::SentOrReceived:{
        break;
    }
    case  MessageType::Read:{
        whereColumns << receiverColumn << receiverReadStatusColumn << receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(1) << QString::number(0);
        break;
    }
    case  MessageType::UnRead:{
        whereColumns << receiverColumn << receiverReadStatusColumn<< receiverDeletionStatusColumn;
        likeValues << QString::number(userId) << QString::number(0) << QString::number(0);
        break;
    }
    }

    QString query;
    if(messageType == MessageType::SentOrReceived){
        Message message;
        query = QString("SELECT * FROM \"%1\" WHERE \"%2\" = \"%3\" AND \"%4\" = \"%5\" ORDER BY %8 DESC LIMIT 1")
                .arg(tablename())
                .arg(conversationIdentifierColumn).arg(conversation.conversationIdentifier())
                .arg(receiverDeletionStatusColumn).arg(0)
                .arg(timestampColumn);
        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        INIT_MESSAGE_MODEL_FIELDS
                if(sqlQuery->isActive() && sqlQuery->first()){
            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    message =  msg;
        }

        if(message.isEmpty()){
            query = QString("SELECT * FROM \"%1\" WHERE \"%2\" = \"%3\" AND \"%4\" = \"%5\" ORDER BY %8 DESC LIMIT 1")
                    .arg(tablename())
                    .arg(conversationIdentifierColumn).arg(conversation.conversationIdentifier())
                    .arg(senderDeletionStatusColumn).arg(0)
                    .arg(timestampColumn);

            auto sqlQuery = m_db->getSqlQuery();
            sqlQuery->exec(query);
            INIT_MESSAGE_MODEL_FIELDS

                    if(sqlQuery->isActive() && sqlQuery->first()){
                CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                        message =  msg;
            }
        }
        return message;
    }else{
        query = generateSelectQuery(selectColumns,whereColumns,likeValues,timestampColumn,Sort::Order::DESC,1);
        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        INIT_MESSAGE_MODEL_FIELDS
                if(sqlQuery->isActive() && sqlQuery->first()){
            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    return msg;
        }
    }

    return Message();
}


Message MessagesModel::getNextUnReadMessage(const QString& currentMessageIndentifier,unsigned long receiver){

    Message currentMessage; currentMessage.messageIdentifier_ = currentMessageIndentifier;
    const auto conversationIdentifier = currentMessage.getConversation().conversationIdentifier();//getConversation() will initilize conversationIdentifier_ if valid messageIdentifier is set, and conversationIdentifier_ is not

    if(!conversationExist(conversationIdentifier)){
        return Message();
    }

    auto arry = currentMessage.messageIdentifier_.split(':');
    if(arry.length() != 3){
        return Message();
    }

    const QString& lastMessageIdentifier = getLastMessageIdentifier(conversationIdentifier);
    if(lastMessageIdentifier.isEmpty()){
        return Message();
    }
    const unsigned long id1 =  arry.at(0).toULong();
    const unsigned long id2 =  arry.at(1).toULong();

    QString nextMessageIdentifier;
    QString query;

    int messageNo = arry.at(2).toInt();
    auto updateIdentifier = [&]() {
        messageNo++;
        nextMessageIdentifier = QString("%1:%2:%3").arg(id1).arg(id2).arg(messageNo);
        query = QString("SELECT * FROM \"%1\""
                        " WHERE \"%2\" = \"%3\"")
                .arg(tablename())
                .arg(messageIdentifierColumn).arg(nextMessageIdentifier);
    };

    auto sqlQuery = m_db->getSqlQuery();
    while(nextMessageIdentifier != lastMessageIdentifier ){
        updateIdentifier();

        sqlQuery->exec(query);

        INIT_MESSAGE_MODEL_FIELDS

                if(sqlQuery->isActive() && sqlQuery->first()){

            CONTRUCT_MESSAGE_FROM_MESSAGE_MODEL_VALUES
                    if((msg.receiver() == receiver)
                       && (!msg.readByReceiver())
                       && (!msg.deletedByReceiver())){
                return msg;
            }
        }

        sqlQuery->clear();
    }

    return Message();

}



QString MessagesModel::generateMessageIdentifier(unsigned long sender, unsigned long receiver)
{
    const QString conversationIdentifier(generateConversationIdentifier(sender,receiver));
    const int count = getMessageCount(conversationIdentifier);
    if(count == 0){
        return QString("%1:%2").arg(conversationIdentifier).arg(1);
    }else{
        auto arry = getLastMessageIdentifier(conversationIdentifier).split(':');
        if(arry.length() == 3){
            const int msgNo = arry.at(2).toInt() + 1;
            return QString("%1:%2").arg(conversationIdentifier).arg(msgNo);
        }else{
            qWarning() << "MessageModel::generateMessageIdentifier |Critical error invalid LastMessageIdentifier";
            return QString("%1:%2").arg(conversationIdentifier).arg(count+1);
        }
    }
}

QString MessagesModel::getLastMessageIdentifier(const QString& convesationIdentifier){
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" LIKE \"%4\""
                            " ORDER BY \"%5\" DESC LIMIT 1;")
            .arg(messageIdentifierColumn).arg(tablename())
            .arg(conversationIdentifierColumn).arg(convesationIdentifier)
            .arg(timestampColumn);

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        return sqlQuery->value(0).toString();
    }else{
        return "";
    }
}

bool MessagesModel::isValidMessageIdentifier(const QString &messageIdentifier){
    return Message::isValidMessageIdentifier(messageIdentifier);
}

QString MessagesModel::getMessageIdentifierOfNextMessage(const QString& messageIdentifier){

    const unsigned long long timestamp = getMessageTimestamp(messageIdentifier);

    const QString& convesationIdentifier = generateConversationIdentifier(messageIdentifier);
    QString query = QString("SELECT %1 FROM \"%2\""
                            " WHERE \"%3\" > \"%4\""
                            " AND \"%5\" = \"%6\"  LIMIT 1;")
            .arg(messageIdentifierColumn).arg(tablename())
            .arg(timestampColumn).arg(timestamp)
            .arg(conversationIdentifierColumn).arg(convesationIdentifier);

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        return sqlQuery->value(0).toString();
    }else{
        return "";
    }
}
void MessagesModel::createTable()
{

    CreateTableStatement createTableStatement(tablename());
    createTableStatement.addColumn(messageIdentifierColumn,"varchar(255)",true,true,true)
            .addColumn(conversationIdentifierColumn,"varchar(255)",false,true,false)
            .addColumn(messageColumn,"TEXT",false,true,false)
            .addColumn(senderColumn,"int(11)",false,true,false)
            .addColumn(receiverColumn,"int(11)",false,true,false)
            .addColumn(timestampColumn,"int(11)",false,true,false)
            .addColumn(senderDeletionStatusColumn,"int(1)",false,true,false)
            .addColumn(receiverDeletionStatusColumn,"int(1)",false,true,false)
            .addColumn(receiverReadStatusColumn,"int(1)",false,true,false);

    m_db->excuteSqlQuery(createTableStatement.toString(),"message table created Sucessfull","Failed to create message table");
}

