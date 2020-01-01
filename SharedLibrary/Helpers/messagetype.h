#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum class MessageType{
    Sent = 0xcf6, //3318
    Received,
    SentOrReceived,
    Read,
    UnRead
};

#endif // MESSAGETYPE_H
