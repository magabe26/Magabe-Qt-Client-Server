/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */



#include "user.h"
#include "debug.h"
#include "Controllers/controller.h"
#include <QSslSocket>
#include <QDebug>
#include <QHostAddress>
#include <QDateTime>
#include "Server/connectedusers.h"
#include "Helpers/connectionpipe.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "Helpers/fileutils.h"

User::User(): QObject(nullptr)
  ,thisClientSocket_(nullptr)
  ,thisClientController_(nullptr)
  ,id_(0){}

User::~User(){
    thisClientSocket_ = nullptr;
    thisClientController_ = nullptr;
}

bool User::operator==(const User &user){
    return((this->username_ == user.username_)
           && (this->firstName_ == user.firstName_)
           && (this->lastName_ == user.lastName_)
           && (this->gender_ == user.gender_)
           && (this->id_ == user.id_)
           && (this->email_ == user.email_)
           && (this->phoneNumber_ == user.phoneNumber_));
}

User::User(User &&other){
    *this = other;
}

User::User(const User &other): QObject(nullptr){
    *this = other;
}

User &User::operator =(User &&other){
    this->thisClientSocket_ = other.thisClientSocket_;
    this->id_ = other.id_;
    this->firstName_ = std::move(other.firstName_);
    this->lastName_ = std::move(other.lastName_);
    this->dateOfBirth_ = std::move(other.dateOfBirth_);
    this->gender_ = other.gender_;
    this->username_ = std::move(other.username_);
    this->authKey_ = std::move(other.authKey_);
    this->password_ = std::move(other.password_);
    this->email_ = std::move(other.email_);
    this->isActive_ = other.isActive_;
    this->isBlackListed_ = other.isBlackListed_;
    this->createdAt_ = other.createdAt_;
    this->updatedAt_ = other.updatedAt_;
    this->profilePicturePath_ = std::move(other.profilePicturePath_);
    this->education_ = std::move(other.education_);
    this->position_ = std::move(other.position_);
    this->location_ = std::move(other.location_);
    this->phoneNumber_ = std::move(other.phoneNumber_);
    this->thisClientMgbSessionId_ = std::move(other.thisClientMgbSessionId_);
    this->thisClientController_ = other.thisClientController_;

    return *this;
}


User &User::operator =(const User &other){
    this->thisClientSocket_ = other.thisClientSocket_;
    this->id_ = other.id_;
    this->firstName_ = other.firstName_;
    this->lastName_ = other.lastName_;
    this->dateOfBirth_ = other.dateOfBirth_;
    this->gender_ = other.gender_;
    this->username_ = other.username_;
    this->authKey_ = other.authKey_;
    this->password_ = other.password_;
    this->email_ = other.email_;
    this->isActive_ = other.isActive_;
    this->isBlackListed_ = other.isBlackListed_;
    this->createdAt_ = other.createdAt_;
    this->updatedAt_ = other.updatedAt_;
    this->profilePicturePath_ = other.profilePicturePath_;
    this->education_ = other.education_;
    this->position_ = other.position_;
    this->location_ = other.location_;
    this->phoneNumber_ = other.phoneNumber_;
    this->thisClientMgbSessionId_ = other.thisClientMgbSessionId_;
    this->thisClientController_ = other.thisClientController_;

    return *this;
}


bool User::closethisClientConnection(){
    if(thisClientController_){
        emit thisClientController_->closeClientSocket(thisClientMgbSessionId_);
        return true;
    }else{
        if(!thisClientMgbSessionId_.isEmpty()){
            return ConnectionPipe::emitSignal("ControllerIdentifier"
                                              ,"closeClientSocket"
                                              ,Q_ARG(QString,thisClientMgbSessionId_));
        }else
            return false;
    }
}

QString User::firstName() const
{
    return firstName_;
}

QString User::lastName() const
{
    return lastName_;
}

QString User::dateOfBirth() const
{
    return dateOfBirth_;
}

User::Gender User::gender() const
{
    return gender_;
}

QString User::username() const
{
    return username_;
}

QString User::authKey() const
{
    return authKey_;
}

QString User::password() const
{
    return password_;
}

QString User::email() const
{
    return email_;
}

bool User::isActive() const
{
    return isActive_;
}

bool User::isBlackListed() const
{
    return isBlackListed_;
}

qint64 User::createdAt() const
{
    return createdAt_;
}

qint64 User::updatedAt() const
{
    return updatedAt_;
}

QString User::profilePicturePath() const
{
    return profilePicturePath_;
}

QString User::education() const
{
    return education_;
}

QString User::position() const
{
    return position_;
}

QString User::location() const
{
    return location_;
}

QString User::phoneNumber() const
{
    return phoneNumber_;
}

QString User::getThisClientMgbSessionId() const
{
    return thisClientMgbSessionId_;
}

unsigned long User::id() const
{
    return id_;
}

bool User::thisClientConnectionIsActive(){
    return  (thisClientSocket_)? (thisClientSocket_->state() == QAbstractSocket::ConnectedState) : false;
}

QString User::getJsonUserInfo(){
    QJsonObject jobj;
    const QString genderStr  =  (gender_ == User::Gender::MALE) ? "M" : "F";

    jobj.insert("id",QJsonValue(QString::number(id_)));
    jobj.insert("firstName",QJsonValue(firstName_));
    jobj.insert("lastName",QJsonValue(lastName_));
    jobj.insert("dateOfBirth",QJsonValue(dateOfBirth_));
    jobj.insert("gender",QJsonValue(genderStr));
    jobj.insert("username",QJsonValue(username_));
    jobj.insert("email",QJsonValue(email_));
    jobj.insert("createdAt",QJsonValue(createdAt_));
    jobj.insert("updatedAt",QJsonValue(updatedAt_));
    jobj.insert("profilePicture",QJsonValue(FileUtils::getFileName(profilePicturePath_)));
    jobj.insert("education",QJsonValue(education_));
    jobj.insert("position",QJsonValue(position_));
    jobj.insert("location",QJsonValue(location_));
    jobj.insert("phoneNumber",QJsonValue(phoneNumber_));
    QJsonDocument doc(jobj);
    return QString(doc.toJson().data());
}

User::ConnectionState User::getThisClientConnectionState(){
    if(thisClientSocket_){
        switch (thisClientSocket_->state()) {
        case QSslSocket::UnconnectedState:
            return ConnectionState::UnconnectedState;
        case QSslSocket::HostLookupState:
            return ConnectionState::HostLookupState;
        case  QSslSocket::ConnectingState:
            return ConnectionState::ConnectingState;
        case QSslSocket::ConnectedState:
            return ConnectionState::ConnectedState;
        case  QSslSocket::BoundState:
            return ConnectionState::BoundState;
        case QSslSocket::ListeningState:
            return ConnectionState::ListeningState;
        case QSslSocket::ClosingState:
            return ConnectionState::ClosingState;
        default:
            return ConnectionState::UnconnectedState;
        }
    }else{
        return ConnectionState::UnconnectedState;
    }
}

QString User::getConnectionStateHuman(){
    if(thisClientSocket_){
        switch (thisClientSocket_->state()) {
        case QSslSocket::UnconnectedState:
            return "ConnectionState::UnconnectedState";
        case QSslSocket::HostLookupState:
            return "ConnectionState::HostLookupState";
        case  QSslSocket::ConnectingState:
            return "ConnectionState::ConnectingState";
        case QSslSocket::ConnectedState:
            return "ConnectionState::ConnectedState";
        case  QSslSocket::BoundState:
            return "ConnectionState::BoundState";
        case QSslSocket::ListeningState:
            return "ConnectionState::ListeningState";
        case QSslSocket::ClosingState:
            return "ConnectionState::ClosingState";
        default:
            return "ConnectionState::UnconnectedState";
        }
    }else{
        return "ConnectionState::UnconnectedState";
    }
}
void User::onDisconnected(){

#if PRINT_MESAE_DEBUG_MESSAGES
    print("User::onDisconnected :: Disabling MESAE ...");
#endif

    exitMESAEExecution();

    auto time = QDateTime::currentDateTime();

#if PRINT_User_Class_DEBUG_MESSAGES
    if(id_ != 0){
        if(thisClientSocket_){
            print(QString("[%1] Registered User on %2:%3 with mgbSessionId : %4 ,username :%5 , id : %6 disconnected")
                  .arg(time.toString("hh:mm:ss.zzz ap"))
                  .arg(thisClientSocket_->peerAddress().toString())
                  .arg(thisClientSocket_->peerPort())
                  .arg(thisClientMgbSessionId_)
                  .arg(username_).arg(id_));
        }else{
            print("Registered User ;-" + username_ + "id: " + id_ + " disconnected");
        }
    }else{
        if(thisClientSocket_){
            print(QString("[%1] Unregistered User on %2:%3 with mgbSessionId : %4 disconnected")
                  .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                  .arg(thisClientSocket_->peerAddress().toString())
                  .arg(thisClientSocket_->peerPort())
                  .arg(thisClientMgbSessionId_));
        }else{
            print("Unregistered User ;- mgbSessionId! " + thisClientMgbSessionId_ + " disconnected");
        }
    }
#endif
}

QList<const QSslSocket *> User::getAllConnectedClientsSocketsForThisUser() const
{
    return ConnectedUserList::instance()->getSockets(id_);
}

bool User::hasAtLeastOneConnectedClient()
{
    return ConnectedUserList::instance()->hasConnectedClients(id_);
}

bool User::hasMoreThanOneConnectedClients()
{
    return (getAllClientsMgbSessionIdsForThisUser().count() > 1);
}

QList<QString> User::getAllClientsMgbSessionIdsForThisUser() const
{
    return ConnectedUserList::instance()->getMgbSessionIds(id_);
}


void User::setThisClientSocket(const QSslSocket *socket)
{
    if(socket){
        thisClientSocket_ = const_cast<QSslSocket *>(socket);
        connect(thisClientSocket_,&QSslSocket::disconnected,this,&User::onDisconnected);
    }
}

void User::print(QString text){
    qDebug() << text;
}

void User::exitMESAEExecution(){
    if(thisClientController_){
        thisClientController_->MESAE_InputExtras_ = nullptr;
        thisClientController_->MESAE_Enabled_ = false;
        //remove from list MESAE controllers
        emit thisClientController_->MESAESignalDispatcher_
                .MESAEEnabledChanged(thisClientController_,false);
        //then
        thisClientController_->MESAE_hasActiveTimer_ = false;
        if(!thisClientController_->MESAE_semaphore_.available()){ //still no imput,gurd so we dont release more than 1 semaphore
            thisClientController_->MESAE_timeout_ = true;
            thisClientController_->MESAE_semaphore_.release(1);//release so we can exit thread
        }
    }
}

