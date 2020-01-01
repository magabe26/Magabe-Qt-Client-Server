/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


import QtQuick 2.6
import QtQuick.Window 2.2
import com.magabelab.magabeServer 1.0

Window {
    visible: true
    width: 320; height: 120
    title: qsTr("MagabeServer App")
    MQmlCallbackCreator{
        id:callbackCreator
    }

    function setAuthentcatedCount(){
        var count = magabeServerApp.authenticedClientsCount;
        if(count == 0){
            authenticated.color = "red"
        }else{
            authenticated.color = "green"
        }

        authenticated.text = "Authenticated : " + count;

    }

    Text {
        id: authenticated
        anchors.centerIn: parent
        font.pixelSize: 25

    }

    Connections{
        target: magabeServerApp
        ignoreUnknownSignals: true
        onAuthenticedClientsCountChanged:{
            setAuthentcatedCount();

        }
    }


    Component.onCompleted:{
        magabeServerApp.startServer("127.0.0.1",12345)
        //        callMe(callbackCreator.createMQmlCallback(function f(name,error){
        //            console.log(name,error);
        //        }),1000/*timeout*/);
    }
}
