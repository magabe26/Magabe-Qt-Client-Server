/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


//NOT ONLY USED SIGNAL AND SLOTS ARE LISTED
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
    /*-----------------MagabeServerApp--------------------------*/
    struct MagabeServerApp{
        constexpr static const char *className = "MagabeServerApp";

        struct Signals{
            constexpr static const char *clientLogedOut = "clientLogedOut";

        };

        struct Slots{


        };
    };

    /*-----------------MagabeServerManagerSignalReceiver--------------------------*/

    struct MagabeServerManagerSignalReceiver{
        constexpr static const char *className = "MagabeServerManagerSignalReceiver";

        struct Signals{
            //signal connected to userController
            constexpr static const char *newSignUp = "newSignUp";
            constexpr static const char *profilePictureUploadProgress = "profilePictureUploadProgress";
            constexpr static const char *profilePictureUploadError = "profilePictureUploadError";
            constexpr static const char *userProfilePictureChanged = "userProfilePictureChanged ";
            constexpr static const char *newFileUploaded = "newFileUploaded";
            constexpr static const char *clientLogedOut = "clientLogedOut";

        };

        struct Slots{

        };
    };

    /*-----------------MagabeServerManager--------------------------*/
    struct MagabeServerManager{
        constexpr static const char *className = "MagabeServerManager";

        struct Signals{
            //connected to MagabeServer compantible Slots
            constexpr static const char *startServer = "startServer";
            constexpr static const char *stopServer = "stopServer";


        };

        struct Slots{
            constexpr static const char *onServerStarted = "onServerStarted";
            constexpr static const char *onServerStoped = "onServerStoped";

            constexpr static const char *onAuthenticedClientsCountChanged = "onAuthenticedClientsCountChanged";
            constexpr static const char *onUnAuthenticatedClientsCountChanged  = "onUnAuthenticatedClientsCountChanged";

            constexpr static const char *onNewAuthentication = "onNewAuthentication";

            constexpr static const char *onClientDisconnected = "onClientDisconnected";

            constexpr static const char *onNewConnection = "onNewConnection";

        };
    };

    /*-----------------MagabeServer--------------------------*/
    struct MagabeServer{
        constexpr static const char *className = "MagabeServer";

        struct Signals{
            constexpr static const char *serverStarted = "serverStarted";
            constexpr static const char *serverStoped = "serverStoped";

        };

        struct Slots{
            constexpr static const char *startServer = "startServer";
            constexpr static const char *stopServer = "stopServer";


        };
    };

    /*-----------------Mgb::ConnectedUsers--------------------------*/
    struct Mgb_ConnectedUsers{
        constexpr static const char *className = "Mgb::ConnectedUsers";

        struct Signals{
            constexpr static const char *authenticedClientsCountChanged ="authenticedClientsCountChanged";
            constexpr static const char *unAuthenticatedClientsCountChanged ="unAuthenticatedClientsCountChanged";
            constexpr static const char *startAuthenticaion = "startAuthenticaion";
            constexpr static const char *clientDisconnected = "clientDisconnected";
            constexpr static const char *newAuthentication = "newAuthentication";
            constexpr static const char *newConnection = "newConnection";

        };

        struct Slots{
            constexpr static const char *onServerStarted = "onServerStarted";
            constexpr static const char *onServerStoped = "onServerStoped";
            constexpr static const char *onLogOut = "onLogOut";



        };
    };

    /*-----------------Template--------------------------*/

    struct Mgb_DataWritter{
        constexpr static const char *className = "Mgb::DataWritter";

        struct Signals{


        };

        struct Slots{
            constexpr static const char *onServerStoped = "onServerStoped";

        };
    };

    /*-----------------UserController--------------------------*/

    struct UserController{
        constexpr static const char *className = "UserController";

        struct Signals{
            constexpr static const char *newSignUp = "newSignUp";
            constexpr static const char *profilePictureUploadProgress = "profilePictureUploadProgress";
            constexpr static const char *profilePictureUploadError = "profilePictureUploadError";
            constexpr static const char *userProfilePictureChanged = "userProfilePictureChanged";
            constexpr static const char *clientLogedOut = "clientLogedOut";


        };

        struct Slots{

        };
    };

    ///*-----------------ServerFilesSignals--------------------------*/

    struct ServerFilesSignals{
        constexpr static const char *className = "ServerFilesSignals";

        struct Signals{
            constexpr static const char *newFileUploaded = "newFileUploaded";


        };

        struct Slots{

        };
    };


};

#endif // REGISTERED_OBJECTS_H
