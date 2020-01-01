/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "conversation.h"
#include <QDebug>

Conversation::Conversation(const QString &conversationIdentifier){
    conversationIdentifier_ =  conversationIdentifier;
    if(!isValid()){
        conversationIdentifier_.clear();
    }
}

Conversation::Conversation(const Conversation &other){
    conversationIdentifier_ =  other.conversationIdentifier_;
    if(!isValid()){
        conversationIdentifier_.clear();
    }
}

QString Conversation::conversationIdentifier(){
    return conversationIdentifier_;
}

bool Conversation::operator ==(const Conversation &other){
    return (this->conversationIdentifier_ == other.conversationIdentifier_);
}

bool Conversation::operator !=(const Conversation &other){
    return (this->conversationIdentifier_ != other.conversationIdentifier_);
}

bool Conversation::isValid(){
    return isValidConversationIdentifier(conversationIdentifier_);
}

unsigned long Conversation::getOtherUser(unsigned long userId){
    if(isValid()){
        auto arry = conversationIdentifier_.split(':',QString::SkipEmptyParts);
        if(arry.at(0).toULong() == userId){
            return arry.at(1).toULong();
        }else{
            return arry.at(0).toULong();
        }
    }else{
        return 0;
    }
}

bool Conversation::isInvolved(unsigned long userId)
{
    auto arry = conversationIdentifier_.split(':',QString::SkipEmptyParts);
    if(arry.length() == 2){
        const auto id1 = arry.at(0).toULong();
        const auto id2 = arry.at(1).toULong();
        return ((id1 == userId) || (id2 == userId));
    }else{
        return false;
    }
}

bool Conversation::isValidConversationIdentifier(const QString &conversationIdentifier){
    auto arry = conversationIdentifier.split(':');
    if(arry.length() == 2){
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

        return true;

    }else{
        return false;
    }

}

QString Conversation::generateConversationIdentifier(unsigned long userId1, unsigned long userId2)
{
    return (userId1 < userId2) ? QString("%1:%2").arg(userId1).arg(userId2)
                               : QString("%1:%2").arg(userId2).arg(userId1);
}

Conversation ConversationList::getConversation(const QString &conversationIdentifier){
    const int index = indexOf(Conversation(conversationIdentifier));
    if(index != -1){
        return at(index);
    }else{
        return Conversation("");
    }
}


QDebug operator <<(QDebug debug, const Conversation& conversation){
    debug.noquote();debug.space() << const_cast<Conversation&>(conversation).conversationIdentifier();
    return debug.quote();
}

QDebug operator <<(QDebug debug, const ConversationList &list){
    QStringList strlist;
    foreach (auto conversation, list) {
        strlist.append(conversation.conversationIdentifier());
    }
    debug.noquote();debug.space() << strlist;
    return debug.quote();
}
