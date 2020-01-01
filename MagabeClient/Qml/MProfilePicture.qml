import QtQuick 2.0
import com.magabelab.magabeClient 1.0
import com.magabelab.MQmlCallback 1.0
import "qrc:/MagabeClientAppHelper.js" as MagabeClientAppHelper

Rectangle {
    id:root
    property int userId: 0
    property color progressBarColor: "gray"
    property color progressBarTextColor: "black"

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#66343434" }
        GradientStop { position: 1.0; color: "#66000000" }
    }

    MQmlCallbackCreator{
        Component.onCompleted: {
            MagabeClientAppHelper.init(this)
        }

    }

    onUserIdChanged: {
        if(userId >= 0){
            profilePicture.load(userId);
        }
    }


    Image {
        id: profilePicture
        anchors.fill: parent
        onSourceChanged: {
            progressBar.visible = false;
            loadingText.visible = false;
            progressBar.downloadProgress = 0;
        }

        Rectangle{
            id:progressBar
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            visible: false
            color: root.progressBarColor
            height:loadingText.paintedHeight + 5
            width: 0
            z:profilePicture.z +1
            property real downloadProgress: 0
            onDownloadProgressChanged: {
                if(!visible){
                    visible = true;
                }

                loadingText.text = downloadProgress + '%';
                width = (downloadProgress/100)*parent.width;

                progressMonitorTimer.restart();
            }

            Text {
                id: loadingText
                visible: progressBar.visible
                anchors.centerIn: parent
                color: progressBarTextColor
            }
            Timer{
                id:progressMonitorTimer
                interval: 4000//4sec
                repeat: false
                onTriggered: {
                    if((progressBar.downloadProgress > 0)
                            && (progressBar.downloadProgress < 100)){
                        if(magabeClientApp.connected && (root.userId > 0)){//download just stoped,restart
                            profilePicture.load(root.userId);
                        }
                    }
                }
            }

        }

        function load(userId){
            const setProfilePicture = function(path){
                profilePicture.source = "file:/"+path;
            };

            const downloadProfilePicture = function(userId){
                if(!progressMonitorTimer.running)
                    progressMonitorTimer.start();

                MagabeClientAppHelper.downloadProfilePicture(userId,function(data,dataError,mQmlCallbackError){
                    if(!MagabeClientAppHelper.error(dataError,mQmlCallbackError)){
                        var obj = MagabeClientAppHelper.dataToObject(data);
                        if(obj.profilePicturePath)
                            setProfilePicture(obj.profilePicturePath);
                    }

                }
                ,function(data,dataError,mQmlCallbackError){
                    if(!MagabeClientAppHelper.error(dataError,mQmlCallbackError)){
                        var obj = MagabeClientAppHelper.dataToObject(data);
                        progressBar.downloadProgress = obj.percentageCompletion;
                    }
                });
            };

            const isProfilePictureChanged = function(userId,profilePicturePath){
                MagabeClientAppHelper.isUserProfilePictureChanged(userId,profilePicturePath,function(data,dataError,mQmlCallbackError){
                    var obj = MagabeClientAppHelper.dataToObject(data);
                    if(parseInt(obj.changed) === 1){
                        downloadProfilePicture(userId);
                    }else{
                        setProfilePicture(profilePicturePath);
                    }
                });

            };

            const path = MagabeClientAppHelper.getProfilePicturePath(userId);
            if(String(path).trim().length != 0){
                isProfilePictureChanged(userId,path);
            }else{
                downloadProfilePicture(userId);
            }
        }
    }
}
