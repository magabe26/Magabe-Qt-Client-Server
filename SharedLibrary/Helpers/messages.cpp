/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "messages.h"
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>

/** -------------------Message------------------
 * @brief Message::Message
 * @param other
 */

Message::Message()
    :sender_ (0)
    ,receiver_(0)
    ,timestamp_(0)
    ,senderDeletionStatus_(false)
    ,receiverDeletionStatus_(false)
    ,receiverReadStatus_(false)
    ,searchBy_(SearchBy::All)
{}

Message::Message(const Message &other):searchBy_(SearchBy::All){
    this->messageIdentifier_ = other.messageIdentifier_;
    this->conversationIdentifier_ = other.conversationIdentifier_;
    this->encodedText_ = other.encodedText_;
    this->sender_ = other.sender_;
    this->receiver_ = other.receiver_;
    this->timestamp_ = other.timestamp_;
    this->senderDeletionStatus_ = other.senderDeletionStatus_;
    this->receiverDeletionStatus_ = other.receiverDeletionStatus_;
    this->receiverReadStatus_ = other.receiverReadStatus_;

}

Message::Message(Message &&other):searchBy_(SearchBy::All){
    this->messageIdentifier_ = other.messageIdentifier_;
    this->conversationIdentifier_ = other.conversationIdentifier_;
    this->encodedText_ = other.encodedText_;
    this->sender_ = other.sender_;
    this->receiver_ = other.receiver_;
    this->timestamp_ = other.timestamp_;
    this->senderDeletionStatus_ = other.senderDeletionStatus_;
    this->receiverDeletionStatus_ = other.receiverDeletionStatus_;
    this->receiverReadStatus_ = other.receiverReadStatus_;

}

Message &Message::operator =(const Message &other){
    this->messageIdentifier_ = other.messageIdentifier_;
    this->conversationIdentifier_ = other.conversationIdentifier_;
    this->encodedText_ = other.encodedText_;
    this->sender_ = other.sender_;
    this->receiver_ = other.receiver_;
    this->timestamp_ = other.timestamp_;
    this->senderDeletionStatus_ = other.senderDeletionStatus_;
    this->receiverDeletionStatus_ = other.receiverDeletionStatus_;
    this->receiverReadStatus_ = other.receiverReadStatus_;

    return *this;
}

QString Message::messageIdentifier() const
{
    return messageIdentifier_;
}

QString Message::decodedText() const
{
    return QByteArray::fromHex(encodedText_.toLatin1()).data();
}

QString Message::encodedText() const{
    return encodedText_;
}

unsigned long Message::sender() const
{
    return sender_;
}

unsigned long Message::receiver() const
{
    return receiver_;
}

unsigned long long Message::timestamp() const
{
    return timestamp_;
}

bool Message::readByReceiver() const
{
    return receiverReadStatus_;
}

bool Message::deletedBySender() const
{
    return senderDeletionStatus_;
}

bool Message::deletedByReceiver() const
{
    return receiverDeletionStatus_;
}

QString Message::toJson(QJsonDocument::JsonFormat format){
    return QJsonDocument::fromJson(QString("{"
                                           "\"messageIdentifier\" : \"%1\","
                                           "\"conversationIdentifier\" : \"%2\","
                                           "\"encodedText\" : \"%3\","
                                           "\"sender\" : \"%4\","
                                           "\"receiver\" : \"%5\","
                                           "\"timestamp\" : \"%6\","
                                           "\"readByReceiver\" : \"%7\","
                                           "\"deletedBySender\" : \"%8\","
                                           "\"deletedByReceiver\" : \"%9\""
                                           "}")
                                   .arg(messageIdentifier_)
                                   .arg(conversationIdentifier_)
                                   .arg(encodedText_)
                                   .arg(sender_)
                                   .arg(receiver_)
                                   .arg(timestamp_)
                                   .arg(QString::number((receiverReadStatus_)? 1 : 0))
                                   .arg(QString::number((senderDeletionStatus_)? 1 : 0))
                                   .arg(QString::number((receiverDeletionStatus_)? 1: 0)).toLatin1()).toJson(format);
}

Message Message::fromJson(const QString &json){
    Message msg;

    auto jObject = QJsonDocument::fromJson(json.toLatin1()).object();
    if(jObject.isEmpty()){
        return msg;
    }
    const QString& messageIdentifier = jObject.value("messageIdentifier").toString();
    const QString& conversationIdentifier = jObject.value("conversationIdentifier").toString();
    const QString& message = jObject.value("encodedText").toString();
    const unsigned long sender = jObject.value("sender").toString().toULong();
    const unsigned long receiver = jObject.value("receiver").toString().toULong();
    const unsigned long long timestamp = jObject.value("timestamp").toString().toULongLong();
    const int receiverReadStatus = jObject.value("readByReceiver").toString().toInt();//NOTE GOING toInt() diectly geive invalid values
    const int senderDeletionStatus = jObject.value("deletedBySender").toString().toInt();
    const int receiverDeletionStatus = jObject.value("deletedByReceiver").toString().toInt();

    msg.messageIdentifier_ = messageIdentifier;
    msg.conversationIdentifier_ = conversationIdentifier;
    msg.encodedText_ = message;
    msg.sender_ = sender;
    msg.receiver_ = receiver;
    msg.timestamp_ = timestamp;
    msg.receiverReadStatus_ = ((receiverReadStatus == 1) ? true : false);
    msg.senderDeletionStatus_ = ((senderDeletionStatus == 1) ? true : false);
    msg.receiverDeletionStatus_ = ((receiverDeletionStatus == 1) ? true : false);

    msg.clearIfIsInvalid();

    return msg;
}

bool Message::operator ==(const Message &other){

    switch (other.searchBy_) {

    case SearchBy::MessageIdentifier:
        return(this->messageIdentifier_ == other.messageIdentifier_);

    case SearchBy::ConversationIdentifier:
        return(this->conversationIdentifier_ == other.conversationIdentifier_);

    case SearchBy::Message:
        return(this->encodedText_ == other.encodedText_);

    case SearchBy::Sender:
        return(this->sender_ == other.sender_);

    case SearchBy::Receiver:
        return(this->receiver_ == other.receiver_);

    case SearchBy::Timestamp:
        return(this->timestamp_ == other.timestamp_);

    case SearchBy::ReceiverReadStatus:
        return(this->receiverReadStatus_ == other.receiverReadStatus_);

    case SearchBy::SenderAndSenderDeletionStatus:
        return((this->sender_ == other.sender_)
               && (this->senderDeletionStatus_ == other.senderDeletionStatus_));

    case SearchBy::ReceiverAndReceiverDeletionStatus: return(
                    (this->receiver_ == other.receiver_)
                    && (this->receiverDeletionStatus_ == other.receiverDeletionStatus_));

    case SearchBy::All:
    default:
        return(
                    (this->messageIdentifier_ == other.messageIdentifier_)
                    && (this->conversationIdentifier_ == other.conversationIdentifier_)
                    && (this->encodedText_ == other.encodedText_)
                    && (this->sender_ == other.sender_)
                    && (this->receiver_ == other.receiver_)
                    && (this->timestamp_ == other.timestamp_)
                    && (this->receiverReadStatus_ == other.receiverReadStatus_)
                    && (this->senderDeletionStatus_ == other.senderDeletionStatus_)
                    && (this->receiverDeletionStatus_ == other.receiverDeletionStatus_)
                    );


    }
}

bool Message::operator !=(const Message &other){

    switch (other.searchBy_) {

    case SearchBy::MessageIdentifier:
        return(this->messageIdentifier_ != other.messageIdentifier_);

    case SearchBy::ConversationIdentifier:
        return(this->conversationIdentifier_ != other.conversationIdentifier_);

    case SearchBy::Message:
        return(this->encodedText_ != other.encodedText_);

    case SearchBy::Sender:
        return(this->sender_ != other.sender_);

    case SearchBy::Receiver:
        return(this->receiver_ != other.receiver_);

    case SearchBy::Timestamp:
        return(this->timestamp_ != other.timestamp_);

    case SearchBy::ReceiverReadStatus:
        return(this->receiverReadStatus_ != other.receiverReadStatus_);

    case SearchBy::SenderAndSenderDeletionStatus:
        return((this->sender_ != other.sender_)
               || (this->senderDeletionStatus_ != other.senderDeletionStatus_));

    case SearchBy::ReceiverAndReceiverDeletionStatus: return(
                    (this->receiver_ != other.receiver_)
                    || (this->receiverDeletionStatus_ != other.receiverDeletionStatus_));

    case SearchBy::All:
    default:
        return(
                    (this->messageIdentifier_ != other.messageIdentifier_)
                    || (this->conversationIdentifier_ != other.conversationIdentifier_)
                    || (this->encodedText_ != other.encodedText_)
                    || (this->sender_ != other.sender_)
                    || (this->receiver_ != other.receiver_)
                    || (this->timestamp_ != other.timestamp_)
                    || (this->receiverReadStatus_ != other.receiverReadStatus_)
                    || (this->senderDeletionStatus_ != other.senderDeletionStatus_)
                    || (this->receiverDeletionStatus_ != other.receiverDeletionStatus_)
                    );


    }
}

bool Message::isValid(){
    return isValidMessageIdentifier(messageIdentifier_);
}

bool Message::isEmpty(){
    clearIfIsInvalid();

    return ((messageIdentifier_.isEmpty()
             && conversationIdentifier_.isEmpty()
             && encodedText_.isEmpty()
             && (sender_ == 0)
             && (receiver_ == 0)
             && (timestamp_ == 0)));
}

void Message::clearIfIsInvalid(){
    if(!isValid()){
        messageIdentifier_.clear();
        conversationIdentifier_.clear();
        encodedText_.clear();
        sender_ = 0;
        receiver_ = 0;
        timestamp_ =0;
        senderDeletionStatus_ = false;
        receiverDeletionStatus_ = false;
        receiverReadStatus_ = false;
    }
}


bool Message::isValidMessageIdentifier(const QString &messageIdentifier){
    auto arry = messageIdentifier.split(':');
    if(arry.length() == 3){
        bool ok = false;
        const unsigned long id1 = arry.at(0).toULong(&ok);//check 1
        if(!ok){
            return false;
        }

        const unsigned id2 = arry.at(1).toULong(&ok);//check 2
        if(!ok){
            return false;
        }

        if((id1 > id2) || (id1 == id2) || (id1 == 0) || (id2 == 0)){
            return false;
        }

        arry.at(2).toULong(&ok);//check 3
        if(!ok){
            return false;
        }
        return true;

    }else{
        return false;
    }

}

Conversation Message::getConversation(){
    if(conversationIdentifier_.isEmpty() && (isValidMessageIdentifier(messageIdentifier_))){
        auto arry = messageIdentifier_.split(':');
        conversationIdentifier_ = QString("%1:%2").arg(arry.at(0)).arg(arry.at(1));
    }
    return Conversation(conversationIdentifier_);
}

/** -------------MessageList----------------
 * @brief MessageList::toJson
 * @param format
 * @return
 */
MessageList::MessageList(const QString &json){
    auto doc = QJsonDocument::fromJson(json.toLatin1());
    if((!doc.isEmpty()) && (!doc.isNull())){
        auto rootJObj = doc.object();
        if(!rootJObj.isEmpty()){
            QJsonObject messagesJObj = rootJObj.value("Messages").toObject();
            QJsonObject sortOrderJObj = rootJObj.value("SortOrder").toObject();
            if(!sortOrderJObj.isEmpty()){
                QJsonArray alignedArry = sortOrderJObj.value("Alignment").toArray();
                QString sortOrderName = sortOrderJObj.value("Name").toString();
                if(!sortOrderName.isEmpty()){
                    sortOrder_ = (sortOrderName == "DESC") ? Sort::Order::DESC : Sort::Order::ASC;
                }else{
                    qWarning() << "MessageList::MessageList | Error : sortOrderName is empty ";
                }
                foreach (auto value, alignedArry) {
                    const QString& messageIdentifier = value.toString();
                    auto messageJObj = messagesJObj.value(messageIdentifier).toObject();
                    if(!messageJObj.isEmpty()){
                        const QString& json =  QJsonDocument(messageJObj).toJson();
                        append(Message::fromJson(json));
                    }else{
                        qWarning() << "MessageList::MessageList | Error : messageJObj is empty ";
                    }
                }
            }else{
                qWarning() << "MessageList::MessageList | Error : sortOrderJObj is empty ";
            }
        }
    }
}

QString MessageList::toJson(QJsonDocument::JsonFormat format){

    QJsonObject rootJObj;
    QJsonArray messageAlignment;
    QJsonObject messageListJObj;

    foreach (auto msg, *this){
        auto messageIdentifier = msg.messageIdentifier();
        messageAlignment.append(messageIdentifier);

        auto messageJObj = QJsonDocument::fromJson(msg.toJson().toLatin1()).object();
        messageListJObj.insert(messageIdentifier,messageJObj);
    }

    QJsonObject sortOrderJObj;
    sortOrderJObj.insert("Alignment",messageAlignment);
    sortOrderJObj.insert("Name",QJsonValue((sortOrder_ == Sort::Order::ASC ? "ASC" : "DESC")));

    rootJObj.insert("SortOrder",sortOrderJObj);

    rootJObj.insert("Messages",messageListJObj);
    return QJsonDocument(rootJObj).toJson(format);
}

void MessageList::setMessageSortOrder(const Sort::Order &sortOrder)
{
    sortOrder_ = sortOrder;
}

MessageList MessageList::fromJson(const QString &json){
    return MessageList(json);
}

void MessageList::sortMessages(Sort::Order sortOrder){
    if((sortOrder_ != sortOrder)){
        if(count() > 1){
            int remained = count();
            int index = 0;
            while (remained > 0) {
                auto message =  takeAt(index);
                prepend(message);
                index++;
                remained--;
            }
            sortOrder_ = sortOrder;
        }else{
            sortOrder_ = sortOrder;
        }
    }
}

Sort::Order MessageList::getSortOrder() const
{
    return sortOrder_;
}

Message MessageList::getMessage(const QString &messageIdentifier){
    Message msgSignature;
    msgSignature.messageIdentifier_ = messageIdentifier;
    msgSignature.searchBy_ = Message::SearchBy::MessageIdentifier;
    const int c = count(msgSignature);
    if(c > 1){
        qWarning() << QString("MessageList::getMessage | Error, Found more than one message with the same messageIdentifier = %1").arg(messageIdentifier);
    }
    const int index = indexOf(msgSignature);
    if(index != -1){
        return at(index);
    }else{
        return Message();
    }
}

ConversationList MessageList::getConversations(){
    ConversationList list;
    foreach (Message msg, *this) {
        auto conersation  = msg.getConversation();
        if(!list.contains(conersation)){
            list.append(conersation);
        }
    }
    return list;
}

QDebug operator <<(QDebug debug, const Message& msg){
    debug.noquote();debug.space() << const_cast<Message&>(msg).toJson();
    return debug.quote();
}

QDebug operator <<(QDebug debug, const MessageList &list){
    debug.noquote();debug.space() << const_cast<MessageList&>(list).toJson();
    return debug.quote();
}

/** -------------------Messages---------------------
 * @brief Messages::getSentMessages
 * @param sender
 * @return
 */
Messages::Messages(const QString &json):MessageList(json){}

SentMessages Messages::getSentMessages(unsigned long sender){
    Message msgSignature;
    msgSignature.sender_ = sender;
    msgSignature.searchBy_ = Message::SearchBy::Sender;
    SentMessages sentMessages;
    sentMessages.sortOrder_ = this->sortOrder_;

    for(int index = indexOf(msgSignature);
        index != -1;
        index = indexOf(msgSignature,(index+1))){
        auto message = at(index);
        if(!message.deletedBySender()){//only undeleted
            sentMessages.append(message);
        }
    }
    return sentMessages;
}

ReceivedMessages Messages::getReceivedMessages(unsigned long receiver){
    Message msgSignature;
    msgSignature.receiver_ = receiver;
    msgSignature.searchBy_ = Message::SearchBy::Receiver;
    ReceivedMessages receivedMessages;
    receivedMessages.sortOrder_ = this->sortOrder_;

    for(int index = indexOf(msgSignature);
        index != -1;
        index = indexOf(msgSignature,(index+1))){
        auto message = at(index);
        if(!message.deletedByReceiver()){//only undeleted
            receivedMessages.append(message);
        }
    }
    return receivedMessages;
}

/** ---------------ReceivedMessages-------------------------
 * @brief ReceivedMessages::getUnReadMessages
 * @return
 */
ReceivedMessages::ReceivedMessages(const QString &json):MessageList(json){}

UnReadMessages ReceivedMessages::getUnReadMessages()
{
    Message msgSignature;
    msgSignature.receiverReadStatus_ = false;
    msgSignature.searchBy_ = Message::SearchBy::ReceiverReadStatus;

    UnReadMessages unReadMessages;
    unReadMessages.sortOrder_ = this->sortOrder_;

    for(int index = indexOf(msgSignature);
        index != -1;
        index = indexOf(msgSignature,(index+1))){
        unReadMessages.append(at(index));
    }
    return unReadMessages;

}

ReadMessages ReceivedMessages::getReadMessages()
{
    Message msgSignature;
    msgSignature.receiverReadStatus_ = true;
    msgSignature.searchBy_ = Message::SearchBy::ReceiverReadStatus;

    ReadMessages readMessages;
    readMessages.sortOrder_ = this->sortOrder_;

    for(int index = indexOf(msgSignature);
        index != -1;
        index = indexOf(msgSignature,(index+1))){
        readMessages.append(at(index));
    }
    return readMessages;

}

ReadMessages::ReadMessages(const QString &json):MessageList(json){}

UnReadMessages::UnReadMessages(const QString &json):MessageList(json){}

SentMessages::SentMessages(const QString &json):MessageList(json){}
