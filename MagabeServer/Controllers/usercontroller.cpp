/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "usercontroller.h"
#include <QDebug>
#include "Helpers/connectionpipe.h"
#include "registeredobjects.h"


UserController::UserController(QObject *parent) : Controller(parent)
{
    //first thing to do
    CONTROLLER_SET_NAME(UserController)
}

UserController::~UserController()
{


}

Controller *UserController::createNew()
{
    return new UserController(nullptr);
}


void UserController::onActionSignUpTriggered(Extras extras){

    const  QString username= extras.getStringExtras(ExtrasNames::usernameExtras);
    const  QString  firstName  = extras.getStringExtras(ExtrasNames::firstNameExtras);
    const  QString   lastName = extras.getStringExtras(ExtrasNames::lastNameExtras);
    const  QString  dateOfBirth = extras.getStringExtras(ExtrasNames::dateOfBirthExtras);
    const  User::Gender gender = (extras.getStringExtras(ExtrasNames::genderExtras).toUpper().at(0) == QChar('M')) ? User::Gender::MALE : User::Gender::FEMALE;
    const  QString password = extras.getStringExtras(ExtrasNames::passwordExtras);
    const  QString  email = extras.getStringExtras(ExtrasNames::emailExtras);


    auto response =  user().response();
    response.setController(AuthenticationControllerName);

    //check 0
    if(appUsers().userRegistered(username)){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UserAlreadyRegistered)
                .addExtras(ExtrasNames::errorStringExtras,QString("This username \'%1\' is already taken!").arg(username))
                .send();
        return;
    }

    //check 1
    if(user().isRegistered()){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UserAlreadyRegistered)
                .addExtras(ExtrasNames::errorStringExtras,QString("You are already registered under username \'%1\',please logOut first, inoder to signUp a new account!").arg(user().username()))
                .send();
        return;
    }

    //check 2
    if(username.length() < 6){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UsernameHasLessThan6Characters)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("Username must have atleast 6 characters"))
                .send();

        return;
    }

    //check 3
    if(password.length() < 6){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::WeakPassword)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("Password must have atleast 6 characters"))
                .send();
        return;
    }

    //check 4
    if(!appUsers().isValidEmail(email)){//must be before isEmailTaken check
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::InvalidEmail)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("Email you entered is not valid"))
                .send();
        return;
    }


    //check 5
    if(appUsers().isEmailTaken(email)){

        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::EmailAlreadyTaken)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("This email \'%1\' is already registered").arg(email))
                .send();

        return;
    }

    //check 6
    if((gender != User::Gender::MALE) && (gender != User::Gender::FEMALE)){

        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::InvalidGender)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("Gender value must be M or F"))
                .send();

        return;
    }

    if(firstName.isEmpty()){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::FirstNameNotSet)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("First must not be empty"))
                .send();

        return;
    }

    if(lastName.isEmpty()){
        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::LastNameNotSet)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("Lastname must not be empty"))
                .send();

        return;
    }

    //    if(invalidDateOfBirth){//TO DO COMPLETE
    //        response.setAction(SignUpFailedActionName)
    //                .addExtras(ExtrasNames::SignUpErrorExtras,(int)SignUpError::InvalidDateOfBirthSignUpError)
    //                .addExtras(ExtrasNames::errorStringExtras
    //                           ,QString("dateOfBirth must be a valid date"))
    //                .send();

    //        return;
    //    }

    const QString  NOT_SET_YET;
    if(appUsers().registerNewUser(firstName,lastName,dateOfBirth
                                  ,gender,username,password
                                  ,email,NOT_SET_YET,NOT_SET_YET
                                  ,NOT_SET_YET,NOT_SET_YET
                                  ,NOT_SET_YET)){

        //then authenticate
        Errors::AuthenticationError error = Errors::AuthenticationError::NoError ;
        const QString emptyAuthKey = "";
        QString newAuthKey;
        bool authenticated =  user().authenticate(username,emptyAuthKey,password,&error,&newAuthKey,false);
        if(!authenticated && (error == Errors::AuthenticationError::IllegalAppUsersState)){
            qWarning() << "UserController::onActionSignUpTriggere :: Critical Implementation Error , appUsers_ is null";

            response.setAction(SignUpFailedActionName)
                    .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UnknownError)
                    .addExtras(ExtrasNames::errorStringExtras
                               ,QString("System error please try again later"))
                    .send();
            return;
        }


        if(authenticated && user().isClientAuthenticated()){

            emit newSignUp(user().id(),user().getThisClientMgbSessionId());

            response.setAction(SignUpSucceededActionName)
                    .addExtras(ExtrasNames::userIdExtras,(unsigned long)user().id())
                    .addExtras(ExtrasNames::usernameExtras,user().username())
                    .addExtras(ExtrasNames::authKeyExtras,user().authKey())
                    .addExtras(ExtrasNames::passwordExtras,user().password())
                    .send();
            //request a user Profile Picture
            requestProfilePicture();

        }else{
            print("UserController::onActionSignUpTriggered ,Criticall Error == " +ConnectedUser::authenticationErrorToString(error));
            response.setAction(SignUpFailedActionName)
                    .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UnknownError)
                    .addExtras(ExtrasNames::errorStringExtras
                               ,QString("System error please try again later"))
                    .send();
        }

    }else{            print("UserController::onActionSignUpTriggered ,Criticall Error == ");

        response.setAction(SignUpFailedActionName)
                .addExtras(ExtrasNames::SignUpErrorExtras,(int)Errors::SignUpError::UnknownError)
                .addExtras(ExtrasNames::errorStringExtras
                           ,QString("System error please try again later"))
                .send();
    }

}

void UserController::onActionAuthenticateTriggered(Extras extras){


    auto response =  user().response();
    response.setController(AuthenticationControllerName);

    if(!user().isClientAuthenticated()){

        //const unsigned long userId =  extras.getULongExtras(ExtrasNames::userIdExtras);
        const QString username =  extras.getStringExtras(ExtrasNames::usernameExtras);
        const QString authKey = extras.getStringExtras(ExtrasNames::authKeyExtras);
        const QString password =extras.getStringExtras(ExtrasNames::passwordExtras);

        //qDebug() << userId << username << authKey << password << user().isClientAuthenticated();

        //then authenticate
        Errors::AuthenticationError error = Errors::AuthenticationError::NoError ;
        QString newAuthKey;
        bool authenticated =  user().authenticate(username,authKey,password,&error,&newAuthKey,true);
        if(!authenticated && (error == Errors::AuthenticationError::IllegalAppUsersState)){
            qWarning() << "UserController::onActionAuthenticateTriggered :: Critical Implementation Error , appUsers_ is null";
            response.setAction(AuthenticationProcessFailedActionName)
                    .addExtras(ExtrasNames::AuthenticationErrorExtras,(int)Errors::AuthenticationError::UnknownError)
                    .send();
            return;
        }


        if(authenticated && user().isClientAuthenticated()){

            response.setAction(AuthenticationProcessSuccededActionName)
                    .addExtras(ExtrasNames::newAuthKeyExtras,newAuthKey)
                    .send();
            //request a user Profile Picture
            requestProfilePicture();
        }else{
            response.setAction(AuthenticationProcessFailedActionName)
                    .addExtras(ExtrasNames::AuthenticationErrorExtras,(int)error)
                    .send();
        }

    }else{
        response.setAction(AuthenticationProcessFailedActionName)
                .addExtras(ExtrasNames::AuthenticationErrorExtras,(int)Errors::AuthenticationError::UserAlreadyAuthenticated)
                .send();
    }

}


void UserController::onActionLoginTriggered(Extras extras){

    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    const QString password = extras.getStringExtras(ExtrasNames::passwordExtras);
    auto response =  user().response();
    response.setController(AuthenticationControllerName);

    Errors::AuthenticationError error = Errors::AuthenticationError::NoError ;

    if(user().logIn(username,password,&error)){
        response.setAction(LoginSucceededActionName)
                .addExtras(ExtrasNames::userIdExtras,user().id())
                .addExtras(ExtrasNames::usernameExtras,user().username())
                .addExtras(ExtrasNames::authKeyExtras,user().authKey())
                .addExtras(ExtrasNames::passwordExtras,user().password())
                .send();

        //request a user Profile Picture
        requestProfilePicture();
    }else{
        response.setAction(LoginFailedActionName)
                .addExtras(ExtrasNames::AuthenticationErrorExtras,(int)error)
                .send();
    }

}

void UserController::onActionLogOutTriggered(Extras extras){
    Q_UNUSED(extras)

    auto response =  user().response();
    response.setController(AuthenticationControllerName);

    if(user().isClientAuthenticated()){

        response.setAction(LogOutSucceededActionName)
                .addExtras("info","you have been logout")//just to make the data valid
                .send();
    }else{//never gonna happen due to checks on triggerAction
        response.setAction(LogOutFailedActionName)
                .addExtras(ExtrasNames::LogOutErrorExtras,(int)Errors::LogOutError::UserNotAuthenticated)
                .send();
    }
}

void UserController::onActionProfilePictureUploadingTriggered(Extras extras){
    //NOTE IGNORE UNUSED VRIABLE WRNING ,extras IS USED
    if(!user().isRegistered()){
        qWarning() << "Unregistered user tring to upload a profile picture,"
                      " this is not supposed to happen WTF ";
        return;
    }

    auto response =  user().response();
    response.setController(AuthenticationControllerName);

    std::function<bool(QString filePath,QString fileTag)> onProfilePictureAlreadyUploadedCallback = [=](QString filePath,QString fileTag){
        Q_UNUSED(filePath)
        Q_UNUSED(fileTag)
        // qDebug() << QString("Profile picture already downloaded to path %1 , delete it, and reuploading ..").arg(filePath);
        return true;//delete the file and restrt
    };

    std::function<void(QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  uploaded,quint64 fileSize)> profilePictureUploadProgressCallback
            = [=](QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  uploaded,quint64 fileSize){
        emit profilePictureUploadProgress(user().id(),fileBaseName,fileExtension,fileLocation
                                          ,percentageCompletion,uploaded
                                          ,fileSize);
        //qDebug() << QString("%1 is uploading profile picture ... %2%").arg(user().username()).arg(percentageCompletion);
    };

    std::function<void(int errorNo,QString errorString)> onProfilePictureUploadErrorCallback = [=](int errorNo,QString errorString){
        emit profilePictureUploadError(errorNo,errorString);
        //qDebug() << errorString;
    };

    FILE_UPLOAD_SETUP_ACTION(AuthenticationControllerName /*FILESENDER_CONTROLLER*/
                             ,ProfilePictureUploadActionName /*FILESENDER_ACTION*/
                             ,UserControllerName/*CALLBACK_CONTROLLER*/
                             ,ProfilePictureUploadingActionName/*CALLBACK_ACTION*/
                             ,QString("%1/%2").arg(profilePictureFolder).arg(user().username()) /*SAVE_FOLDER*/
                             ,profilePictureUploadProgressCallback/*UPLOAD_PROGRESS_CALLBACK*/
                             ,onProfilePictureUploadErrorCallback/*ON_ERROR_CALLBACK*/
                             ,onProfilePictureAlreadyUploadedCallback)/*ON_FILE_ALREADY_UPLOADED_CALLBACK*/

    {
        if(uploadCompleted){
            QString newServerFilePath = serverFiles().recordFile(user().id(),filePath);
            if(!newServerFilePath.isEmpty()){
                user().updateProfilePicturePath(newServerFilePath);
                emit userProfilePictureChanged(user().id(),user().profilePicturePath());
                //notify client ProfilePictureChanged
                response.setAction(ProfilePictureChangedActionName)
                        .addExtras(ExtrasNames::clientProfilePicturePathExtras,fileLocation)
                        .addExtras(ExtrasNames::profilePictureFileAliasExtras,serverFiles().getFileName(user().profilePicturePath()))
                        .send();
            }else{
                qWarning() << "UserController::onActionProfilePictureUploadingTriggered | Error serverFiles().recordFile Failed";
                if(serverFiles().fileExistsOnDisk(filePath)){
                    user().updateProfilePicturePath(filePath);
                    emit userProfilePictureChanged(user().id(),user().profilePicturePath());
                    //notify client ProfilePictureChanged
                    response.setAction(ProfilePictureChangedActionName)
                            .addExtras(ExtrasNames::clientProfilePicturePathExtras,fileLocation)
                            .addExtras(ExtrasNames::profilePictureFileAliasExtras,serverFiles().getFileName(user().profilePicturePath()))
                            .send();
                }
            }
            // qDebug() << QString("%1 updated a profile picture (path = %2)").arg(user().username()).arg(user().profilePicturePath());
        }
    }

}

void UserController::onActionGetUserInfoTriggered(Extras extras){

    const QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    auto response = user().response();
    response.setController(AuthenticationControllerName);

    if(appUsers().userRegistered(username)){
        auto usr = appUsers().getUser(username);
        response.setAction(UserInfoActionName)
                .addExtras(ExtrasNames::userInfoExtras,usr.getJsonUserInfo())
                .send();
    }else{
        response.setAction(UserNotFoundActionName)
                .addExtras(ExtrasNames::usernameExtras,username)
                .send();
    }

}

void UserController::onActionGetProfilePictureTriggered(Extras extras){
    auto response = user().response();
    response.setController(AuthenticationControllerName);

    QString username = extras.getStringExtras(ExtrasNames::usernameExtras);
    if(!appUsers().userRegistered(username)){
        response.setAction(ProfilePictureDownloadFailedActionName)
                .addExtras(ExtrasNames::ProfilePictureDownloadErrorExtras,(int)Errors::ProfilePictureDownloadError::UserNotFound)
                .addExtras(ExtrasNames::usernameExtras,username)
                .send();
        return;
    }

    QString path = appUsers().getUser(username).profilePicturePath();
    if(!path.isEmpty()){
        extras.addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,path);//set the file location for that user profile picture
        FILE_SENDER_SETUP_ACTION(username,0,0)
    }else{
        qDebug() <<"UserController::onActionGetProfilePictureTriggered"
                << username << " has not uploaded a profile picture yet";
        response.setAction(ProfilePictureDownloadFailedActionName)
                .addExtras(ExtrasNames::ProfilePictureDownloadErrorExtras,(int)Errors::ProfilePictureDownloadError::ProfilePictureNotUploadedYet)
                .addExtras(ExtrasNames::usernameExtras,username)
                .send();
    }

}

void UserController::onActionFetchUserNamesTriggered(Extras extras){
    const QString & fetchUserNamesRequestTag = extras.getStringExtras(ExtrasNames::fetchUserNamesRequestTagExtras);
    auto jarry = extras.getJsonArrayExtras(ExtrasNames::userIdsExtras);

    Extras usernames;
    foreach (auto value, jarry) {
        auto id = value.toString();
        auto username = appUsers().getUserName(id.toULong());
        usernames.addExtras(id,username);
    }
    user().response().setController(ConversationControllerName)
            .setAction(UserNamesReceivedActionName)
            .addExtras(ExtrasNames::jsonUserNamesExtras,usernames.getJsonExtras())
            .addExtras(ExtrasNames::fetchUserNamesRequestTagExtras,fetchUserNamesRequestTag)
            .send();
}

void UserController::onActionIsUserProfilePictureChangedTriggered(Extras extras){
    auto tag = extras.getStringExtras(ExtrasNames::isUserProfilePictureChangedRequestTagExtras);
    auto imgName = extras.getStringExtras("imgName");
    unsigned long userId = extras.getULongExtras(ExtrasNames::userIdExtras);

    UserModel model;
    auto path =     model.getUser(userId).profilePicturePath();
    bool changed = false;
    if(!path.isEmpty()){
        if(FileUtils::getFileName(path) != imgName){
            changed = true;
        }
    }

    user().response().setController(AuthenticationControllerName)
            .setAction(IsUserProfilePictureChangedResultsActionName)
            .addExtras(ExtrasNames::isUserProfilePictureChangedRequestTagExtras,tag)
            .addExtras("changed",changed)
            .send();
}



/**
 * @brief UserController::TriggerAction
 * @param data
 */
void UserController::triggerAction(MagabeData data)
{

    //connect signals to MagabeServerManagerSignalReceiver
    ConnectionPipe::connectSignals(RegisteredObjects::UserController::className,RegisteredObjects::UserController::Signals::newSignUp,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::newSignUp);
    ConnectionPipe::connectSignals(RegisteredObjects::UserController::className,RegisteredObjects::UserController::Signals::profilePictureUploadProgress,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::profilePictureUploadProgress);
    ConnectionPipe::connectSignals(RegisteredObjects::UserController::className,RegisteredObjects::UserController::Signals::profilePictureUploadError,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::profilePictureUploadError);
    ConnectionPipe::connectSignals(RegisteredObjects::UserController::className,RegisteredObjects::UserController::Signals::userProfilePictureChanged,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::userProfilePictureChanged);
    ConnectionPipe::connectSignals(RegisteredObjects::UserController::className,RegisteredObjects::UserController::Signals::clientLogedOut,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::clientLogedOut);


    if(enabled()){

        if(user().isClientAuthenticated() && (!user().isBlackListed())){
            triggerActionDoContinue(data);
        }else if((!user().isBlackListed())){
            QString action  = data.action();
            if(action == TO_CSTR(ActionAuthenticate) || action == TO_CSTR(ActionLogin) || action == TO_CSTR(ActionSignUp)){
                triggerActionDoContinue(data);
            }else{
                print("ALERT:  user must be Authenticated to access this action");
            }
        }else{
            print("ALERT:  user must be Authenticated  and not BlackListed to access this action");
        }

    }else{
        print("ALERT: "+name()+"  is Disabled");
    }

}

void UserController::requestProfilePicture()
{//CHECK IF THE FILE EXISTS ON DISC TOO
    bool hasProfilePicture = (!user().profilePicturePath().isEmpty());
    if(user().isRegistered() && (!user().isBlackListed()) &&(!hasProfilePicture)){
        wait(5); //wait 5sec before sending a request
        bool sent = user().response().setController(AuthenticationControllerName)
                .setAction(UploadAProfilePictureActionName)
                .addExtras("info","Please upload your profile picture")
                .send();
        if(sent)
            qDebug() << "UploadAProfilePicture request sent";
    }

}
