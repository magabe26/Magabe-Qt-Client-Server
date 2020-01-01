import QtQuick 2.0
import com.magabelab.magabeClient 1.0
import com.magabelab.MQmlCallback 1.0
import "qrc:/MagabeClientAppHelper.js" as MagabeClientAppHelper


Item {
    id:root

    property bool connected
    property bool authenticated

    signal loginRequired();
    signal signUpSucceeded(int id, string username);
    signal signUpFailed(int signUpError, string errorString);
    signal authenticationSucceded(int id,string username);
    signal authenticationFailed(int authenticationError);
    signal loginSucceeded(int id,string username);
    signal loginFailed(int authenticationError);
    signal logOutSucceeded();
    signal logOutFailed(int logOutError);
    signal profilePictureRequired();
    signal profilePictureChanged(string imagePath);
    signal connectTimeout();

    Component.onCompleted: {
        connected = magabeClientApp.connected;
        authenticated = magabeClientApp.authenticated;
    }

    MQmlCallbackCreator{
        Component.onCompleted: {
            MagabeClientAppHelper.init(this)
        }
    }

    Connections{
        target: magabeClientApp
        ignoreUnknownSignals: true;

        onLoginRequired:{
            root.loginRequired();
        }

        onSignUpSucceeded:{
            root.signUpSucceeded(id,username);
        }

        onSignUpFailed:{
            root.signUpFailed(signUpError,errorString);
        }
        onAuthenticationSucceded:{
            root.authenticationSucceded(id,username);
        }
        onAuthenticationFailed:{
            root.authenticationFailed(authenticationError);
        }

        onLoginSucceeded:{
            root.loginSucceeded(id,username);
        }

        onLoginFailed:{
            root.loginFailed(authenticationError);
        }

        onLogOutSucceeded:{
            root.logOutSucceeded();
        }

        onLogOutFailed:{
            root.logOutFailed(logOutError);
        }

        onProfilePictureRequired:{
            root.profilePictureRequired();
        }

        onProfilePictureChanged:{
            root.profilePictureChanged(imagePath);
        }

        onConnectTimeout:{
            root.connectTimeout();

        }
        onConnectedChanged:{
            root.connected = magabeClientApp.connected;

        }

        onAuthenticatedChanged:{
            root.authenticated = magabeClientApp.authenticated;
        }
    }

    QtObject{
        id:defaultValues
        property real mQmlCallbackTimeOut : (60000*2);/*2min*/
        property string done: '{\"done\":\"1\"}'
        property string emptyData: '{}'

    }

    function signUp(username,irstName,lastName,dateOfBirth,gender, password,email){
        MagabeClientAppHelper.signUp(username,irstName,lastName,dateOfBirth,gender, password,email);
    }

    function login(username,password){
        MagabeClientAppHelper.login(username,password);
    }

    function logOut(){
        MagabeClientAppHelper.logOut();
    }

    function connectToServer(ipAddress,port,msecsTimeout){
        MagabeClientAppHelper.connectToServer(ipAddress,port,msecsTimeout);
    }

    function disconnectFromServer(){
        MagabeClientAppHelper.disconnectFromServer();
    }

    function dataToObject(data){
        return MagabeClientAppHelper.dataToObject(data);
    }

    function objectToJson(obj){
        return MagabeClientAppHelper.objectToJson(obj);
    }

    function userId(){
        return MagabeClientAppHelper.userId();
    }

    function userName(){
        return MagabeClientAppHelper.userName();
    }

    function authKey(){
        return MagabeClientAppHelper.authKey();
    }

    function password(){
        return MagabeClientAppHelper.password();
    }

    //return true if any error ocurred,param =>data is there to match the callbackks
    function error(dataError,mQmlCallbackError){
        return MagabeClientAppHelper.error(dataError,mQmlCallbackError);
    }

    function toMQmlCallback(cb){
        return MagabeClientAppHelper.toMQmlCallback(cb);
    }

    /* return error 10 on errors and set errorString*/
    function uploadProfilePicture(imagePath
                                  ,uploadedCb
                                  ,uploadProgressCb){

        MagabeClientAppHelper.uploadProfilePicture(imagePath,uploadedCb,uploadProgressCb);
    }

    /* if param is  userIdOrUsername ,Pass number for userId Or a string for username*/

    /* dataError = Errors::UserInfoError*/
    function getUserInfo(userIdOrUsername,cb){
        MagabeClientAppHelper.getUserInfo(userIdOrUsername,cb);
    }

    /*dataError = Errors::ProfilePictureDownloadError*/
    function downloadProfilePicture(userIdOrUsername,downloadCb,downloadProgressCb){
        MagabeClientAppHelper.downloadProfilePicture(userIdOrUsername,downloadCb,downloadProgressCb);
    }

    /* return dataError == 10 on error*/
    function getUserName(userId,cb){
        MagabeClientAppHelper.getUserName(userId,cb);
    }

    /*dataError = Errors::OnSendMessageError*/
    function sendMessage(userIdOrUsername,message,cb){
        MagabeClientAppHelper.sendMessage(userIdOrUsername,message,cb);
    }

    function decodeMessage(encodedMessage){
        return MagabeClientAppHelper.decodeMessage(encodedMessage);
    }

    /* Returns dataError == 10 on error*/
    function fetchUserNames(userIdList,cb){
        MagabeClientAppHelper.fetchUserNames(userIdList,cb);
    }

    function generateMessageObject(messageIdentifier){
        return MagabeClientAppHelper.generateMessageObject(messageIdentifier);
    }

    function generateConversationObject(conversationIdentifier){
        return MagabeClientAppHelper.generateConversationObject(conversationIdentifier);
    }

    function isValidConversationIdentifier(conversationIdentifier){
        return  MagabeClientAppHelper.isValidConversationIdentifier(conversationIdentifier);
    }

    function isValidMessageIdentifier(messageIdentifier){
        return  MagabeClientAppHelper.isValidMessageIdentifier(messageIdentifier);
    }

    /*    Returns null if messageIdentifier is not valid*/
    function generateConversationIdentifier(messageIdentifier){
        return MagabeClientAppHelper.generateConversationIdentifier(messageIdentifier);
    }


    function onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier){
        return MagabeClientAppHelper.onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
    }

    /*-------------------------------------------------------------------------------*/

    function printMessageCountSummary(conversationIdentifier){
        MagabeClientAppHelper.printMessageCountSummary(conversationIdentifier);
    }

    function messageFilter(data,dataError,mQmlCallbackError,cb){
        MagabeClientAppHelper.messageFilter(data,dataError,mQmlCallbackError,cb);
    }

    /*-----------------SENT MESSAGES------------------------------------*/
    /*-------------------------------------------------------------------------------*/

    function getFirstSentMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getFirstSentMessage(conversationIdentifier,cb);
    }

    function getSentMessageCount(conversationIdentifier,cb){
        MagabeClientAppHelper.getSentMessageCount(conversationIdentifier,cb);
    }

    function getSentMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getSentMessage(messageIdentifier,cb);
    }

    function getSentMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getSentMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb);
    }

    function getSentMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getSentMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb);
    }

    function getNextSentMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getNextSentMessage(messageIdentifier,cb);
    }

    /*    Pass endMessageIdentifier == null , To loop through all the SentMessages
                          Returns {done:1} object when done
                    */
    function streamGetSentMessages(startMessageIdentifier,endMessageIdentifier,cb){
        MagabeClientAppHelper.streamGetSentMessages(startMessageIdentifier,endMessageIdentifier,cb);
    }

    function getLastSentMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getLastSentMessage(conversationIdentifier,cb);
    }

    /*-------------------------------------------------------------------------------*/

    /*--------------------RECEIVED MESSAGES--------------------------------------*/
    /*-------------------------------------------------------------------------------*/

    function getFirstReceivedMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getFirstReceivedMessage(conversationIdentifier,cb);
    }

    function getReceivedMessageCount(conversationIdentifier,cb){
        MagabeClientAppHelper.getReceivedMessageCount(conversationIdentifier,cb);
    }

    function getReceivedMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getReceivedMessage(messageIdentifier,cb);
    }

    function getReceivedMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getReceivedMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb);
    }

    function getReceivedMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getReceivedMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb);
    }

    function getNextReceivedMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getNextReceivedMessage(messageIdentifier,cb);
    }

    /*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                          Returns {done:1} object when done
                    */
    function streamReceivedMessages(startMessageIdentifier,endMessageIdentifier,cb){
        MagabeClientAppHelper.streamReceivedMessages(startMessageIdentifier,endMessageIdentifier,cb);
    }

    function getLastReceivedMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getLastReceivedMessage(conversationIdentifier,cb);
    }

    /*-------------------------------------------------------------------------------*/

    /*--------------------BOTH SENT && RECEIVED MESSAGES--------------------------------------*/
    /*-------------------------------------------------------------------------------*/

    function getFirstMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getFirstMessage(conversationIdentifier,cb);
    }

    function getMessageCount(conversationIdentifier, cb){
        MagabeClientAppHelper.getMessageCount(conversationIdentifier, cb);
    }

    function getMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getMessage(messageIdentifier,cb);
    }

    function getMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb);
    }

    function getMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb);
    }

    function getNextMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getNextMessage(messageIdentifier,cb);
    }

    function streamMessages(startMessageIdentifier,endMessageIdentifier,cb){
        MagabeClientAppHelper.streamMessages(startMessageIdentifier,endMessageIdentifier,cb);
    }

    function getLastMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getLastMessage(conversationIdentifier,cb);
    }

    /*--------------------READ MESSAGES--------------------------------------*/
    /*-------------------------------------------------------------------------------*/

    function getFirstReadMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getFirstReadMessage(conversationIdentifier,cb);
    }

    function getReadMessageCount(conversationIdentifier,cb){
        MagabeClientAppHelper.getReadMessageCount(conversationIdentifier,cb);
    }

    function getReadMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getReadMessage(messageIdentifier,cb);
    }

    function getReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb);
    }

    function getReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb);
    }

    function getNextReadMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getNextReadMessage(messageIdentifier,cb);
    }

    /*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                          Returns {done:1} object when done
                    */
    function streamReadMessages(startMessageIdentifier,endMessageIdentifier,cb){
        MagabeClientAppHelper.streamReadMessages(startMessageIdentifier,endMessageIdentifier,cb);
    }

    function getLastReadMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getLastReadMessage(conversationIdentifier,cb);
    }

    /*--------------------UNREAD MESSAGES--------------------------------------*/
    /*-------------------------------------------------------------------------------*/
    function getFirstUnReadMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getFirstUnReadMessage(conversationIdentifier,cb);
    }

    function getUnReadMessageCount(conversationIdentifier,cb){
        MagabeClientAppHelper.getUnReadMessageCount(conversationIdentifier,cb);
    }

    function getUnReadMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getUnReadMessage(messageIdentifier,cb);
    }

    function getUnReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getUnReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb);
    }

    function getUnReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
        MagabeClientAppHelper.getUnReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb);
    }

    function getNextUnReadMessage(messageIdentifier,cb){
        MagabeClientAppHelper.getNextUnReadMessage(messageIdentifier,cb);
    }

    /*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                                            Returns {done:1} object when done
                                      */
    function streamUnReadMessages(startMessageIdentifier,endMessageIdentifier,cb){
        MagabeClientAppHelper.streamUnReadMessages(startMessageIdentifier,endMessageIdentifier,cb);
    }


    function getLastUnReadMessage(conversationIdentifier,cb){
        MagabeClientAppHelper.getLastUnReadMessage(conversationIdentifier,cb);
    }

    function getProfilePicturePath(userIdOrUsername){
        return MagabeClientAppHelper.getProfilePicturePath(userIdOrUsername);
    }

    function isUserProfilePictureChanged(userId,imagePath,cb){
        MagabeClientAppHelper.isUserProfilePictureChanged(userId,imagePath,cb);
    }

    function deleteMassage(messageIdentifier,cb){
        MagabeClientAppHelper.deleteMassage(messageIdentifier,cb);
    }

}
