/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

import QtQuick 2.6
import QtQuick.Window 2.2
import com.magabelab.magabeClient 1.0
import com.magabelab.MQmlCallback 1.0

Window {
    visible: true
    width: 320; height: 120
    title: qsTr("MagabeClient App")


    MagabeClientApp{
        id:app
        anchors.fill: parent


        Text {
            id: connectionStatus
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 25
        }

        MProfilePicture{
            id:profile
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width*0.25; height: parent.height*0.90
        }

        MProfilePicture{
            id:profile2
            anchors.top: parent.top
            anchors.left: profile.right
            width: parent.width*0.25; height: parent.height*0.90
        }

        function setConnectionStaatus(){
            if(connected){
                connectionStatus.text = "Connected!";
                connectionStatus.color = "green"
            }else{
                connectionStatus.text = "DisConnected!";
                connectionStatus.color = "red"

            }
        }

        onLoginRequired: {
            console.log("login ...")

            login("qwertyeer",123456)
        }

        onAuthenticatedChanged: {
            if(authenticated){

                console.log("Authenticated ...")

                //load profile pictures
                profile.userId = app.userId();
                profile2.userId = 1;


                /*----------------------------------------------------------*/
                /*----------------------------------------------------------*/
                console.log("--> getUserInfo")

                app.getUserInfo("chura123"/*2*/,function(data,dataError,mQmlCallbackError){
                    if(mQmlCallbackError !== MQmlCallback.NoError
                            || (dataError !==  Errors.NoError)){
                        console.log("getUserInfo:: Error Occured! ");
                    }
                    console.log(data,dataError,mQmlCallbackError);
                });
                /*----------------------------------------------------------*/

                //                app.getUserInfo(2,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log("getUserInfo:: Error Occured! ");
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/
                //                /*----------------------------------------------------------*/

                //                console.log("--> downloadProfilePicture")

                //                app.downloadProfilePicture(2,function(data,dataError,mQmlCallbackError){
                //                    if(!app.error(dataError,mQmlCallbackError)){
                //                        var obj = app.dataToObject(data);
                //                        console.log("download completed...",obj.profilePicturePath);

                //                    }else{
                //                    }

                //                }
                //                ,function(data,dataError,mQmlCallbackError){
                //                    if(!app.error(dataError,mQmlCallbackError)){
                //                        var obj = app.dataToObject(data);
                //                        console.log("downloading...",obj.fileName,'[',obj.percentageCompletion,']','%');

                //                    }else{
                //                    }
                //                });
                //                /*----------------------------------------------------------*/

                //                app.downloadProfilePicture("chura123",function(data,dataError,mQmlCallbackError){
                //                    if(!app.error(dataError,mQmlCallbackError)){
                //                        var obj = app.dataToObject(data);
                //                        console.log("download completed...",obj.profilePicturePath);

                //                    }else{
                //                    }

                //                }
                //                ,function(data,dataError,mQmlCallbackError){
                //                    if(!app.error(dataError,mQmlCallbackError)){
                //                        var obj = app.dataToObject(data);
                //                        console.log("downloading...",obj.fileName,'[',obj.percentageCompletion,']','%');

                //                    }else{
                //                    }
                //                });

                //                /*----------------------------------------------------------*/
                //                /*----------------------------------------------------------*/

                //                console.log("--> getUserName")

                //                app.getUserName(2,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log("getUserName:: Error Occured! ");
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*----------------------------------------------------------*/
                //                /*----------------------------------------------------------*/

                //                                console.log("--> sendMessage")

                //                                app.sendMessage(1," qwwwe 25-8-2019",function(data,dataError,mQmlCallbackError){
                //                                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                                            || (dataError !==  Errors.NoError)){
                //                                        console.log("sendMessage:: Error Occured! ");
                //                                    }else{
                //                                        var obj = JSON.parse(data);
                //                                        var sentMessageObj  = JSON.parse(data)['sent'];
                //                                        const encodedMessageText = sentMessageObj['encodedText'];
                //                                        const decodeMessageText = app.decodeMessage(encodedMessageText);
                //                                        console.log("sent message :",decodeMessageText);
                //                                    }

                //                                    console.log(data,dataError,mQmlCallbackError);
                //                                });

                //                /*----------------------------------------------------------*/

                //                app.sendMessage(2,"HELLOW 2 TO HOW ARE YOU,SENT USING ID",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log("sendMessage:: Error Occured! ");
                //                    }else{
                //                        var obj = JSON.parse(data);
                //                        var sentMessageObj  = JSON.parse(data)['sent'];
                //                        const encodedMessageText = sentMessageObj['encodedText'];
                //                        const decodeMessageText = app.decodeMessage(encodedMessageText);
                //                        console.log("sent message :",decodeMessageText);
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                /*----------------------------------------------------------*/
                /*----------------------------------------------------------*/

                console.log("--> fetchUserNames")

                app.fetchUserNames([1,2,3,4,5],function(data,dataError,mQmlCallbackError){
                    if(mQmlCallbackError !== MQmlCallback.NoError
                            || (dataError !==  Errors.NoError)){
                        console.log("fetchUserNames:: Error Occured! ");
                    }else{
                        var usernames = JSON.parse(data)['usernames'];
                        console.log("username for 1 = ",usernames[1]);
                    }

                    console.log(data,dataError,mQmlCallbackError);
                });

                //                /*------------------SENT MESSAGES----------------------*/

                //                /*----------------------------------------------------------*/

                //                console.log("--> getSentMessage")

                //                app.getSentMessage("1:2:4",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" MessageNotFound ");
                //                        }else{
                //                            console.log("getSentMessage:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);

                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getSentMessages")

                //                app.getSentMessages("1:2",20,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log("getSentMessages:: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getSentMessagesStartWith")

                //                app.getSentMessagesStartWith("1:2:16",true,10,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log("getSentMessagesStartWith:: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getNextSentMessage")

                //                app.getNextSentMessage("1:2:4",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !== Errors.NoError)){
                //                        console.log("getNextSentMessage:: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/
                //                /*----------------------------------------------------------*/
                //                console.log("--> streamGetSentMessages")

                //                app.streamGetSentMessages("1:2:14",null/*"1:2:14"*/,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log(data,dataError,mQmlCallbackError);
                //                    }
                //                    var dataObj = JSON.parse(data);
                //                    if((parseInt(dataObj.done) !== 1)){
                //                        var messgeText =app.decodeMessage(dataObj.message.encodedText);
                //                        console.log(dataObj.message.messageIdentifier,"--",messgeText,"----sender == ",dataObj.message.sender);
                //                    }else{
                //                        console.log("streamGetSentMessages:: download finished");
                //                    }

                //                });


                //                /*------------------RECEIVED MESSAGES----------------------*/
                //                /*----------------------------------------------------------*/

                //                console.log("--> getReceivedMessage")

                //                app.getReceivedMessage("1:2:16",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" MessageNotFound ");
                //                        }else{
                //                            console.log("getReceivedMessage:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);

                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getReceivedMessagesStartWith")

                //                app.getReceivedMessagesStartWith("1:2:25",true,20,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" no received messages ");
                //                        }else{
                //                            console.log("getSentMessagesStartWith:: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getReceivedMessages")

                //                app.getReceivedMessages("1:2",20,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" no received messages ");
                //                        }else{
                //                            console.log("getSentMessagesStartWith:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("-->  getNextReceivedMessage")
                //                console.log("-->  current  userid = ",app.userId())

                //                app.getNextReceivedMessage("1:2:0",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !== Errors.NoError)){
                //                        console.log(" getNextReceivedMessage:: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> streamReceivedMessages")

                //                app.streamReceivedMessages("1:2:15",/*"1:2:17"*/null,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log(data,dataError,mQmlCallbackError);
                //                        return;
                //                    }
                //                    var dataObj = JSON.parse(data);
                //                    if((parseInt(dataObj.done) !== 1)){
                //                        var receivedMessageObj = dataObj.message;
                //                        var messgeText =app.decodeMessage(receivedMessageObj.encodedText);
                //                        console.log("Received : Message[", receivedMessageObj.messageIdentifier
                //                                    ,"] from -> ",receivedMessageObj.sender," | text -->",messgeText);
                //                    }else{
                //                        console.log("streamReceivedMessages:: download finished");
                //                    }

                //                });

                //                /*--------------------BOTH SENT && RECEIVED MESSAGES--------------------------------------*/
                //                /*-------------------------------------------------------------------------------*/
                //                console.log("--> getMessage")

                //                app.getMessage("1:2:0",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" MessageNotFound ");
                //                        }else{
                //                            console.log("getMessage:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);

                //                });

                                /*----------------------------------------------------------*/

                                console.log("--> getMessages")

                                app.getMessages("1:2",20,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                                    if(mQmlCallbackError !== MQmlCallback.NoError
                                            || (dataError !==  Errors.NoError)){

                                        if(dataError === Errors.MessageNotFound){
                                            console.log(" no received messages ");
                                        }else{
                                            console.log("getMessages:: Error Occured! ");
                                        }
                                    }
                                    console.log(data,dataError,mQmlCallbackError);
                                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getMessagesStartWith")

                //                app.getMessagesStartWith("1:2:14",false,20,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" Errors.MessageNotFound");
                //                        }else{
                //                            console.log("getMessagesStartWith:: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*----------------------------------------------------------*/

                //                console.log("-->  getNextMessage")

                //                app.getNextMessage("1:2:45",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !== Errors.NoError)){
                //                        console.log("getNextMessage:: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*----------------------------------------------------------*/

                //                console.log("--> streamMessages")
                //                console.log("--> stream all messages between user 1 and 2")

                //                var messagesCb = function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log(data,dataError,mQmlCallbackError);
                //                        return;
                //                    }
                //                    var dataObj = JSON.parse(data);
                //                    if((parseInt(dataObj.done) !== 1)){
                //                        messagesCb.count ++;
                //                        console.log("#",messagesCb.count)
                //                        var receivedMessageObj = dataObj.message;
                //                        var messgeText =app.decodeMessage(receivedMessageObj.encodedText);
                //                        console.log("Received : Message[", receivedMessageObj.messageIdentifier
                //                                    ,"] from -> ",receivedMessageObj.sender," to -> ",receivedMessageObj.receiver," | text -->",messgeText);
                //                    }else{
                //                        console.log("streamMessages:: download finished");
                //                    }

                //                };
                //                messagesCb.count = 0;
                //                app.streamMessages("1:2:0",null,messagesCb);


                //                /*--------------------READ MESSAGES--------------------------------------*/
                //                /*-------------------------------------------------------------------------------*/
                //                console.log("--> getReadMessage")

                //                app.getReadMessage("1:2:0",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" MessageNotFound ");
                //                        }else{
                //                            console.log("getReadMessage:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);

                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getReadMessages")

                //                app.getReadMessages("1:2",20,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" no received messages ");
                //                        }else{
                //                            console.log("getReadMessages:: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });

                //                /*----------------------------------------------------------*/

                //                console.log("--> getReadMessagesStartWith")

                //                app.getReadMessagesStartWith("1:2:23",false,20,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" Errors.MessageNotFound");
                //                        }else{
                //                            console.log("getReadMessagesStartWith: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*----------------------------------------------------------*/

                //                console.log("--> streamReadMessages")
                //                console.log("--> stream all read between user 1 and 2")
                //                app.streamReadMessages("1:2:0",null,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log(data,dataError,mQmlCallbackError);
                //                        return;
                //                    }
                //                    var dataObj = JSON.parse(data);
                //                    if((parseInt(dataObj.done) !== 1)){
                //                        var receivedMessageObj = dataObj.message;
                //                        var messgeText =app.decodeMessage(receivedMessageObj.encodedText);
                //                        console.log("Received : Message[", receivedMessageObj.messageIdentifier
                //                                    ,"] from -> ",receivedMessageObj.sender," to -> ",receivedMessageObj.receiver," | text -->",messgeText);
                //                    }else{
                //                        console.log("streamReadMessages:: download finished");
                //                    }

                //                });


                //                /*----------------------------------------------------------*/

                //                console.log("-->  getNextReadMessage:")

                //                app.getNextReadMessage("1:2:45",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !== Errors.NoError)){
                //                        console.log("getNextReadMessage::: Error Occured! ");
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*--------------------UNREAD MESSAGES--------------------------------------*/
                //                /*-------------------------------------------------------------------------------*/
                //                /*-------------------------------------------------------------------------------*/
                //                console.log("--> getUnReadMessage")

                //                app.getUnReadMessage("1:2:15",function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" MessageNotFound ");
                //                        }else{
                //                            console.log("getUnReadMessage:: Error Occured! ");
                //                        }
                //                    }

                //                    console.log(data,dataError,mQmlCallbackError);

                //                });
                //                /*----------------------------------------------------------*/
                //                console.log("--> getUnReadMessages")

                //                app.getUnReadMessages("1:2",20,SortOrder.DESC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" no received messages ");
                //                        }else{
                //                            console.log("getUnReadMessages:: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });
                //                /*----------------------------------------------------------*/


                //                console.log("--> getUnReadMessagesStartWith")

                //                app.getUnReadMessagesStartWith("1:2:15",true,20,SortOrder.ASC,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){

                //                        if(dataError === Errors.MessageNotFound){
                //                            console.log(" Errors.MessageNotFound");
                //                        }else{
                //                            console.log("getUnReadMessagesStartWith: Error Occured! ");
                //                        }
                //                    }
                //                    console.log(data,dataError,mQmlCallbackError);
                //                });


                //                /*----------------------------------------------------------*/

                //                console.log("--> streamUnReadMessages")
                //                console.log("--> stream all read between user 1 and 2")
                //                app.streamUnReadMessages("1:2:0",null/*"1:2:28"*/,function(data,dataError,mQmlCallbackError){
                //                    if(mQmlCallbackError !== MQmlCallback.NoError
                //                            || (dataError !==  Errors.NoError)){
                //                        console.log(data,dataError,mQmlCallbackError);

                //                        return;
                //                    }
                //                    var dataObj = JSON.parse(data);
                //                    if((parseInt(dataObj.done) !== 1)){
                //                        var receivedMessageObj = dataObj.message;
                //                        var messgeText =app.decodeMessage(receivedMessageObj.encodedText);
                //                        console.log("Received : Message[", receivedMessageObj.messageIdentifier
                //                                    ,"] from -> ",receivedMessageObj.sender," to -> ",receivedMessageObj.receiver," | text -->",messgeText);
                //                    }else{
                //                        console.log("streamUnReadMessages:: download finished");
                //                    }

                //                });

                //                /*-------------------------MESSAGE COUNTS---------------------------------*/


                //                //print summary
                //                app.printMessageCountSummary("1:2");

                //                /*------------FIRST MESSAGES-------------------*/
                //                app.getFirstSentMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("FirstSentMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                app.getFirstReceivedMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("FirstReceivedMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                app.getFirstMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("FirstMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                app.getFirstReadMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("FirstReadMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                app.getFirstUnReadMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("FirstUnReadMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });
                //                /*----------------------------------------------------------*/
                //                /*----------------------------------------------------------*/

                //                app.uploadProfilePicture("C:/Users/Edwin Magabe/Pictures/1525671_616086251796745_1143890410_n.jpg"
                //                                         ,function(data,dataError,mQmlCallbackError){
                //                                             console.log(data,dataError,mQmlCallbackError);

                //                                         }
                //                                         ,function(data,dataError,mQmlCallbackError){
                //                                             var obj = app.dataToObject(data);
                //                                             console.log("profile picture uploading ... ",obj.percentageCompletion)
                //                                             // console.log(data,dataError,mQmlCallbackError);

                //                                         });

//                                getLastMessage("1:2",function(data,dataError,mQmlCallbackError){
//                                    console.log("getLastMessage: for conversation 1:2 = ",data);
//                                    console.log("\n");
//                                });

                //                getLastSentMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("getLastSentMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                getLastReceivedMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("getLastReceivedMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                getLastReadMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log("getLastReadMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

                //                getLastUnReadMessage("1:2",function(data,dataError,mQmlCallbackError){
                //                    console.log(" getLastUnReadMessage: for conversation 1:2 = ",data);
                //                    console.log("\n");
                //                });

//                isUserProfilePictureChanged(userId(),getProfilePicturePath(userId()),function(data,dataError,mQmlCallbackError){
//                    var obj = dataToObject(data);
//                    if(obj.changed){
//                        console.log(obj.changed)
//                        if(parseInt(obj.changed) === 1){
//                            console.log(" Profile Picture Changed ");
//                        }else{
//                            console.log(" Profile Picture NOT Changed ");
//                        }
//                    }
//                });
//                /*-----------------------------------------------------------*/

//                var getSMS = function(msg){
//                    getMessage(msg,function(data,dataError,mQmlCallbackError){
//                        if(mQmlCallbackError !== MQmlCallback.NoError
//                                || (dataError !==  Errors.NoError)){

//                            if(dataError === Errors.MessageNotFound){
//                                console.log(" no received messages ");
//                            }else{
//                                console.log("getMessages:: Error Occured! ");
//                            }
//                        }
//                        console.log(data,dataError,mQmlCallbackError);
//                    });
//                };

//                var msg = "1:2:107";
//                getSMS(msg)
//                deleteMassage(msg,function(data,dataError,mQmlCallbackError){
//                    var obj = dataToObject(data);
//                    if(parseInt(obj.deleted) === 1){
//                        console.log(" Message deleted ");
//                        getSMS(msg)
//                    }else{
//                        console.log(" Message NOT deleted");
//                    }
//                });
                /*-----------------------------------------------------------*/

            }
        }

        onConnectedChanged:{
            console.log("-- onConnectedChanged--",connected)
            app.setConnectionStaatus();
        }

        onConnectTimeout: {
            console.log("-- onConnectTimeout")

        }

        Component.onCompleted:{

            connectToServer("127.0.0.1",12345,4000);

        }
    }




}
