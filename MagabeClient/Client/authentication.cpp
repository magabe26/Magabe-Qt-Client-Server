/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "authentication.h"
#include "Helpers/datawritter.h"
#include "Models/userprofilepicturemodel.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include "Models/authenticationmodel.h"
#include "Models/currentuseridmodel.h"


Authentication::Authentication(QObject *parent) : QObject(parent)
{

}

bool Authentication::isUserProfilePictureExist(const QString &userName){
    UserProfilePictureModel model;
    QString path = model.getPath(userName);
    return (!path.isEmpty() && isFileExists(path));
}


bool Authentication::sendRequest(MagabeData data)
{
    bool invocked =  QMetaObject::invokeMethod(DWritter::instance()
                                               ,"sendData"
                                               ,Q_ARG(QString,data.toByteArray()));
    if(!invocked){
        qWarning() << "Authentication::sendRequest :: Critical Error , Invocation Failed" ;
    }
    return invocked;
}


bool Authentication::isFileExists(const QString &absoluteFilePath){
    return (QFile::exists(absoluteFilePath));
}


void Authentication::login(const QString& username,const QString& password)
{
    print("---> login");

    MagabeData	loginCredentials;
    loginCredentials.setController(UserControllerName)
            .setAction(LoginActionName)
            .addExtras(ExtrasNames::usernameExtras,username)
            .addExtras(ExtrasNames::passwordExtras,password);

    sendRequest(loginCredentials);
}

void Authentication::signUp(const QString& username,const QString& firstName
                            ,const QString& lastName,const QString& dateOfBirth,QChar gender
                            ,const QString& password,const QString& email)
{
    print("---> signUp");
    MagabeData signUpDetails;
    signUpDetails.setController(UserControllerName)
            .setAction(SignUpActionName)
            .addExtras(ExtrasNames::usernameExtras,username)
            .addExtras(ExtrasNames::firstNameExtras,firstName)
            .addExtras(ExtrasNames::lastNameExtras,lastName)
            .addExtras(ExtrasNames::dateOfBirthExtras,dateOfBirth)
            .addExtras(ExtrasNames::genderExtras,gender)
            .addExtras(ExtrasNames::passwordExtras,password)
            .addExtras(ExtrasNames::emailExtras,email);

    sendRequest(signUpDetails);

}

void Authentication::isProfilePictureChanged()
{
    MagabeData data;
    if(isFileExists(getProfilePictureFilePath())){

        data.setController(UserControllerName)
                .setAction("ActionIsProfilePictureChanged")
                .addExtras("newFileName",getFileName(getProfilePictureFilePath()));

    }else{
        data.setController(UserControllerName)
                .setAction(GetProfilePictureActionName);

    }

    sendRequest(data);
}


void Authentication::logOut()
{
    print("--- signOut");

    MagabeData request;
    request.setController(UserControllerName)
            .setAction(LogOutActionName)
            .addExtras("info","i want to signOut");


    sendRequest(request);

}


void  Authentication::getMyUserInfo()
{
    CurrentUserIdModel currentUser;
    int userId = currentUser.getCurrentId();

    print("--- getMyUserInfo");

    MagabeData data;
    data.setController(UserControllerName)
            .setAction("ActionUserInfo")
            .addExtras("userId",userId);

    sendRequest(data);
}


void  Authentication::upDateUserInfo(QString firstName, QString lastName, QString dateOfBirth, QString phonenumber, QString education, QString position, QString location)
{
    print("--- upDateUserInfo");

    MagabeData data;
    data.setController(UserControllerName)
            .setAction("ActionUpdateUserProfile")
            .addExtras("first_name",firstName.toUpper())
            .addExtras("last_name",lastName.toUpper())
            .addExtras("date_of_birth",dateOfBirth)
            .addExtras("Phone_number",phonenumber)
            .addExtras("Education",education.toUpper())
            .addExtras("Position",position.toUpper())
            .addExtras("Location",location.toUpper());

    sendRequest(data);
}


void Authentication::print(QString status)
{
    qDebug()<< status <<"\n";
}


void Authentication::saveProfilePictureFilePath(QString filePath)
{
    QSettings settings;
    settings.beginGroup("MagabeClient");
    settings.setValue("userProfilePath", filePath);
    settings.endGroup();
}



QString  Authentication::getProfilePictureFilePath()
{
    QSettings settings;
    settings.beginGroup("MagabeClient");
    QString path = settings.value("userProfilePath").toString();
    settings.endGroup();

    return path;
}

QString Authentication::getCompleteBaseName(QString filePath)
{
    QFileInfo info(filePath);
    return info.completeBaseName();

}

bool Authentication::authenticate()
{
    CurrentUserIdModel  currentUserIdModel;

    if(currentUserIdModel.idExist()){

        AuthenticationModel authenticationModel;
        unsigned long id = currentUserIdModel.getCurrentId();
        QString username = authenticationModel.getUserName(id);
        QString authKey = authenticationModel.getAuthKey(id);
        QString password = authenticationModel.getPassword(id);

        //autheticate
        MagabeData autheticationDetails;
        autheticationDetails.setController(UserControllerName)
                .setAction(AuthenticateActionName)
                .addExtras(ExtrasNames::userIdExtras,id)
                .addExtras(ExtrasNames::usernameExtras,username)
                .addExtras(ExtrasNames::authKeyExtras,authKey)
                .addExtras(ExtrasNames::passwordExtras,password);

        return sendRequest(autheticationDetails);

    }else{
        return false;
    }
}

bool Authentication::isLogedIn()
{
    CurrentUserIdModel  currentUserIdModel;
    return currentUserIdModel.idExist();
}


QString Authentication::getFileExtension(QString filePath)
{
    QFileInfo info(filePath);
    return info.suffix();
}


QString Authentication::getFileName(QString filePath)
{
    return getCompleteBaseName(filePath)+"."+getFileExtension(filePath);
}


QString Authentication::getFileName(QString fileBaseName, QString fileExtension)
{
    return fileBaseName + "." + fileExtension;
}













