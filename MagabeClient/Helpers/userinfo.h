/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef USERINFO_H
#define USERINFO_H
#include <QString>
#include <QObject>

class MagabeClientApp;
class UserInfoCallbacks;

class UserInfo: public QObject
{

    Q_OBJECT

    Q_PROPERTY(unsigned long id READ id)
    Q_PROPERTY(QString firstName READ firstName)
    Q_PROPERTY(QString lastName READ lastName)
    Q_PROPERTY(QString dateOfBirth READ dateOfBirth)
    Q_PROPERTY(Gender gender READ gender)
    Q_PROPERTY(QString username READ username)
    Q_PROPERTY(QString email READ email)
    Q_PROPERTY(QString phoneNumber READ phoneNumber)
    Q_PROPERTY(qint64 createdAt READ createdAt)
    Q_PROPERTY(qint64 updatedAt READ updatedAt)
    Q_PROPERTY(QString profilePicture READ profilePicture)
    Q_PROPERTY(QString position READ position)
    Q_PROPERTY(QString education READ education)
    Q_PROPERTY(QString location READ location)

public:

    enum  Gender { MALE = 0x7c5,FEMALE };
    Q_ENUM(Gender)

    UserInfo(QObject *parent = nullptr);
    UserInfo(UserInfo&& other);
    UserInfo(const UserInfo &other);
    unsigned long id() const;
    QString firstName() const;
    QString lastName() const;
    QString dateOfBirth() const;
    Gender gender() const;
    QString username() const;
    QString email() const;
    qint64 createdAt() const;
    qint64 updatedAt() const;
    QString profilePicture() const;
    QString education() const;
    QString position() const;
    QString location() const;
    QString phoneNumber() const;
    static UserInfo fromJson(const QString json);
    QString toJson();
    bool operator ==(const UserInfo &other);
    UserInfo &operator =(const UserInfo &other);

private:
    friend class MagabeClientApp;
    friend class UserInfoCallbacks;
    friend class MagabeClientManager;
    unsigned long id_;
    QString firstName_;
    QString lastName_;
    QString dateOfBirth_;
    Gender gender_;
    QString username_;
    QString email_;
    qint64 createdAt_;
    qint64 updatedAt_;
    QString profilePicture_;
    QString education_;
    QString position_;
    QString location_;
    QString phoneNumber_;
    void setId(unsigned long id);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setDateOfBirth(const QString &dateOfBirth);
    void setGender(const Gender &gender);
    void setUsername(const QString &username);
    void setEmail(const QString &email);
    void setCreatedAt(const qint64 &createdAt);
    void setUpdatedAt(const qint64 &updatedAt);
    void setProfilePicture(const QString &profilePicture);
    void setEducation(const QString &education);
    void setPosition(const QString &position);
    void setLocation(const QString &location);
    void setPhoneNumber(const QString &phoneNumber);

};



#endif // USERINFO_H
