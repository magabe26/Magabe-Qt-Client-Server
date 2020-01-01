/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef REGISTERED_OBJECTS_H
#define REGISTERED_OBJECTS_H
///*-----------------Template--------------------------*/

//struct Template{
//    constexpr static const char *className = "";

//    struct Signals{


//    };

//    struct Slots{

//    };
//};

struct RegisteredObjects{

    /*-----------------AuthenticationController--------------------------*/

    struct AuthenticationController{
        constexpr static const char *className = "AuthenticationController";

        struct Signals{
            constexpr static const char *loginRequired = "loginRequired";
            constexpr static const char *signUpSucceeded = "signUpSucceeded";
            constexpr static const char *signUpFailed = "signUpFailed";
            constexpr static const char *authenticationSucceded = "authenticationSucceded";
            constexpr static const char *authenticationFailed = "authenticationFailed";
            constexpr static const char *loginSucceeded = "loginSucceeded";
            constexpr static const char *loginFailed = "loginFailed";
            constexpr static const char *logOutSucceeded = "logOutSucceeded";
            constexpr static const char *logOutFailed ="logOutFailed";
            constexpr static const char *profilePictureRequired = "profilePictureRequired";
            constexpr static const char *profilePictureUploaded = "profilePictureUploaded";
            constexpr static const char *profilePictureUploadingFailed = "profilePictureUploadingFailed";
            constexpr static const char *profilePictureUploadingNotFinished ="profilePictureUploadingNotFinished";
            constexpr static const char *profilePictureChanged = "profilePictureChanged";
            constexpr static const char *userInfo = "userInfo";
            constexpr static const char *userNotFound = "userNotFound";
            constexpr static const char *userProfilePictureDownloadFinished = "userProfilePictureDownloadFinished";
            constexpr static const char *profilePictureDownloadFailed = "profilePictureDownloadFailed";
            constexpr static const char *profilePictureUploadProgress = "profilePictureUploadProgress";
            constexpr static const char *profilePictureDownloadProgress = "profilePictureDownloadProgress";
            constexpr static const char *isUserProfilePictureChanged = "isUserProfilePictureChanged";

        };

        struct Slots{
        };
    };
    /*-----------------AuthenticationControllerSignalsReceiver--------------------------*/

    struct MagabeClientManagerSignalsReceiver{
        constexpr static const char *className = "MagabeClientManagerSignalsReceiver";

        struct Signals{
            constexpr static const char *loginRequired = "loginRequired";
            constexpr static const char *signUpSucceeded = "signUpSucceeded";
            constexpr static const char *signUpFailed = "signUpFailed";
            constexpr static const char *authenticationSucceded = "authenticationSucceded";
            constexpr static const char *authenticationFailed = "authenticationFailed";
            constexpr static const char *loginSucceeded = "loginSucceeded";
            constexpr static const char *loginFailed = "loginFailed";
            constexpr static const char *logOutSucceeded = "logOutSucceeded";
            constexpr static const char *logOutFailed ="logOutFailed";
            constexpr static const char *profilePictureRequired = "profilePictureRequired";
            constexpr static const char *profilePictureUploaded = "profilePictureUploaded";
            constexpr static const char *profilePictureUploadingFailed = "profilePictureUploadingFailed";
            constexpr static const char *profilePictureUploadingNotFinished ="profilePictureUploadingNotFinished";
            constexpr static const char *profilePictureChanged = "profilePictureChanged";

            constexpr static const char *connectTimeout = "connectTimeout";
            constexpr static const char *profilePictureUploadProgress = "profilePictureUploadProgress";
            constexpr static const char *profilePictureDownloadProgress = "profilePictureDownloadProgress";


        };

        struct Slots{

        };
    };


    /*-----------------MagabeClientApp--------------------------*/

    struct MagabeClientApp{
        constexpr static const char *className = "MagabeClientApp";

        struct Signals{
            constexpr static const char *loginRequired = "loginRequired";
            constexpr static const char *signUpSucceeded = "signUpSucceeded";
            constexpr static const char *signUpFailed = "signUpFailed";
            constexpr static const char *authenticationSucceded = "authenticationSucceded";
            constexpr static const char *authenticationFailed = "authenticationFailed";
            constexpr static const char *loginSucceeded = "loginSucceeded";
            constexpr static const char *loginFailed = "loginFailed";
            constexpr static const char *logOutSucceeded = "logOutSucceeded";
            constexpr static const char *logOutFailed ="logOutFailed";
            constexpr static const char *profilePictureRequired = "profilePictureRequired";
            constexpr static const char *profilePictureUploaded = "profilePictureUploaded";
            constexpr static const char *profilePictureUploadingFailed = "profilePictureUploadingFailed";
            constexpr static const char *profilePictureUploadingNotFinished ="profilePictureUploadingNotFinished";
            constexpr static const char *profilePictureChanged = "profilePictureChanged";
            constexpr static const char *connectTimeout = "connectTimeout";




        };

        struct Slots{

        };
    };
    ///*-----------------MagabeClient--------------------------*/

    struct MagabeClient{
        constexpr static const char *className = "MagabeClient";

        struct Signals{
            constexpr static const char *connectedToMagabeserver = "connectedToMagabeserver";
            constexpr static const char *disconnectedFromMagabeServer = "disconnectedFromMagabeServer";
            constexpr static const char *connectTimeout = "connectTimeout";


        };

        struct Slots{
            constexpr static const char *onConnectToServer = "onConnectToServer";
            constexpr static const char *disconnect = "disconnect";

        };
    };

    ///*-----------------MagabeClientManager--------------------------*/

    struct MagabeClientManager{
        constexpr static const char *className = "MagabeClientManager";

        struct Signals{
            constexpr static const char *connectToServer = "connectToServer";
            constexpr static const char *disconnectFromServer = "disconnectFromServer";


        };

        struct Slots{
            constexpr static const char *onUserInfo = "onUserInfo";
            constexpr static const char *onUserNotFound = "onUserNotFound";

            constexpr static const char *onUserProfilePictureDownloadFinished = "onUserProfilePictureDownloadFinished";
            constexpr static const char *onProfilePictureDownloadFailed  = "onProfilePictureDownloadFailed";

            constexpr static const char *onMessagesReceived = "onMessagesReceived";
            constexpr static const char *onGetMessageError = "onGetMessageError";

            constexpr static const char *onMessageSent = "onMessageSent";
            constexpr static const char *onMessageNotSent = "onMessageNotSent";

            constexpr static const char *onUserNamesReceived ="onUserNamesReceived";

            constexpr static const char *onConnected = "onConnected";
            constexpr static const char *onDisconnected = "onDisconnected";


            constexpr static const char *onMessageCountReceived = "onMessageCountReceived";
            constexpr static const char *onLastMessageReceived = "onLastMessageReceived";
            constexpr static const char *onIsUserProfilePictureChanged = "onIsUserProfilePictureChanged";
            constexpr static const char *onMessageDeletionStatus = "onMessageDeletionStatus";


        };
    };

    /*-----------------ConversationController--------------------------*/

    struct ConversationController{
        constexpr static const char *className = "ConversationController";

        struct Signals{
            constexpr static const char *messagesReceived = "messagesReceived";
            constexpr static const char *getMessageError = "getMessageError";
            constexpr static const char *messageSent = "messageSent";
            constexpr static const char *messageNotSent  = "messageNotSent";
            constexpr static const char *userNamesReceived = "userNamesReceived";
            constexpr static const char *messageCountReceived = "messageCountReceived";
            constexpr static const char *lastMessageReceived = "lastMessageReceived";
            constexpr static const char *messageDeletionStatus = "messageDeletionStatus";

        };

        struct Slots{

        };
    };
};

#endif // REGISTERED_OBJECTS_H
