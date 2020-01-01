/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


/* Using chura mkia trick*/

import QtQuick 2.0
import com.magabelab.MQmlCallback 1.0

Item {
    Component{
        id: mQmlCallbackComponent
        MQmlCallback{
            objectName: "MQmlCallback"
            property var jsFunction
            property real progressCbTimeout:  (60000);/*1min*/
//            Component.onDestruction:  {
//                console.log("destroying MQmlCallback--------")
//            }

            function makeProgressive(){
                isProgressCallback = true;
            }

            function destroyCallback(){
                if(timerIsRunning())
                    stopTimer();

                destroy();
            }

            onJsFunctionCaller: {

                if(timerIsRunning())
                    stopTimer();

                if(jsFunction)
                    jsFunction(data,dataError,mQmlCallbackError);

                if(!isProgressCallback){
                    destroy();
                }else{//wait for progress update if is isProgressCallback
                   startTimer(progressCbTimeout)
                }
            }

            onTimeout: {
                if((jsFunction)&& (!isProgressCallback)){
                    jsFunction("{}",0,MQmlCallback.Timeout);
                }

                destroy();
            }
        }

    }

    /*

    A call back function(func) must be in this formart

    function(data,dataError,mQmlCallbackError){

    }

    where
         data = a json string, or normal string
         dataError = an int error that correspond to c++ enum errors of that function
         mQmlCallbackError = { NoError = 0, CallbackCreationFailed = 1, Timeout = 2 };

    */
    function createMQmlCallback(func,timeoutInXMilliseconds) {
        var mQmlCallback =  mQmlCallbackComponent.createObject(parent);
        if (mQmlCallback === null) {
            if(func){
                func("{}",0,MQmlCallback.CallbackCreationFailed);//9 failed to create callback
            }
            console.log("MQmlCallbackCreator.js,createMQmlCallback  | Error creating mQmlCallback");
        }else{
            if(func){
                mQmlCallback.jsFunction = func;
                mQmlCallback.startTimer(timeoutInXMilliseconds)
            }else{
                console.log("MQmlCallbackCreator.js,createMQmlCallback  | Error invalid function passed");
            }
        }
        return mQmlCallback;
    }
}
