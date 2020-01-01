/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "authenticationcontroller.h"
#include "Client/authentication.h"
#include "Helpers/connectionpipe.h"
#include "registeredobjects.h"
#include "Helpers/fileutils.h"


AuthenticationController::AuthenticationController(QObject *parent) : Controller(parent)
{
    CONTROLLER_SET_NAME(AuthenticationController)
}

QString AuthenticationController::authenticationErrorToString(Errors::AuthenticationError authenticationError){
    switch (authenticationError) {
    case Errors::AuthenticationError::NoError :return "NoError";
    case Errors::AuthenticationError::UserAlreadyAuthenticated: return "UserAlreadyAuthenticatedError";
    case Errors::AuthenticationError::IllegalAppUsersState: return "IllegalAppUsersStateError";
    case Errors::AuthenticationError::UserNotRegistered: return "UserNotRegisteredError";
    case Errors::AuthenticationError::UserIsBlacklisted: return "UserIsBlacklistedError";
    case Errors::AuthenticationError::UnknownError: return "UnknownError";
    case Errors::AuthenticationError::UserNameNotSet: return "UserNameNotSetError";
    case Errors::AuthenticationError::AuthKeyNotSet: return "AuthKeyNotSetError";
    case Errors::AuthenticationError::PasswordNotSet: return "PasswordNotSetError";
    case Errors::AuthenticationError::InvalidCredentials: return "InvalidCredentialsError";
    default: return "NoError";
    }
}

Controller *AuthenticationController::createNew()
{
    return new  AuthenticationController(nullptr);//must have a null parent
}

void AuthenticationController::onActionAuthenticationRequiredTriggered(Extras extras)
{
    Q_UNUSED(extras)
    if(Authentication::authenticate()){
        print("authenticating ....");
    }else{
        emit loginRequired();
    }
}

void AuthenticationController::onActionAuthenticationProcessFailedTriggered(Extras extras){
    const int authenticationError = extras.getIntExtras(ExtrasNames::AuthenticationErrorExtras);
    clearCurrentUserInfo();//clear first
    emit authenticationFailed(authenticationError);
}


void AuthenticationController::onActionAuthenticationProcessSuccededTriggered(Extras extras){

    const QString newAuthKey = extras.getStringExtras(ExtrasNames::newAuthKeyExtras);

    const unsigned long id = currentUserModel_.getCurrentId();
    const QString username = authenticationModel_.getUserName(id);

    if(newAuthKey.isEmpty() || (id <= 0) || (username.isEmpty())){
        qWarning() << "AuthenticationController::onActionAuthenticationProcessSuccededTriggered :: Critical error, newAuthy is empty";
    }else{
        authenticationModel_.updateAuthKey(newAuthKey,id);
    }

    emit authenticationSucceded(id,username);
}

void AuthenticationController::onActionLoginSucceededTriggered(Extras extras){

    const unsigned long id = extras.getULongExtras(ExtrasNames::userIdExtras);
    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    const QString authKey = extras.getStringExtras(ExtrasNames::authKeyExtras);
    const QString  password = extras.getStringExtras(ExtrasNames::passwordExtras);

    //save id
    if(currentUserModel_.saveCurrentId(id)
            &&  authenticationModel_.updateCredetials(id,username,authKey,password)){
        emit loginSucceeded(id,username);
    }else{
        qWarning() <<"AuthenticationController::onActionLoginSucceededTriggered( :: Critical error database write failed";
    }

}

void AuthenticationController::onActionLoginFailedTriggered(Extras extras){
    const int authenticationError = extras.getIntExtras(ExtrasNames::AuthenticationErrorExtras);
    emit loginFailed(authenticationError);
}

void AuthenticationController::onActionSignUpSucceededTriggered(Extras extras){

    const unsigned long id = extras.getULongExtras(ExtrasNames::userIdExtras);
    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    const QString authKey = extras.getStringExtras(ExtrasNames::authKeyExtras);
    const QString  password = extras.getStringExtras(ExtrasNames::passwordExtras);

    //save id
    if(currentUserModel_.saveCurrentId(id)
            &&  authenticationModel_.updateCredetials(id,username,authKey,password)){
        emit signUpSucceeded(id,username);
    }else{
        qWarning() <<"AuthenticationController::onActionSignUpSucceededTriggered :: Critical error database write failed";
    }

}

void AuthenticationController::onActionSignUpFailedTriggered(Extras extras){
    const int signUpError = extras.getIntExtras(ExtrasNames::SignUpErrorExtras);
    QString  errorString = extras.getStringExtras(ExtrasNames::errorStringExtras);
    emit signUpFailed(signUpError,errorString);
}

void AuthenticationController::onActionLogOutFailedTriggered(Extras extras){
    int error = extras.getIntExtras(ExtrasNames::LogOutErrorExtras);
    emit logOutFailed(error);
}

void AuthenticationController::onActionUploadAProfilePictureTriggered(Extras extras){
    Q_UNUSED(extras)
    emit profilePictureRequired();
}

void AuthenticationController::onActionProfilePictureUploadTriggered(Extras extras){
    //note esxtras is used
    // user().response().fileSender().setTransferRate(1024*200);//200kb
    std::function<void(QString fileTag,QString fileLocation,float percentageCompletion,quint64 fileSize)> profilePictureUploadingProgressCallback
            = [=](QString fileTag,QString fileLocation,float percentageCompletion,quint64 fileSize){
        emit profilePictureUploadProgress(fileTag,fileLocation,percentageCompletion,fileSize);
    };

    QString profilePictureTag = extras.getStringExtras(ExtrasNames::profilePictureTagExtras);
    if(profilePictureTag.isEmpty()){
        qWarning()<< "AuthenticationController::onActionProfilePictureUploadTriggered Error , profilePictureTag is Empty";
    }

    FILE_SENDER_SETUP_ACTION(profilePictureTag,profilePictureUploadingProgressCallback,0)

    {
        QString filePath = fileSender.currentFile().path();
        QString fileName = QString("%1.%2").arg(fileSender.currentFile().baseName())
                .arg(fileSender.currentFile().fileExtension());

        if(fileSendingFinished){

            emit profilePictureUploaded(fileTag,fileName,filePath);

#ifdef MAGABE_TESTS
            qDebug() << QString("Profile picture uploaded | Name = %1 ,Path = %2")
                        .arg(fileName).arg(filePath);
#endif

        }else{
            if(errorOccured){
                emit profilePictureUploadingFailed(fileTag,fileName,filePath,error.toString());

#ifdef MAGABE_TESTS
                qDebug() << " Error occured " << error.toString();
#endif
            }else{

#ifdef MAGABE_TESTS
                qDebug() << "Profile picture uploading not finished";
#endif

                profilePictureUploadingNotFinished(fileTag,fileName,filePath);
            }
        }

    }

}

void AuthenticationController::onActionProfilePictureChangedTriggered(Extras extras){

    QString  clientProfilePicturePath = extras.getStringExtras(ExtrasNames::clientProfilePicturePathExtras);
    QString  profilePictureFileAlias = extras.getStringExtras(ExtrasNames::profilePictureFileAliasExtras);

    if(clientProfilePicturePath.isEmpty() ||  profilePictureFileAlias.isEmpty()){
        qWarning() << "AuthenticationController::onActionProfilePictureChangedTriggered | Error ,received Empty data";
        return;
    }
    const bool isOnProfilePictureFolder = clientProfilePicturePath.contains(getProfilePicturesFolder());
    const QString newPath = FileUtils::getFileFolderPath(clientProfilePicturePath)+"/"+profilePictureFileAlias;

    bool everyThingOk = true;
    if(!isOnProfilePictureFolder){
        auto tmpPath =   FileUtils::generateFilePath(getProfilePicturesFolder(),FileUtils::getFileName(clientProfilePicturePath));
        everyThingOk = FileUtils::copyFile(clientProfilePicturePath,tmpPath);
        if(everyThingOk){
            clientProfilePicturePath = tmpPath;
        }
    }
    if(everyThingOk){
        const bool renamed = FileUtils::renameFile(clientProfilePicturePath,newPath);
        if(renamed){
            const int userId = currentUserModel_.getCurrentId();
            const QString username = authenticationModel_.getUserName(userId);
            const QString previousProfilePicturePath = userProfilePictureModel_.getPath(userId);
            if(!previousProfilePicturePath.isEmpty()){
                FileUtils::deleteFileFromDisc(previousProfilePicturePath);
            }
            if((userId != 0) && (!username.isEmpty())){
                userProfilePictureModel_.savePath(userId,username,newPath);
            }else{
                qWarning() << "AuthenticationController::onActionProfilePictureChangedTriggered | invalid current user details found";
            }
            emit profilePictureChanged(newPath);
        }
    }
}

void AuthenticationController::onActionUserInfoTriggered(Extras extras){
    const QString info = extras.getStringExtras(ExtrasNames::userInfoExtras);
    emit userInfo(info);
}

void AuthenticationController::onActionUserNotFoundTriggered(Extras extras){
    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    emit userNotFound(username);
}

void AuthenticationController::onActionDownloadProfilePictureTriggered(Extras extras){

    std::function<bool(QString filePath,QString fileTag)> onProfilePictureAlreadyDownloadedCallback = [=](QString filePath,QString fileTag){
        Q_UNUSED(filePath)
        Q_UNUSED(fileTag)
        return true;//delete the file and restrt, if u return false ,the the exution  will exit in here
    };

    std::function<void(QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  downloaded,quint64 fileSize,QString fileTag)> profilePictureDownloadProgressCallback =
            [=](QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  downloaded,quint64 fileSize,QString fileTag){
        Q_UNUSED(fileLocation)
        Q_UNUSED(downloaded)
        emit profilePictureDownloadProgress(fileBaseName+'.'+fileExtension,percentageCompletion,fileSize,fileTag);
    };

    std::function<void(int errorNo,QString errorString,QString fileTag)> onProfilePictureDownloadErrorCallback =[&](int errorNo,QString errorString,QString fileTag){
        Q_UNUSED(errorNo)
        Q_UNUSED(errorString)
        Q_UNUSED(fileTag)
        qDebug()<< "AuthenticationController::onProfilePictureDownloadErrorCallback";
    };


    FILE_DOWNLOAD_SETUP_ACTION(UserControllerName,GetProfilePictureActionName
                               ,TO_CSTR(AuthenticationController),TO_CSTR(ActionDownloadProfilePicture)
                               ,getProfilePicturesFolder(),profilePictureDownloadProgressCallback,onProfilePictureDownloadErrorCallback,onProfilePictureAlreadyDownloadedCallback);

    //note the code below wiil not be executed ,if you return false inside ON_FILE_ALREADY_DOWNLOADED_CALLBACK, in that case this code
    //must also be executed in ON_FILE_ALREADY_DOWNLOADED_CALLBACK
    const bool profilePictureDownloaded  = (downloadCompleted || FileUtils::fileExistsOnDisk(filePath));
    if(profilePictureDownloaded){
        const QString& username = fileTag;
        if(username.isEmpty()){
            qWarning()<< "AuthenticationController::onActionDownloadProfilePictureTriggered ;-Error fileTag is Empty ";
            return;
        }

        const unsigned long userId = currentUserModel_.getCurrentId();
        if(authenticationModel_.getUserName(userId) == username){
            if((userId <= 0)){//must be in here
                qWarning() << "AuthenticationController::onActionDownloadProfilePictureTriggered | invalid current user details found";
                return;
            }
            userProfilePictureModel_.savePath(userId,username,filePath);//NOTE path for other users are not saved here, They are saved in Magbe Client app
            emit userProfilePictureDownloadFinished(username,filePath);
            //then
            emit profilePictureChanged(filePath);
        }else{
            //moving the picture to OtherUsers subfolder
            QString newPath = FileUtils::getFileFolderPath(filePath)+"/OtherUsers/"+FileUtils::getFileName(filePath);
            if(FileUtils::moveFile(filePath,newPath)){
                emit userProfilePictureDownloadFinished(username,newPath);
            }else{
                emit userProfilePictureDownloadFinished(username,filePath);
            }
        }
    }
}

void AuthenticationController::onActionProfilePictureDownloadFailedTriggered(Extras extras){
    const int error = extras.getIntExtras(ExtrasNames::ProfilePictureDownloadErrorExtras);
    const QString& username = extras.getStringExtras(ExtrasNames::usernameExtras);
    emit profilePictureDownloadFailed(username,error);
}

void AuthenticationController::onActionIsUserProfilePictureChangedResultsTriggered(Extras extras){
    auto tag = extras.getStringExtras(ExtrasNames::isUserProfilePictureChangedRequestTagExtras);
    bool changed = extras.getBooleanExtras("changed");
    emit isUserProfilePictureChanged(changed,tag);
}

void AuthenticationController::clearCurrentUserInfo(){
    const unsigned long userId = currentUserModel_.getCurrentId();
    userProfilePictureModel_.deleteProfilePicture(userId);

    authenticationModel_.dropTable();

    currentUserModel_.dropTable();
}

void AuthenticationController::onActionLogOutSucceededTriggered(Extras extras){
    Q_UNUSED(extras)

    clearCurrentUserInfo();

    emit logOutSucceeded();
}

void AuthenticationController::triggerAction(MagabeData data)
{
    //Connect AuthenticationController signals to AuthenticationControllerSignalsReceiver signals
    //ALL CONTROLLER SIGNALS MUST BE CONNECTED THIS WAY SINCE THE LIFE TIME OF A CONTROLLER IS SHORT
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::loginRequired,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::loginRequired);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::signUpSucceeded,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::signUpSucceeded);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::signUpFailed,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::signUpFailed);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::authenticationSucceded,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::authenticationSucceded);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::authenticationFailed,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::authenticationFailed);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::loginSucceeded,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::loginSucceeded);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::loginFailed,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::loginFailed);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::logOutSucceeded,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::logOutSucceeded);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::logOutFailed,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::logOutFailed);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureRequired,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureRequired);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureUploaded,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureUploaded);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureUploadingFailed,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureUploadingFailed);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureUploadingNotFinished,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureUploadingNotFinished);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureChanged,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureChanged);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureUploadProgress,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureUploadProgress);
    ConnectionPipe::connectSignals(RegisteredObjects::AuthenticationController::className,RegisteredObjects::AuthenticationController::Signals::profilePictureDownloadProgress,RegisteredObjects::MagabeClientManagerSignalsReceiver::className,RegisteredObjects::MagabeClientManagerSignalsReceiver::Signals::profilePictureDownloadProgress);

    //all controller  derived class must call this , to peoceed action excution
    Controller::triggerActionDoContinue(data);
}


void AuthenticationController::onActionStartProfilePictureUploadingTriggered(Extras extras){
    QString imagePath = extras.getStringExtras("imagePath");
    QString profilePictureTag = extras.getStringExtras(ExtrasNames::profilePictureTagExtras);

    if(imagePath.isEmpty()){
        qWarning() << "onUploadProfilePicture :: imagePath is Empty";
        return;
    }
    extras.clear();
    extras.addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,ProfilePictureUploadingActionName)
            .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,UserControllerName)
            .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,imagePath)
            .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,0)
            .addExtras(ExtrasNames::profilePictureTagExtras,profilePictureTag)
            .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,false);

    triggerAnotherAction(ActionProfilePictureUpload,extras);

}

void AuthenticationController::onActionStartProfilePictureDownloadingTriggered(Extras extras){

    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);

    user().response().setController(UserControllerName)
            .setAction(GetProfilePictureActionName)
            .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,TO_CSTR(ActionDownloadProfilePicture))
            .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,TO_CSTR(AuthenticationController))
            .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,0)
            .addExtras(ExtrasNames::usernameExtras,username)
            .send();

}



