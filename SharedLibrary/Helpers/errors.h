/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef ERRORS_H
#define ERRORS_H

#include <QObject>

class Errors : public QObject
{
    Q_OBJECT

public:
    enum class AuthenticationError{//do not change order
        NoError = 0,
        UserAlreadyAuthenticated = 0x7e3, //2019
        IllegalAppUsersState = 0x7e4,//2020 not used incase of client
        UserNotRegistered = 0x7e5, //2021
        UserIsBlacklisted = 0x7e6, //2022
        UserNameNotSet = 0x7e7,//2023
        AuthKeyNotSet = 0x7e8,//2024
        PasswordNotSet = 0x7e9,//2025
        InvalidCredentials = 0x7ea,//2026
        UnknownError = 0x13ba //5050
    };

    enum class SignUpError{//do not change order
        NoError = 0,
        UsernameHasLessThan6Characters = 0x7c, //124
        WeakPassword = 0x7d,//125
        UserAlreadyRegistered = 0x7e,//126
        InvalidEmail = 0x7f,//127
        EmailAlreadyTaken = 0x80,//128
        InvalidGender = 0x81,//129
        FirstNameNotSet = 0x82, //130
        LastNameNotSet = 0x83,//131
        InvalidDateOfBirth = 0x84,//132
        UnknownError = 0x13ba //5050

    };

    enum class UserInfoError{
        NoError = 0,
        UserNotFound = 0x19f,//415
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba //5050
    };

    enum class LogOutError{
        NoError = 0,
        UserNotAuthenticated = 0xd62,//3426
        UnknownError = 0x13ba //5050
    };

    enum class ProfilePictureDownloadError{
        NoError = 0,
        UserNotFound = 0x19f,//415
        ProfilePictureNotUploadedYet = 0x1a0,//146
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba //5050
    };

    enum class OnGetMessagesError{
        NoError = 0,
        UserNotAuthenticated = 0xd62,//3426
        ConversationNotFound = 0xd63,//3427
        MessageNotFound = 0xd64,//3428
        UserNotInvolvedInConversation = 0xd65,//3429
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba ,//5050
        InvalidMessageIdentifier = 0x100f, //4111
        InvalidConversationIdentifier = 0x1e3d //7741


    };

    enum class OnSendMessageError{
        NoError = 0,
        UserNotAuthenticated = 0xd62,//3426
        UserNotFound = 0x19f,//415
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba, //5050
        InvalidMessageIdentifier = 0x100f, //4111
        InvalidConversationIdentifier = 0x1e3d //7741
    };

    enum class OnGetMessageCountError{
        NoError = 0,
        UserNotAuthenticated = 0xd62,//3426
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba, //5050
        InvalidConversationIdentifier = 0x1e3d //7741
    };


    enum class OnGetLastMessageError{
        NoError = 0,
        UserNotAuthenticated = 0xd62,//3426
        ConnectionErrors = 0x14b, //331
        UnknownError = 0x13ba, //5050
        InvalidConversationIdentifier = 0x1e3d //7741
    };

    Q_ENUM(AuthenticationError)
    Q_ENUM(SignUpError)
    Q_ENUM(UserInfoError)
    Q_ENUM(LogOutError)
    Q_ENUM(ProfilePictureDownloadError)
    Q_ENUM(OnGetMessagesError)
    Q_ENUM(OnSendMessageError)

    explicit Errors(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ERRORS_H
