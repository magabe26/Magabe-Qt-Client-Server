/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef AUTHENTICATIONCONTROLLER_H
#define AUTHENTICATIONCONTROLLER_H

#include <QObject>
#include "controller.h"
#include "Helpers/magabedata.h"
#include <QMetaEnum> //important for controller class
#include <QDebug>
#include "Helpers/constants.h"
#include <QDir>
#include <QFile>
#include "Models/currentuseridmodel.h"
#include "Models/authenticationmodel.h"
#include "Models/userprofilepicturemodel.h"
#include "Helpers/errors.h"

class AuthenticationController : public Controller
{
    Q_OBJECT

    Q_ENUMS(Actions)

public:
    struct ExtrasNames{
        NEW_EXTRAS_NAME(callbackController)
        NEW_EXTRAS_NAME(callbackAction)
        NEW_EXTRAS_NAME(filePosition)
        NEW_EXTRAS_NAME(ActionGetFileError)
        NEW_EXTRAS_NAME(fileLocation)
        NEW_EXTRAS_NAME(userId)
        NEW_EXTRAS_NAME(username)
        NEW_EXTRAS_NAME(firstName)
        NEW_EXTRAS_NAME(lastName)
        NEW_EXTRAS_NAME(authKey)
        NEW_EXTRAS_NAME(password)
        NEW_EXTRAS_NAME(dateOfBirth)
        NEW_EXTRAS_NAME(gender)
        NEW_EXTRAS_NAME(email)
        NEW_EXTRAS_NAME(SignUpError)
        NEW_EXTRAS_NAME(errorString)
        NEW_EXTRAS_NAME(AuthenticationError)
        NEW_EXTRAS_NAME(newAuthKey)
        NEW_EXTRAS_NAME(LogOutError)
        NEW_EXTRAS_NAME(clientProfilePicturePath)
        NEW_EXTRAS_NAME(profilePictureFileAlias)
        NEW_EXTRAS_NAME(userInfo)
        NEW_EXTRAS_NAME(ProfilePictureDownloadError)
        NEW_EXTRAS_NAME(profilePictureTag)
        NEW_EXTRAS_NAME(isUserProfilePictureChangedRequestTag)

    };

    enum Actions{
        ActionAuthenticationRequired,
        ActionAuthenticationProcessFailed,
        ActionAuthenticationProcessSucceded,
        ActionLoginSucceeded,
        ActionLoginFailed,
        ActionSignUpSucceeded,
        ActionSignUpFailed,
        ActionUserInfo,
        ActionLogOutSucceeded,
        ActionLogOutFailed,
        ActionProfilePictureUpload,
        ActionStartProfilePictureUploading,
        ActionUploadAProfilePicture,
        ActionProfilePictureChanged,
        ActionUserNotFound,
        ActionStartProfilePictureDownloading,
        ActionDownloadProfilePicture,
        ActionProfilePictureDownloadFailed,
        ActionIsUserProfilePictureChangedResults
    };



    REGISTER_ACTIONS

    NEW_CONTROLLER_NAME(User)
    NEW_CONTROLLER_NAME(Authentication)

    NEW_ACTION_NAME(ProfilePictureUploading)//UserController
    NEW_ACTION_NAME(GetProfilePicture)//UserController


    explicit AuthenticationController(QObject *parent = nullptr);
    static QString authenticationErrorToString(Errors::AuthenticationError authenticationError);
    static constexpr const char *getProfilePicturesFolder(){
        return "ProfilePictures";
    }

    // Controller interface
    Controller *createNew();

signals:
    //A login interface must connect to this signal, in this case MApp
    void loginRequired();
    void signUpSucceeded(unsigned long id,QString username);
    void signUpFailed(int signUpError,QString errorString);
    void authenticationSucceded(unsigned long id,QString username);
    void authenticationFailed(int authenticationError);
    void loginSucceeded(unsigned long id,QString username);
    void loginFailed(int authenticationError);
    void logOutSucceeded();
    void logOutFailed(int logOutError);
    void profilePictureRequired();
    void profilePictureUploaded(QString tag,QString fileName,QString filePath);
    void profilePictureUploadingFailed(QString tag,QString fileName,QString filePath,QString errorString);
    void profilePictureUploadingNotFinished(QString tag,QString fileName,QString filePath);//not reliable
    void profilePictureUploadProgress(QString tag,QString filePath,float percentageCompletion,quint64 fileSize);
    void profilePictureChanged(QString imagePath);
    void userInfo(QString json);
    void userNotFound(QString username);
    void userProfilePictureDownloadFinished(QString username,QString imagePath);
    void profilePictureDownloadFailed(QString username,int profilePictureDownloadError);
    void profilePictureDownloadProgress(QString fileName,float percentageCompletion,quint64 fileSize,QString fileTag);
    void isUserProfilePictureChanged(bool changed,QString tag);

public slots:
    /**
     * @brief onActionAuthenticationRequiredTriggered
     * @param extras => empty
     */
    void onActionAuthenticationRequiredTriggered(Extras extras);

    /**
     * @brief onActionAuthenticationProcessFailedTriggered
     * @param extras =>
     * ExtrasNames::AuthenticationErrorExtras
     */
    void onActionAuthenticationProcessFailedTriggered(Extras extras);
    /**
     * @brief onActionAuthenticationProcessSuccededTriggered
     * @param extras =>
     *   ExtrasNames::newAuthKeyExtras
     */
    void onActionAuthenticationProcessSuccededTriggered(Extras extras);
    /**
     * @brief onActionLoginSucceededTriggered
     * @param extras =>
     *  ExtrasNames::userIdExtras
        ExtrasNames::usernameExtras
        ExtrasNames::authKeyExtras
        ExtrasNames::passwordExtras
     */
    void onActionLoginSucceededTriggered(Extras extras);

    /**
     * @brief onActionLoginFailedTriggered
     * @param extras =>
     * ExtrasNames::AuthenticationErrorExtras
     */
    void onActionLoginFailedTriggered(Extras extras);
    /**
     * @brief onActionSignUpSucceededTriggered
     * @param extras =>
          ExtrasNames::userIdExtras
          ExtrasNames::usernameExtras
          ExtrasNames::authKeyExtras
          ExtrasNames::passwordExtras
     */
    void onActionSignUpSucceededTriggered(Extras extras);

    /**
     * @brief onActionSignUpFailedTriggered
     * @param extras =>
         ExtrasNames::SignUpErrorExtras
         ExtrasNames::ErrorStringExtras
     */
    void onActionSignUpFailedTriggered(Extras extras);

    /**
     * @brief onActionLogOutSucceededTriggered
     * @param extras
     */
    void onActionLogOutSucceededTriggered(Extras extras);

    /**
     * @brief onActionLogOutFailedTriggered
     * @param extras => LogOutError

     */
    void onActionLogOutFailedTriggered(Extras extras);

    /**
     * @brief onActionUploadAProfilePictureTriggered
     * @param extras
     */
    void onActionUploadAProfilePictureTriggered(Extras extras);

    /**
     * @brief onActionProfilePictureUploadTriggered
     * @param extras
     */
    void onActionProfilePictureUploadTriggered(Extras extras);

    /**
     * @brief onActionProfilePictureChanged
     * @param extras =>
     * clientProfilePicturePath
        profilePictureFileAlias
     */
    void onActionProfilePictureChangedTriggered(Extras extras);

    /**
     * @brief onActionUserInfoTriggered
     * @param extras => ExtrasNames::userInfoExtras
     */
    void onActionUserInfoTriggered(Extras extras);

    /**
     * @brief onActionUserNotFoundTriggered
     * @param extras => ExtrasNames::usernameExtras
     */
    void onActionUserNotFoundTriggered(Extras extras);
    /**
     * @brief onActionDownloadProfilePictureTriggered
     * @param extras
     */
    void onActionDownloadProfilePictureTriggered(Extras extras);
    /**
     * @brief onActionProfilePictureDownloadFailedTriggered
     * @param extras => ExtrasNames::ProfilePictureDownloadErrorExtras
                        ExtrasNames::usernameExtras
     */
    void onActionProfilePictureDownloadFailedTriggered(Extras extras);

    /**
     * @brief onActionIsUserProfilePictureChangedResultsTriggered
     * @param extras
     */
    void onActionIsUserProfilePictureChangedResultsTriggered(Extras extras);

private:
    CurrentUserIdModel currentUserModel_;
    AuthenticationModel authenticationModel_;
    UserProfilePictureModel userProfilePictureModel_;
    void clearCurrentUserInfo();

private slots:
    // Controller interface
    void triggerAction(MagabeData data);
    //for internal use only
    void onActionStartProfilePictureUploadingTriggered(Extras extras);
    void onActionStartProfilePictureDownloadingTriggered(Extras extras);
};


#endif // AUTHENTICATIONCONTROLLER_H
















//    void onActionDownloadTriggered(Extras extras){

//        auto dCallback  = [=]
//                (QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  downloaded,quint64 fileSize){
//            Q_UNUSED(downloaded)
//            Q_UNUSED(fileBaseName)
//            Q_UNUSED(fileExtension)
//            Q_UNUSED(fileLocation)
//            auto status = QStringLiteral("downloading ... ")+QString("%1")
//                    .arg(percentageCompletion)
//                    +"%"+" of "+QString("%1 MB").
//                    arg(fileSize/(1024*1024));
//            qDebug() << status;
//        };

//        FILE_DOWNLOAD_SETUP_ACTION(UserControllerName /*FILESENDER_CONTROLLER*/
//                                   ,"ActionGetFile" /*FILESENDER_ACTION*/
//                                   ,AuthenticationControllerName /*CALLBACK_CONTROLLER*/
//                                   ,TO_CSTR(ActionDownload) /*CALLBACK_ACTION*/
//                                   ,"TEST"  /*SAVE_FOLDER*/
//                                   ,dCallback/*DOWNLOAD_PROGRESS_CALLBACK*/
//                                   ,0 /*ON_ERROR_CALLBACK*/
//                                   )

//        {
//            if(downloadCompleted){
//                qDebug() << "download completed " << filePath;
//            }else{
//                qDebug() << "download not completed ";

//            }
//        }
//    }






