/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

/*

USAGE:-
import "qrc:/MagabeClientAppHelper.js" as MagabeClientAppHelper

    MQmlCallbackCreator{
        Component.onCompleted: {
            MagabeClientAppHelper.init(this)
        }
    }

A call back function(cb) must be in this formart

function(data,dataError,mQmlCallbackError){

}

where
     data = a json string, or normal string
     dataError = an int error that correspond to c++ enum errors of that function
     mQmlCallbackError = { NoError = 0, CallbackCreationFailed = 1, Timeout = 2 };


      ->   SortOrder  =  { DESC : 0, ASC : 1 };
*/



var Errors = {
    NoError : 0,
    UserAlreadyAuthenticated : 0x7e3, //2019
    IllegalAppUsersState : 0x7e4,//2020 not used incase of client
    UserNotRegistered : 0x7e5, //2021
    UserIsBlacklisted : 0x7e6, //2022
    UserNameNotSet : 0x7e7,//2023
    AuthKeyNotSet : 0x7e8,//2024
    PasswordNotSet : 0x7e9,//2025
    InvalidCredentials : 0x7ea,//2026
    UnknownError : 0x13ba, //5050
    UsernameHasLessThan6Characters : 0x7c, //124
    WeakPassword : 0x7d,//125
    UserAlreadyRegistered : 0x7e,//126
    InvalidEmail : 0x7f,//127
    EmailAlreadyTaken : 0x80,//128
    InvalidGender : 0x81,//129
    FirstNameNotSet : 0x82, //130
    LastNameNotSet : 0x83,//131
    InvalidDateOfBirth : 0x84,//132
    UserNotFound : 0x19f,//415
    ConnectionErrors : 0x14b, //331
    UserNotAuthenticated : 0xd62,//3426
    ProfilePictureNotUploadedYet : 0x1a0,//146
    ConversationNotFound : 0xd63,//3427
    MessageNotFound : 0xd64,//3428
    UserNotInvolvedInConversation : 0xd65,//3429
    InvalidMessageIdentifier : 0x100f, //4111
    InvalidConversationIdentifier : 0x1e3d, //7741

};

var MQmlCallback = { NoError : 0, CallbackCreationFailed : 1, Timeout : 2 };

var SortOrder  =  { DESC : 0, ASC : 1 };

var callbackCreator;

/*User must call this funcction before using this js module*/
function init(cbCreator){
    callbackCreator = cbCreator;
}

var defaultValues ={
    mQmlCallbackTimeOut : (60000*2),/*2min*/
    done: '{\"done\":\"1\"}',
    emptyData: '{}'

}




/* -----------------------copy-functions-----------------------------*/

function signUp(username,irstName,lastName,dateOfBirth,gender, password,email){
    magabeClientApp.signUp(username,irstName,lastName,dateOfBirth,gender, password,email);
}

function login(username,password){
    magabeClientApp.login(username,password);
}

function logOut(){
    magabeClientApp.logOut();
}

function connectToServer(ipAddress,port,msecsTimeout){
    magabeClientApp.connectToServer(ipAddress,port,msecsTimeout);
}

function disconnectFromServer(){
    magabeClientApp.disconnectFromServer();
}

function dataToObject(data){
    return JSON.parse(data);
}

function objectToJson(obj){
    return JSON.stringify(obj);
}

function userId(){
    return magabeClientApp.userId();
}

function userName(){
    return magabeClientApp.userName();
}

function authKey(){
    return magabeClientApp.authKey();
}

function password(){
    return magabeClientApp.password();
}

//return true if any error ocurred,param =>data is there to match the callbackks
function error(dataError,mQmlCallbackError){
    return ((dataError !== Errors.NoError)
            || (mQmlCallbackError !== Errors.NoError))
}

function toMQmlCallback(cb){
    return callbackCreator.createMQmlCallback(cb,defaultValues.mQmlCallbackTimeOut);
}

/* return error 10 on errors and set errorString*/
function uploadProfilePicture(imagePath
                              ,uploadedCb
                              ,uploadProgressCb){

    var qmlUploadProgressCb = toMQmlCallback(function(data,dataError,mQmlCallbackError){
        uploadProgressCb(data,dataError,mQmlCallbackError);
    });

    qmlUploadProgressCb.makeProgressive();//prevent from being auto destroyed

    var qmlUploadedCb = toMQmlCallback(function(data,dataError,mQmlCallbackError){
        uploadedCb(data,dataError,mQmlCallbackError);

        qmlUploadProgressCb.destroyCallback();
        qmlFailedCb.destroyCallback();

    });

    var qmlFailedCb = toMQmlCallback(function(data,dataError,mQmlCallbackError){
        uploadedCb(data,dataError,mQmlCallbackError);

        qmlUploadProgressCb.destroyCallback();
        qmlUploadedCb.destroyCallback();
    });

    magabeClientApp.uploadProfilePicture(imagePath,qmlUploadedCb
                                         ,qmlFailedCb
                                         ,qmlUploadProgressCb);
}

/* if param is  userIdOrUsername ,Pass number for userId Or a string for username*/

/* dataError = Errors::UserInfoError*/
function getUserInfo(userIdOrUsername,cb){
    magabeClientApp.getUserInfo(userIdOrUsername,toMQmlCallback(cb));
}

/*dataError = Errors::ProfilePictureDownloadError*/
function downloadProfilePicture(userIdOrUsername,downloadCb,downloadProgressCb){

    var qmlDownloadProgressCb = toMQmlCallback(function(data,dataError,mQmlCallbackError){
        downloadProgressCb(data,dataError,mQmlCallbackError);
    });

    qmlDownloadProgressCb.makeProgressive();//prevent from being auto destroyed

    var qmlDownloadCb = toMQmlCallback(function(data,dataError,mQmlCallbackError){
        qmlDownloadProgressCb.destroyCallback();
        downloadCb(data,dataError,mQmlCallbackError);
    });
    magabeClientApp.downloadProfilePicture(userIdOrUsername,qmlDownloadCb,qmlDownloadProgressCb);
}

/* return dataError == 10 on error*/
function getUserName(userId,cb){
    magabeClientApp.getUserName(userId,toMQmlCallback(cb));
}

/*dataError = Errors::OnSendMessageError*/
function sendMessage(userIdOrUsername,message,cb){
    magabeClientApp.sendMessage(userIdOrUsername,message,toMQmlCallback(cb));
}

function decodeMessage(encodedMessage){
    return magabeClientApp.decodeMessage(encodedMessage);
}

/* Returns dataError == 10 on error*/
function fetchUserNames(userIdList,cb){
    magabeClientApp.fetchUserNames(userIdList,toMQmlCallback(cb));
}

function generateMessageObject(messageIdentifier){
    return { messageIdentifier : messageIdentifier};
}

function generateConversationObject(conversationIdentifier){
    return { conversationIdentifier : conversationIdentifier};
}

function isValidConversationIdentifier(conversationIdentifier){
    return  magabeClientApp.isValidConversationIdentifier(conversationIdentifier);
}

function isValidMessageIdentifier(messageIdentifier){
    return  magabeClientApp.isValidMessageIdentifier(messageIdentifier);
}

/*    Returns null if messageIdentifier is not valid*/
function generateConversationIdentifier(messageIdentifier){
    return magabeClientApp.generateConversationIdentifier(messageIdentifier);
}


function onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier){
    var message = dataToObject(data);
    var messageIdentifier = message.messageIdentifier;

    if(messageIdentifier){

        if(cb[messageIdentifier]){//is a repeat
            return messageIdentifier;
        }

        cb[messageIdentifier] = true;

        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start > end){//not in range
                return messageIdentifier;
            }
        }
    }else{
        messageIdentifier = ""; //change from undefined
    }

    var obj = {
        done : 0,
        message : (message) ? message : defaultValues.emptyData
    };

    cb(objectToJson(obj),dataError,mQmlCallbackError);

    return messageIdentifier;
}

/*-------------------------------------------------------------------------------*/

function printMessageCountSummary(conversationIdentifier){
    var summary = String();
    var appendText = function(text){
        summary = summary + text + '\n';
    }
    var appendLine = function(){appendText("----------------------------------");}
    var printProgress = function(percentange){
        console.log('MessageCountSummary for [ '+conversationIdentifier+' ] loading ... '+percentange+'%');
    }

    getSentMessageCount(conversationIdentifier,function(data,dataError,mQmlCallbackError){
        appendText('')
        appendText("-----------MessageCountSummary-----------");
        appendText("      Conversation ["+conversationIdentifier+'] userId:'+userId());
        appendLine();
        appendLine();
        appendText("SentMessages ="+data);
        appendLine();
        printProgress(25);
        getReceivedMessageCount(conversationIdentifier,function(data,dataError,mQmlCallbackError){
            appendText("ReceivedMessages ="+data);
            appendLine();
            printProgress(50)
            getReadMessageCount(conversationIdentifier,function(data,dataError,mQmlCallbackError){
                appendText("-> ReadMessages ="+data);
                printProgress(62.5)
                getUnReadMessageCount(conversationIdentifier,function(data,dataError,mQmlCallbackError){
                    appendText("-> UnReadMessages ="+data);
                    appendLine();
                    printProgress(80)
                    getMessageCount(conversationIdentifier,function(data,dataError,mQmlCallbackError){
                        appendText("Total Messages = "+data);
                        appendLine();
                        appendLine();
                        printProgress(100)
                        console.log(summary);
                    });
                });
            });

        });
    });
}

function messageFilter(data,dataError,mQmlCallbackError,cb){
    if(mQmlCallbackError !== MQmlCallback.NoError
            || (dataError !== Errors.NoError)){
        cb(defaultValues.emptyData,dataError,mQmlCallbackError);
    }else{
        if(data){
            var obj = dataToObject(data);
            if(obj){
                var messagesObj = obj.Messages;
                if(messagesObj){
                    var keys = Object.keys(messagesObj);
                    if((keys) && (keys.length == 1)){
                        var receivedMessage = messagesObj[keys[0]];
                        cb(objectToJson(receivedMessage),0,0);
                    }else{
                        cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
                    }
                }else{
                    cb(defaultValues.emptyData,Errors.UnknownError,mQmlCallbackError);
                }
            }else{
                cb(defaultValues.emptyData,Errors.UnknownError,mQmlCallbackError);
            }
        }else{
            cb(defaultValues.emptyData,Errors.UnknownError,mQmlCallbackError);
        }
    }

}

/*-----------------SENT MESSAGES------------------------------------*/
/*-------------------------------------------------------------------------------*/

function getFirstSentMessage(conversationIdentifier,cb){
    var startMessageIdentifier = conversationIdentifier + ':0';
    getSentMessagesStartWith(startMessageIdentifier,true,1,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function getSentMessageCount(conversationIdentifier,cb){
    magabeClientApp.getSentMessageCount(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/*returns empty {} is the message is not found/already deleted*/
function getSentMessage(messageIdentifier,cb){
    getSentMessagesStartWith(messageIdentifier,true,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,function(data,dataError,mQmlCallbackError){
            //because the server sends the next available message,if messageIdentifier is not found, we have to check
            var message = dataToObject(data)
            if(message.messageIdentifier === messageIdentifier){
                cb(data,dataError,mQmlCallbackError);
            }else{
                cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
            }
        });
    });
}

function getSentMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getSentMessages(generateConversationObject(conversationIdentifier),maxNoOfMessages,sortOrder
                                    ,toMQmlCallback(datafilter));
}

function getSentMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getSentMessages(generateMessageObject(messageIdentifier),includeStartMessage,maxNoOfMessages,sortOrder
                                    ,toMQmlCallback(datafilter));

}

function getNextSentMessage(messageIdentifier,cb){
    getSentMessagesStartWith(messageIdentifier,false,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);

    });
}

/*    Pass endMessageIdentifier == null , To loop through all the SentMessages
                      Returns {done:1} object when done
                */
function streamGetSentMessages(startMessageIdentifier,endMessageIdentifier,cb){

    if((!magabeClientApp.isValidMessageIdentifier(startMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }

    if((endMessageIdentifier) && (!magabeClientApp.isValidMessageIdentifier(endMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }


    cb.callCount = 0;

    var shouldGetMore = function(messageIdentifier){
        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start >= end){
                return false;
            }else{
                return true;
            }
        } else{
            return true;
        }
    };


    var fetchTheFirstMessage = function(messageIdentifier){
        getSentMessage(messageIdentifier,function(data,dataError,mQmlCallbackError){

            var nextMessageIdentifier;
            var errorOccurred = error(dataError,mQmlCallbackError);
            if(errorOccurred){
                nextMessageIdentifier =  messageIdentifier;
            }else{
                nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(nextMessageIdentifier){
                    nextMessageIdentifier =  messageIdentifier;
                }
            }

            if(shouldGetMore(nextMessageIdentifier)){
                fetchTheNextMessage(nextMessageIdentifier);
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };


    var fetchTheNextMessage = function(startMessageIdentifier){
        getNextSentMessage(startMessageIdentifier,function(data,dataError,mQmlCallbackError){
            if(!error(dataError,mQmlCallbackError)){
                var  nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(shouldGetMore(nextMessageIdentifier)){
                    fetchTheNextMessage(nextMessageIdentifier);
                }else{
                    cb(defaultValues.done,dataError,mQmlCallbackError)
                }
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };

    fetchTheFirstMessage(startMessageIdentifier);

}

function getLastSentMessage(conversationIdentifier,cb){
    magabeClientApp.getLastSentMessage(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/*-------------------------------------------------------------------------------*/

/*--------------------RECEIVED MESSAGES--------------------------------------*/
/*-------------------------------------------------------------------------------*/

function getFirstReceivedMessage(conversationIdentifier,cb){
    var startMessageIdentifier = conversationIdentifier + ':0';
    getReceivedMessagesStartWith(startMessageIdentifier,true,1,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function getReceivedMessageCount(conversationIdentifier,cb){
    magabeClientApp.getReceivedMessageCount(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/* returns empty {} is the message is not found/already deleted*/
function getReceivedMessage(messageIdentifier,cb){
    getReceivedMessagesStartWith(messageIdentifier,true,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,function(data,dataError,mQmlCallbackError){
            //because the server sends the next available message,if messageIdentifier is not found, we have to check
            var message = dataToObject(data)
            if(message.messageIdentifier === messageIdentifier){
                cb(data,dataError,mQmlCallbackError);
            }else{
                cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
            }
        });
    });
}

function getReceivedMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getReceivedMessages(generateConversationObject(conversationIdentifier),maxNoOfMessages,sortOrder
                                        ,toMQmlCallback(datafilter));
}

function getReceivedMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){

    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };

    magabeClientApp.getReceivedMessages(generateMessageObject(messageIdentifier),includeStartMessage,maxNoOfMessages,sortOrder
                                        ,toMQmlCallback(datafilter));

}

function getNextReceivedMessage(messageIdentifier,cb){
    getReceivedMessagesStartWith(messageIdentifier,false,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

/*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                      Returns {done:1} object when done
                */
function streamReceivedMessages(startMessageIdentifier,endMessageIdentifier,cb){
    if((!magabeClientApp.isValidMessageIdentifier(startMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }

    if((endMessageIdentifier) && (!magabeClientApp.isValidMessageIdentifier(endMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }


    cb.callCount = 0;

    var shouldGetMore = function(messageIdentifier){
        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start >= end){
                return false;
            }else{
                return true;
            }
        } else{
            return true;
        }
    };

    var fetchTheFirstMessage = function(messageIdentifier){
        getReceivedMessage(messageIdentifier,function(data,dataError,mQmlCallbackError){

            var nextMessageIdentifier;
            var errorOccurred = error(dataError,mQmlCallbackError);
            if(errorOccurred){
                nextMessageIdentifier =  messageIdentifier;
            }else{
                nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(nextMessageIdentifier){
                    nextMessageIdentifier =  messageIdentifier;
                }
            }

            if(shouldGetMore(nextMessageIdentifier)){
                fetchTheNextMessage(nextMessageIdentifier);
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };


    var fetchTheNextMessage = function(startMessageIdentifier){
        getNextReceivedMessage(startMessageIdentifier,function(data,dataError,mQmlCallbackError){
            if(!error(dataError,mQmlCallbackError)){
                var  nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(shouldGetMore(nextMessageIdentifier)){
                    fetchTheNextMessage(nextMessageIdentifier);
                }else{
                    cb(defaultValues.done,dataError,mQmlCallbackError)
                }
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };

    fetchTheFirstMessage(startMessageIdentifier);

}

function getLastReceivedMessage(conversationIdentifier,cb){
    magabeClientApp.getLastReceivedMessage(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/*-------------------------------------------------------------------------------*/

/*--------------------BOTH SENT && RECEIVED MESSAGES--------------------------------------*/
/*-------------------------------------------------------------------------------*/

function getFirstMessage(conversationIdentifier,cb){
    var startMessageIdentifier = conversationIdentifier + ':0';
    getMessagesStartWith(startMessageIdentifier,true,1,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function getMessageCount(conversationIdentifier, cb){
    magabeClientApp.getMessageCount(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/* returns empty {} is the message is not found/already deleted*/
function getMessage(messageIdentifier,cb){
    getMessagesStartWith(messageIdentifier,true,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,function(data,dataError,mQmlCallbackError){
            //because the server sends the next available message,if messageIdentifier is not found, we have to check
            var message = dataToObject(data)
            if(message.messageIdentifier === messageIdentifier){
                cb(data,dataError,mQmlCallbackError);
            }else{
                cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
            }
        });
    });
}

function getMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getMessages(generateConversationObject(conversationIdentifier),maxNoOfMessages,sortOrder
                                ,toMQmlCallback(datafilter));
}

function getMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){

    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };

    magabeClientApp.getMessages(generateMessageObject(messageIdentifier),includeStartMessage,maxNoOfMessages,sortOrder
                                ,toMQmlCallback(datafilter));

}

function getNextMessage(messageIdentifier,cb){
    getMessagesStartWith(messageIdentifier,false,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function streamMessages(startMessageIdentifier,endMessageIdentifier,cb){
    if((!magabeClientApp.isValidMessageIdentifier(startMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }

    if((endMessageIdentifier) && (!magabeClientApp.isValidMessageIdentifier(endMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }


    cb.callCount = 0;

    var shouldGetMore = function(messageIdentifier){
        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start >= end){
                return false;
            }else{
                return true;
            }
        } else{
            return true;
        }
    };

    var fetchTheFirstMessage = function(messageIdentifier){
        getMessage(messageIdentifier,function(data,dataError,mQmlCallbackError){

            var nextMessageIdentifier;
            var errorOccurred = error(dataError,mQmlCallbackError);
            if(errorOccurred){
                nextMessageIdentifier =  messageIdentifier;
            }else{
                nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(nextMessageIdentifier){
                    nextMessageIdentifier =  messageIdentifier;
                }
            }

            if(shouldGetMore(nextMessageIdentifier)){
                fetchTheNextMessage(nextMessageIdentifier);
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };


    var fetchTheNextMessage = function(startMessageIdentifier){
        getNextMessage(startMessageIdentifier,function(data,dataError,mQmlCallbackError){
            if(!error(dataError,mQmlCallbackError)){
                var  nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(shouldGetMore(nextMessageIdentifier)){
                    fetchTheNextMessage(nextMessageIdentifier);
                }else{
                    cb(defaultValues.done,dataError,mQmlCallbackError)
                }
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };

    fetchTheFirstMessage(startMessageIdentifier);

}

function getLastMessage(conversationIdentifier,cb){
    magabeClientApp.getLastMessage(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/*--------------------READ MESSAGES--------------------------------------*/
/*-------------------------------------------------------------------------------*/

function getFirstReadMessage(conversationIdentifier,cb){
    var startMessageIdentifier = conversationIdentifier + ':0';
    getReadMessagesStartWith(startMessageIdentifier,true,1,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function getReadMessageCount(conversationIdentifier,cb){
    magabeClientApp.getReadMessageCount(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/* returns empty {} is the message is not found/already deleted*/
function getReadMessage(messageIdentifier,cb){
    getReadMessagesStartWith(messageIdentifier,true,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,function(data,dataError,mQmlCallbackError){
            //because the server sends the next available message,if messageIdentifier is not found, we have to check
            var message = dataToObject(data)
            if(message.messageIdentifier === messageIdentifier){
                cb(data,dataError,mQmlCallbackError);
            }else{
                cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
            }
        });
    });
}

function getReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getReadMessages(generateConversationObject(conversationIdentifier),maxNoOfMessages,sortOrder
                                    ,toMQmlCallback(datafilter));
}

function getReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){

    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };

    magabeClientApp.getReadMessages(generateMessageObject(messageIdentifier),includeStartMessage,maxNoOfMessages,sortOrder
                                    ,toMQmlCallback(datafilter));

}

function getNextReadMessage(messageIdentifier,cb){
    getReadMessagesStartWith(messageIdentifier,false,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

/*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                      Returns {done:1} object when done
                */
function streamReadMessages(startMessageIdentifier,endMessageIdentifier,cb){
    if((!magabeClientApp.isValidMessageIdentifier(startMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }

    if((endMessageIdentifier) && (!magabeClientApp.isValidMessageIdentifier(endMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }


    cb.callCount = 0;

    var shouldGetMore = function(messageIdentifier){
        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start >= end){
                return false;
            }else{
                return true;
            }
        } else{
            return true;
        }
    };


    var fetchTheFirstMessage = function(messageIdentifier){
        getReadMessage(messageIdentifier,function(data,dataError,mQmlCallbackError){

            var nextMessageIdentifier;
            var errorOccurred = error(dataError,mQmlCallbackError);
            if(errorOccurred){
                nextMessageIdentifier =  messageIdentifier;
            }else{
                nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(nextMessageIdentifier){
                    nextMessageIdentifier =  messageIdentifier;
                }
            }

            if(shouldGetMore(nextMessageIdentifier)){
                fetchTheNextMessage(nextMessageIdentifier);
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };


    var fetchTheNextMessage = function(startMessageIdentifier){
        getNextReadMessage(startMessageIdentifier,function(data,dataError,mQmlCallbackError){
            if(!error(dataError,mQmlCallbackError)){
                var  nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(shouldGetMore(nextMessageIdentifier)){
                    fetchTheNextMessage(nextMessageIdentifier);
                }else{
                    cb(defaultValues.done,dataError,mQmlCallbackError)
                }
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };

    fetchTheFirstMessage(startMessageIdentifier);

}

function getLastReadMessage(conversationIdentifier,cb){
    magabeClientApp.getLastReadMessage(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/*--------------------UNREAD MESSAGES--------------------------------------*/
/*-------------------------------------------------------------------------------*/
function getFirstUnReadMessage(conversationIdentifier,cb){
    var startMessageIdentifier = conversationIdentifier + ':0';
    getUnReadMessagesStartWith(startMessageIdentifier,true,1,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

function getUnReadMessageCount(conversationIdentifier,cb){
    magabeClientApp.getUnReadMessageCount(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

/* returns empty {} is the message is not found/already deleted*/
function getUnReadMessage(messageIdentifier,cb){
    getUnReadMessagesStartWith(messageIdentifier,true,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,function(data,dataError,mQmlCallbackError){
            //because the server sends the next available message,if messageIdentifier is not found, we have to check
            var message = dataToObject(data)
            if(message.messageIdentifier === messageIdentifier){
                cb(data,dataError,mQmlCallbackError);
            }else{
                cb(defaultValues.emptyData,Errors.MessageNotFound,mQmlCallbackError);
            }
        });
    });
}

function getUnReadMessages(conversationIdentifier,maxNoOfMessages,sortOrder,cb){
    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };
    magabeClientApp.getUnReadMessages(generateConversationObject(conversationIdentifier),maxNoOfMessages,sortOrder
                                      ,toMQmlCallback(datafilter));
}

function getUnReadMessagesStartWith(messageIdentifier,includeStartMessage,maxNoOfMessages,sortOrder,cb){

    var datafilter = function(data,dataError,mQmlCallbackError){

        if(error(dataError,mQmlCallbackError)){
            cb(defaultValues.emptyData,dataError,mQmlCallbackError);
        }else{
            cb(data,dataError,mQmlCallbackError);
        }
    };

    magabeClientApp.getUnReadMessages(generateMessageObject(messageIdentifier),includeStartMessage,maxNoOfMessages,sortOrder
                                      ,toMQmlCallback(datafilter));

}

function getNextUnReadMessage(messageIdentifier,cb){
    getUnReadMessagesStartWith(messageIdentifier,false,1,0,function(data,dataError,mQmlCallbackError){
        messageFilter(data,dataError,mQmlCallbackError,cb);
    });
}

/*    Pass endMessageIdentifier == null , To loop through all the ReceivedMessages
                                        Returns {done:1} object when done
                                  */
function streamUnReadMessages(startMessageIdentifier,endMessageIdentifier,cb){
    if((!magabeClientApp.isValidMessageIdentifier(startMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }

    if((endMessageIdentifier) && (!magabeClientApp.isValidMessageIdentifier(endMessageIdentifier))){
        cb(defaultValues.done,Errors.InvalidConversationIdentifierError,0);
        return;
    }


    cb.callCount = 0;

    var shouldGetMore = function(messageIdentifier){
        if(endMessageIdentifier){
            var start = parseInt(String(messageIdentifier).split(':')[2]);
            var end = parseInt(String(endMessageIdentifier).split(':')[2]);
            if(start >= end){
                return false;
            }else{
                return true;
            }
        } else{
            return true;
        }
    };

    var fetchTheFirstMessage = function(messageIdentifier){
        getUnReadMessage(messageIdentifier,function(data,dataError,mQmlCallbackError){

            var nextMessageIdentifier;
            var errorOccurred = error(dataError,mQmlCallbackError);
            if(errorOccurred){
                nextMessageIdentifier =  messageIdentifier;
            }else{
                nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(nextMessageIdentifier){
                    nextMessageIdentifier =  messageIdentifier;
                }
            }

            if(shouldGetMore(nextMessageIdentifier)){
                fetchTheNextMessage(nextMessageIdentifier);
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };


    var fetchTheNextMessage = function(startMessageIdentifier){
        getNextUnReadMessage(startMessageIdentifier,function(data,dataError,mQmlCallbackError){
            if(!error(dataError,mQmlCallbackError)){
                var  nextMessageIdentifier = onStreamMessageDataReceived(data,dataError,mQmlCallbackError,cb,endMessageIdentifier);
                if(shouldGetMore(nextMessageIdentifier)){
                    fetchTheNextMessage(nextMessageIdentifier);
                }else{
                    cb(defaultValues.done,dataError,mQmlCallbackError)
                }
            }else{
                cb(defaultValues.done,dataError,mQmlCallbackError)
            }
            cb.callCount ++;
        });
    };

    fetchTheFirstMessage(startMessageIdentifier);

}

function getLastUnReadMessage(conversationIdentifier,cb){
    magabeClientApp.getLastUnReadMessage(generateConversationObject(conversationIdentifier),toMQmlCallback(cb));
}

function getProfilePicturePath(userIdOrUsername){
    return magabeClientApp.getProfilePicturePath(userIdOrUsername);
}

function isUserProfilePictureChanged(userId,imagePath,cb){
    magabeClientApp.isUserProfilePictureChanged(userId,imagePath,toMQmlCallback(cb));
}

function deleteMassage(messageIdentifier,cb){
    magabeClientApp.deleteMassage(generateMessageObject(messageIdentifier),toMQmlCallback(cb));
}
