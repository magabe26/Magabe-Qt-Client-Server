/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include <QObject>
#include "controller.h"
#include <QMetaEnum>
#include "Helpers/errors.h"


class UserController : public Controller
{
    Q_OBJECT

    Q_ENUMS(Actions)

public:
    struct ExtrasNames{
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
        NEW_EXTRAS_NAME(userIds)
        NEW_EXTRAS_NAME(fetchUserNamesRequestTag)
        NEW_EXTRAS_NAME(jsonUserNames)
        NEW_EXTRAS_NAME(isUserProfilePictureChangedRequestTag)

    };


    enum Actions{
        ActionSignUp,
        ActionAuthenticate,
        ActionLogin,
        ActionLogOut,
        ActionProfilePictureUploading,
        ActionGetUserInfo,
        ActionGetProfilePicture,
        ActionFetchUserNames,
        ActionIsUserProfilePictureChanged
    };

    //must be here for good alignment
    REGISTER_ACTIONS

    //controllers
    NEW_CONTROLLER_NAME(Authentication)
    NEW_CONTROLLER_NAME(User)
    NEW_CONTROLLER_NAME(Conversation)

    //actions
    NEW_ACTION_NAME(SignUpFailed)
    NEW_ACTION_NAME(SignUpSucceeded)
    NEW_ACTION_NAME(AuthenticationProcessSucceded)
    NEW_ACTION_NAME(AuthenticationProcessFailed)
    NEW_ACTION_NAME(LoginSucceeded)
    NEW_ACTION_NAME(LoginFailed)
    NEW_ACTION_NAME(LogOutSucceeded)
    NEW_ACTION_NAME(LogOutFailed)
    NEW_ACTION_NAME(ProfilePictureUpload)//Authentication
    NEW_ACTION_NAME(ProfilePictureUploading)//UserController
    NEW_ACTION_NAME(UploadAProfilePicture)
    NEW_ACTION_NAME(ProfilePictureChanged)
    NEW_ACTION_NAME(UserInfo)
    NEW_ACTION_NAME(UserNotFound)
    NEW_ACTION_NAME(ProfilePictureDownloadFailed)
    NEW_ACTION_NAME(UserNamesReceived)
    NEW_ACTION_NAME(IsUserProfilePictureChangedResults)


    explicit UserController(QObject *parent);
    ~UserController();
    // Controller interface
    Controller *createNew();

signals:
    /*emited when a newsignup of a user completed sucessesfully,
     and authenticated*/
    void newSignUp(unsigned long userId,QString mgbSessionId);
    void profilePictureUploadProgress(unsigned long userId,QString fileBaseName
                                      ,QString fileExtension,QString fileLocation
                                      ,float percentageCompletion,quint64  uploaded
                                      ,quint64 fileSize);
    void profilePictureUploadError(int errorNo,QString errorString);
    void userProfilePictureChanged(unsigned long userId,QString filePath);
    void clientLogedOut(unsigned long userId,QString mgbSessionId);

public slots:

    /**
     * @brief onActionSignUpTriggered
     * @param extras =>
            ExtrasNames::usernameExtras
            ExtrasNames::firstNameExtras
            ExtrasNames::lastNameExtras
            ExtrasNames::dateOfBirthExtras
            ExtrasNames::genderExtras
            ExtrasNames::passwordExtras
            ExtrasNames::emailExtras
     */
    void onActionSignUpTriggered(Extras extras);

    /**
     * @brief onActionAuthenticateTriggered
     * @param extras =>
     *   ExtrasNames::userIdExtras
         ExtrasNames::usernameExtras
         ExtrasNames::authKeyExtras
         ExtrasNames::passwordExtras
     */
    void onActionAuthenticateTriggered(Extras extras);

    /**
     * @brief onActionLoginTriggered
     * @param extras =>
     *   ExtrasNames::usernameExtras
         ExtrasNames::passwordExtras

     */
    void onActionLoginTriggered(Extras extras);

    /**
     * @brief onActionLogOutTriggered
     * @param extras =>

     */
    void onActionLogOutTriggered(Extras extras);

    /**
     * @brief onActionProfilePictureUploadingTriggered
     * @param extras
     */
    void onActionProfilePictureUploadingTriggered(Extras extras);

    /**
     * @brief onActionGetUserInfoTriggered
     * @param extras => username
     */
    void onActionGetUserInfoTriggered(Extras extras);

    /**
     * @brief onActionGetProfilePictureTriggered
     * @param extras => ExtrasNames::usernameExtras;
     */
    void onActionGetProfilePictureTriggered(Extras extras);

    /**
     * @brief onActionFetchUserNamesTriggered
     * @param extras => ExtrasNames::fetchUserNamesRequestTagExtras
                         ExtrasNames::userIdsExtras
     */
    void onActionFetchUserNamesTriggered(Extras extras);

    /**
     * @brief onActionIsUserProfilePictureChangedTriggered
     * @param extras
     */
    void onActionIsUserProfilePictureChangedTriggered(Extras extras);

private slots:
    // Controller interface
    void triggerAction(MagabeData data);//data must be a copy
private:
    constexpr static const char * profilePictureFolder = "Profile_Pictures";
    void requestProfilePicture();
};



#endif // USERCONTROLLER_H
