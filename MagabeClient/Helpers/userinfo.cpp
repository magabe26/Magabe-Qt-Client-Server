/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "userinfo.h"
#include <QJsonObject>
#include <QJsonDocument>

UserInfo::UserInfo(QObject* parent)
    :QObject(parent)
{

}

UserInfo::UserInfo(UserInfo &&other){
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->dateOfBirth_ = other.dateOfBirth_;
    this->gender_ = other.gender_;
    this->username_ = other.username_;
    this->email_ = other.email_;
    this->createdAt_ = other.createdAt_;
    this->updatedAt_ = other.updatedAt_;
    this->profilePicture_ = profilePicture_;
    this->education_ = other.education_;
    this->position_ = other.position_;
    this->location_ = other.location_;
    this->phoneNumber_ = other.phoneNumber_;
}

UserInfo::UserInfo(const UserInfo &other){
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->dateOfBirth_ = other.dateOfBirth_;
    this->gender_ = other.gender_;
    this->username_ = other.username_;
    this->email_ = other.email_;
    this->createdAt_ = other.createdAt_;
    this->updatedAt_ = other.updatedAt_;
    this->profilePicture_ = profilePicture_;
    this->education_ = other.education_;
    this->position_ = other.position_;
    this->location_ = other.location_;
    this->phoneNumber_ = other.phoneNumber_;
}

UserInfo &UserInfo::operator =(const UserInfo &other){
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->dateOfBirth_ = other.dateOfBirth_;
    this->gender_ = other.gender_;
    this->username_ = other.username_;
    this->email_ = other.email_;
    this->createdAt_ = other.createdAt_;
    this->updatedAt_ = other.updatedAt_;
    this->profilePicture_ = profilePicture_;
    this->education_ = other.education_;
    this->position_ = other.position_;
    this->location_ = other.location_;
    this->phoneNumber_ = other.phoneNumber_;
    return *this;
}
unsigned long UserInfo::id() const
{
    return id_;
}

void UserInfo::setId(unsigned long id)
{
    id_ = id;
}

QString UserInfo::firstName() const
{
    return firstName_;
}

void UserInfo::setFirstName(const QString &firstName)
{
    firstName_ = firstName;
}

QString UserInfo::lastName() const
{
    return lastName_;
}

void UserInfo::setLastName(const QString &lastName)
{
    lastName_ = lastName;
}

QString UserInfo::dateOfBirth() const
{
    return dateOfBirth_;
}

void UserInfo::setDateOfBirth(const QString &dateOfBirth)
{
    dateOfBirth_ = dateOfBirth;
}

UserInfo::Gender UserInfo::gender() const
{
    return gender_;
}

void UserInfo::setGender(const Gender &gender)
{
    gender_ = gender;
}

QString UserInfo::username() const
{
    return username_;
}

void UserInfo::setUsername(const QString &username)
{
    username_ = username;
}

QString UserInfo::email() const
{
    return email_;
}

void UserInfo::setEmail(const QString &email)
{
    email_ = email;
}

qint64 UserInfo::createdAt() const
{
    return createdAt_;
}

void UserInfo::setCreatedAt(const qint64 &createdAt)
{
    createdAt_ = createdAt;
}

qint64 UserInfo::updatedAt() const
{
    return updatedAt_;
}

void UserInfo::setUpdatedAt(const qint64 &updatedAt)
{
    updatedAt_ = updatedAt;
}

QString UserInfo::profilePicture() const
{
    return profilePicture_;
}

void UserInfo::setProfilePicture(const QString &profilePicture)
{
    profilePicture_ = profilePicture;
}

QString UserInfo::education() const
{
    return education_;
}

void UserInfo::setEducation(const QString &education)
{
    education_ = education;
}

QString UserInfo::position() const
{
    return position_;
}

void UserInfo::setPosition(const QString &position)
{
    position_ = position;
}

QString UserInfo::location() const
{
    return location_;
}

void UserInfo::setLocation(const QString &location)
{
    location_ = location;
}

QString UserInfo::phoneNumber() const
{
    return phoneNumber_;
}

UserInfo UserInfo::fromJson(const QString json)
{
    UserInfo info;
    auto doc = QJsonDocument::fromJson(json.toLatin1());
    if(doc.isNull() || doc.isEmpty()){
        return info;
    }
    QJsonObject jobj = doc.object();
    if(!jobj.isEmpty()){
        info.setId(jobj.value("id").toString().toUInt());
        info.setFirstName(jobj.value("firstName").toString());
        info.setLastName(jobj.value("lastName").toString());
        info.setDateOfBirth(jobj.value("dateOfBirth").toString());
        info.setGender((jobj.value("gender").toString().trimmed().toUpper().at(0) == 'M') ? Gender::MALE :Gender::FEMALE);
        info.setUsername(jobj.value("username").toString());
        info.setEmail(jobj.value("email").toString());
        info.setCreatedAt(jobj.value("createdAt").toString().toULongLong());
        info.setUpdatedAt(jobj.value("updatedAt").toString().toULongLong());
        info.setProfilePicture(jobj.value("profilePicture").toString());
        info.setEducation(jobj.value("education").toString());
        info.setPosition(jobj.value("position").toString());
        info.setLocation(jobj.value("location").toString());
        info.setPhoneNumber(jobj.value("phoneNumber").toString());
    }
    return info;
}

QString UserInfo::toJson()
{
    QJsonObject jobj;
    jobj.insert("id",QString::number(id_));
    jobj.insert("firstName",firstName_);
    jobj.insert("lastName",lastName_);
    jobj.insert("dateOfBirth",dateOfBirth_);
    jobj.insert("gender",(gender_ == Gender::MALE ? "M" : "F"));
    jobj.insert("username",username_);
    jobj.insert("email",email_);
    jobj.insert("createdAt",createdAt_);
    jobj.insert("updatedAt",updatedAt_);
    jobj.insert("profilePicture",profilePicture_);
    jobj.insert("education",education_);
    jobj.insert("position",position_);
    jobj.insert("location",location_);
    jobj.insert("phoneNumber",phoneNumber_);

    return  QJsonDocument(jobj).toJson().data();
}

bool UserInfo::operator ==(const UserInfo &other)
{
    return ((this->id_ == other.id_)
            && (this->firstName_ == other.firstName_)
            && (this->lastName_ == other.lastName_)
            && (this->dateOfBirth_ == other.dateOfBirth_)
            && (this->gender_ == other.gender_)
            && (this->username_ == other.username_)
            && (this->email_ == other.email_)
            && (this->createdAt_ == other.createdAt_)
            && (this->updatedAt_ == other.updatedAt_)
            && (this->profilePicture_ == profilePicture_)
            && (this->education_ == other.education_)
            && (this->position_ == other.position_)
            && (this->location_ == other.location_)
            && (this->phoneNumber_ == other.phoneNumber_));
}

void UserInfo::setPhoneNumber(const QString &phoneNumber)
{
    phoneNumber_ = phoneNumber;
}
