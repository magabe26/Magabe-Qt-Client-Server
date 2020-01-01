/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABECLIENTHELPER_H
#define MAGABECLIENTHELPER_H

#include <QObject>
#include "Helpers/magabedata.h"
#include "Controllers/controller.h"


class Authentication : public QObject
{
    Q_OBJECT


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
    };


    NEW_CONTROLLER_NAME(User)

    NEW_ACTION_NAME(SignUp)
    NEW_ACTION_NAME(Authenticate)
    NEW_ACTION_NAME(Login)
    NEW_ACTION_NAME(LogOut)
    NEW_ACTION_NAME(GetProfilePicture)


    explicit Authentication(QObject *parent = nullptr);
    static bool sendRequest(MagabeData data);
    static  bool isUserProfilePictureExist(const QString &userName);
    static bool isFileExists(const QString &absoluteFilePath);
    static void login(const QString &username, const QString &password);
    static void logOut();
    static void signUp(const QString& username,const QString& firstName
                       ,const QString& lastName,const QString& dateOfBirth,QChar gender
                       ,const QString& password,const QString& email);

    static void upDateUserInfo(QString firstName, QString lastName, QString dateOfBirth, QString phonenumber, QString education, QString position, QString location);
    static void getMyUserInfo();
    static void isProfilePictureChanged();
    static QString  getProfilePictureFilePath();
    static void saveProfilePictureFilePath(QString filePath);
    static QString getFileName(QString filePath);
    static QString getFileName(QString fileBaseName, QString fileExtension);
    static QString getFileExtension(QString filePath);
    static QString getCompleteBaseName(QString filePath);
    /* return true if the request is sent , or false when uer is not loged in */
    static bool authenticate();
    static bool isLogedIn();
private:
    static void print(QString status);
};

#endif // MAGABECLIENTHELPER_H
