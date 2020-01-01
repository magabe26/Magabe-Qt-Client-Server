/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONVERSATIONCONTROLLER_H
#define CONVERSATIONCONTROLLER_H

#include <QObject>
#include <Controllers/controller.h>

class ConversationController : public Controller
{
    Q_OBJECT
    Q_ENUMS(Actions)

public:
    struct ExtrasNames{
        NEW_EXTRAS_NAME(jsonMessages)
        NEW_EXTRAS_NAME(OnGetMessagesError)
        NEW_EXTRAS_NAME(getMessageRequestTag)
        NEW_EXTRAS_NAME(sendMessageRequestTag)
        NEW_EXTRAS_NAME(jsonMessage)
        NEW_EXTRAS_NAME(OnSendMessageError)
        NEW_EXTRAS_NAME(userIds)
        NEW_EXTRAS_NAME(fetchUserNamesRequestTag)
        NEW_EXTRAS_NAME(jsonUserNames)
        NEW_EXTRAS_NAME(getMessageCountRequestTag)
        NEW_EXTRAS_NAME(messageCount)
        NEW_EXTRAS_NAME(getLastMessageRequestTag)
        NEW_EXTRAS_NAME(deleteMessageRequestTag)

    };


    enum Actions{
        ActionMessagesReceived,
        ActionGetMessageError,
        ActionMessageSent,
        ActionMessageNotSent,
        ActionUserNamesReceived,
        ActionMessageCountReceived,
        ActionLastMessageReceived,
        ActionMessageDeletionStatus
    };

    REGISTER_ACTIONS

    explicit ConversationController(QObject *parent);

    // Controller interface
    Controller *createNew();

signals:
    void messagesReceived(QString jsonMessages, QString tag);
    void getMessageError(int onGetMessagesError, QString tag);
    void messageSent(QString jsonMessage, QString tag);
    void messageNotSent(int onSendMessageError, QString tag);
    void userNamesReceived(QString jsonUserNames, QString tag);
    void messageCountReceived(int count,QString tag);
    void lastMessageReceived(QString jsonLastMessage, QString tag);
    void messageDeletionStatus(bool status, QString tag);

public slots:
    /**
     * @brief onActionMessagesReceivedTriggered
     * @param extras =>ExtrasNames::jsonMessagesExtras
     *                 ExtrasNames::getMessageRequestTagExtras
     */
    void onActionMessagesReceivedTriggered(Extras extras);

    /**
     * @brief onActionGetMessageErrorTriggered
     * @param extras => ExtrasNames::OnGetMessagesErrorExtras);
                        ExtrasNames::getMessageRequestTagExtras
     */
    void onActionGetMessageErrorTriggered(Extras extras);

    /**
     * @brief onActionMessageSentTriggered
     * @param extras =>ExtrasNames::sendMessageRequestTagExtras
                       ExtrasNames::jsonMessageExtras
     */
    void onActionMessageSentTriggered(Extras extras);

    /**
     * @brief onActionMessageNotSentTriggered
     * @param extras => ExtrasNames::sendMessageRequestTagExtras
                        ExtrasNames::OnSendMessageErrorExtras
     */
    void onActionMessageNotSentTriggered(Extras extras);

    /**
     * @brief onActionUserNamesTriggered
     * @param extras =>ExtrasNames::fetchUserNamesRequestTagExtras
                       ExtrasNames::jsonUserNamesExtras
     */
    void onActionUserNamesReceivedTriggered(Extras extras);

    /**
     * @brief onActionMessageCountReceivedTriggered
     * @param extras => ExtrasNames::getMessageCountRequestTagExtras
                        ExtrasNames::messageCountExtras
     */
    void onActionMessageCountReceivedTriggered(Extras extras);
    /**
     * @brief onActionLastMessageReceivedTriggered
     * @param extras =>
                        ExtrasNames::jsonMessageExtras
                        ExtrasNames::getLastMessageRequestTagExtras
     */
    void onActionLastMessageReceivedTriggered(Extras extras);

    void onActionMessageDeletionStatusTriggered(Extras extras);
};

#endif // CONVERSATIONCONTROLLER_H
