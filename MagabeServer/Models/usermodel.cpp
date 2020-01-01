/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */



#include "usermodel.h"
#include "debug.h"
#include <Db/createtablestatement.h>
#include <Db/insertstatement.h>
#include "Models/statistticsmodel.h"
#include <QJsonObject>
#include "security.h"
#include <QSqlRecord>
#include <QDebug>

UserModel::UserModel(QObject *parent) : Model(parent)
{

    setTablename("user");
    setUserIdColumnName(idColumn);

    /* register signals,inorder be able to connect to those signals
        using Controller::connectSlot OR ConnectionPipe::connectSlot
        and to be able to emit those signals from any where in a program*/
    registerModelSignals();
}

UserModel::~UserModel(){
    /*unregister signals registered by registerModelSignals*/
    unRegisterModel();
}

bool UserModel::insertNewUser(const QString &firstName, const QString &lastName, const QString &dateOfBirth,
                              User::Gender gender, const QString &username, const QString &password,
                              const QString &email, const QString &profilePicturePath,
                              const QString &education, const QString &position, const QString &location,
                              const QString &phoneNumber){

    qint64 timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    unsigned long id = createNewId();
    return insertNewUser(id,firstName,lastName,
                         dateOfBirth,gender,username,generateAuthKey(id),toPasswordHash(password)
                         ,email,false,false,timeStamp,timeStamp,profilePicturePath
                         ,education,position,location,phoneNumber);
}

bool UserModel::deleteUser(unsigned long  userId){
    return deleteRecord(idColumn,QString::number(userId));
}

bool UserModel::deleteUser(const QString &username){
    return deleteUser(getUserId(username));
}

User UserModel::getUser(unsigned long  userId)
{

    auto sqlQuery =  m_db->getSqlQuery();
    sqlQuery->exec(QString("SELECT * FROM %1 WHERE %2 LIKE \'%3\'")
                   .arg(tablename()).arg(idColumn).arg(userId));

    int idFieldNo = sqlQuery->record().indexOf(idColumn);
    int firstNameFieldNo = sqlQuery->record().indexOf(firstNameColumn);
    int lastNameFieldNo = sqlQuery->record().indexOf(lastNameColumn);
    int dateOfBirthFieldNo = sqlQuery->record().indexOf(dateOfBirthColumn);
    int genderFieldNo = sqlQuery->record().indexOf(genderColumn);
    int usernameFieldNo = sqlQuery->record().indexOf(usernameColumn);
    int authKeyFieldNo = sqlQuery->record().indexOf(authKeyColumn);
    int passwordHashFieldNo = sqlQuery->record().indexOf(passwordHashColumn);
    int emailFieldNo = sqlQuery->record().indexOf(emailColumn);
    int isActiveFieldNo = sqlQuery->record().indexOf(isActiveColumn);
    int isBlackListedFieldNo = sqlQuery->record().indexOf(isBlackListedColumn);
    int createdAtFieldNo = sqlQuery->record().indexOf(createdAtColumn);
    int updatedAtFieldNo = sqlQuery->record().indexOf(updatedAtColumn);
    int profilePicturePathFieldNo = sqlQuery->record().indexOf(profilePicturePathColumn);
    int educationFieldNo = sqlQuery->record().indexOf(educationColumn);
    int positionFieldNo = sqlQuery->record().indexOf(positionColumn);
    int locationFieldNo =  sqlQuery->record().indexOf(locationColumn);
    int phoneNumberFieldNo = sqlQuery->record().indexOf(phoneNumberColumn);

    User user; user.id_ = 0;
    if(sqlQuery->isActive() && sqlQuery->first()) {
        user.id_ = sqlQuery->value(idFieldNo).toString().trimmed().toULong();
        user.firstName_ = sqlQuery->value(firstNameFieldNo).toString().trimmed();
        user.lastName_ = sqlQuery->value(lastNameFieldNo).toString().trimmed();
        user.dateOfBirth_ = sqlQuery->value(dateOfBirthFieldNo).toString().trimmed();
        user.gender_  =  (sqlQuery->value(genderFieldNo).toString().trimmed().toUpper().at(0) == QChar('M'))? User::Gender::MALE : User::Gender::FEMALE;
        user.username_ = sqlQuery->value(usernameFieldNo).toString().trimmed();
        user.authKey_  = sqlQuery->value(authKeyFieldNo).toString().trimmed();
        user.password_ = toPassword(sqlQuery->value(passwordHashFieldNo).toString());
        user.email_ = sqlQuery->value(emailFieldNo).toString().trimmed();
        user.isActive_ = (sqlQuery->value(isActiveFieldNo).toString().trimmed().toInt() == 1);
        user.isBlackListed_ = (sqlQuery->value(isBlackListedFieldNo).toString().trimmed().toInt() == 1);
        user.createdAt_  = (qint64) sqlQuery->value(createdAtFieldNo).toULongLong();
        user.updatedAt_ = (qint64) sqlQuery->value(updatedAtFieldNo).toULongLong();
        user.profilePicturePath_ = sqlQuery->value(profilePicturePathFieldNo).toString();
        user.education_ = sqlQuery->value(educationFieldNo).toString();
        user.position_ = sqlQuery->value(positionFieldNo).toString();
        user.location_ = sqlQuery->value(locationFieldNo).toString();
        user.phoneNumber_ = sqlQuery->value(phoneNumberFieldNo).toString();
    }

    return user;
}

User UserModel::getUser(const QString &username)
{
    User user; user.id_ = 0;
    unsigned long id= getUserId(username.trimmed());
    if(id != 0){
        user = getUser(id);
    }
    return user;
}

unsigned long UserModel::getUserId(const QString &username){
    if(!userExists(username)) return 0;
    unsigned long id = 0;
    QString dbRecord = getRecord(idColumn,usernameColumn,username,"User UserModel::getUser(QString username)");
    if(!dbRecord.isEmpty()){
        bool ok = false;
        id = dbRecord.toULong(&ok);
        if(!ok){//we want a valid id here
            id = 0;
        }
    }
    return id;
}

QString UserModel::getUserName(unsigned long userId)
{
    return getRecord(usernameColumn,idColumn,QString::number(userId),"UserModel::getUserName");
}

bool UserModel::userExists(QString username){
    if(username.isEmpty()) return false;
    QString dbRecord = getRecord(usernameColumn,usernameColumn,username,"bool UserModel::isUserExists(QString username)");
    return (!dbRecord.isEmpty() && (dbRecord.trimmed() == username.trimmed()));

}

bool UserModel::userExists(unsigned long userId){
    if(userId <= 0)  return false;
    unsigned long dbRecord= getRecord(idColumn,idColumn,QString::number(userId),"bool isUserExists(unsigned long userId)").toULong();
    return(dbRecord == userId);
}

bool UserModel::updatePhoneNumber(const QString &phoneNumber , unsigned long userId)
{
    return updateRecord(phoneNumberColumn,phoneNumber,idColumn,QString::number(userId));
}

bool UserModel::updateLocation(const QString &location , unsigned long userId)
{
    return updateRecord(locationColumn,location,idColumn,QString::number(userId));
}

bool UserModel::updatePosition(const QString &position , unsigned long userId)
{
    return updateRecord(positionColumn,position,idColumn,QString::number(userId));
}
bool UserModel::updateEducation(const QString &education , unsigned long userId)
{
    return updateRecord(educationColumn,education,idColumn,QString::number(userId));
}
bool UserModel::updateProfilePicturePath(const QString &profilePicturePath , unsigned long userId)
{
    return updateRecord(profilePicturePathColumn,profilePicturePath,idColumn,QString::number(userId));
}

bool UserModel::updateIsBlackListed(bool isBlackListed , unsigned long userId)
{
    return updateRecord(isBlackListedColumn,QString::number((isBlackListed)? 1 : 0),idColumn,QString::number(userId));
}

bool UserModel::updateIsActive(bool isActive , unsigned long userId)
{
    return updateRecord(isActiveColumn,QString::number((isActive)? 1 : 0),idColumn,QString::number(userId));
}

bool UserModel::isEmailTaken(const QString &email)
{
    QString dbemail = getRecord(emailColumn,emailColumn,email,"bool UserModel::isEmailTaken(const QString &email)");
    return (email == dbemail);
}


bool UserModel::updateEmail(const QString &email , unsigned long userId)
{
    if(isValidEmail(email)){
        return updateRecord(emailColumn,email,idColumn,QString::number(userId));
    }else{
        return false;
    }
}

bool UserModel::updatePassword(const QString &password , unsigned long userId)
{
    return updateRecord(passwordHashColumn,toPasswordHash(password),idColumn,QString::number(userId));
}

bool UserModel::updateUsername(const QString &username , unsigned long userId)
{
    return updateRecord(usernameColumn,username,idColumn,QString::number(userId));
}

bool UserModel::updateGender(const User::Gender &gender , unsigned long userId)
{
    const QString genderStr = (gender == User::Gender::MALE) ? "M" : "F";//to avoid implicity conversions
    return updateRecord(genderColumn,genderStr,idColumn,QString::number(userId));
}

bool UserModel::updateDateOfBirth(const QString &dateOfBirth , unsigned long userId)
{
    return updateRecord(dateOfBirthColumn,dateOfBirth,idColumn,QString::number(userId));
}

bool UserModel::updateFirstName (const QString &firstName, unsigned long userId)
{
    return updateRecord(firstNameColumn,firstName,idColumn,QString::number(userId));
}

bool UserModel::updateLastName (const QString &lastName, unsigned long userId)
{
    return updateRecord(lastNameColumn,lastName,idColumn,QString::number(userId));
}

bool UserModel::updateAuthKey(const QString &authKey, unsigned long userId)
{
    return updateRecord(authKeyColumn,authKey,idColumn,QString::number(userId));
}


bool UserModel::isValidEmail(const QString &email)
{
    //target all
    // QString emailRegExp = QStringLiteral("^[A-Z a-z 0-9]{1,50}@[A-Z a-z]{1,50}[.]{1,1}[A-Z a-z 0-9]{0,15}[.]{0,1}[A-Z a-z]{2,15}$");
    //that mostlt target yahoo,gmail
    QString emailRegExp = QStringLiteral("^[A-Z a-z 0-9]{1,50}@[A-Z a-z]{5,50}[.]{1,1}[A-Z a-z 0-9]{0,15}[.]{0,1}[A-Z a-z]{2,15}$");
    QRegExp  emailvalidator(emailRegExp);
    bool b =  emailvalidator.exactMatch(email);
    qDebug() << "bool UserModel::isValidEmail" << email << "--" <<b;
    return b;
}



void UserModel::createTable()
{
    CreateTableStatement createTableStatement(tablename());
    createTableStatement.addColumn(idColumn,"int(11)",true,true,true)
            .addColumn(firstNameColumn,"varchar(255)",false,true,false)
            .addColumn(lastNameColumn,"varchar(255)",false,true,false)
            .addColumn(dateOfBirthColumn,"varchar(255)",false,true,false)
            .addColumn(genderColumn,"varchar(2)",false,true,false)
            .addColumn(usernameColumn,"varchar(255)",false,true,true)
            .addColumn(authKeyColumn,authKeyType,false,true,false)
            .addColumn(passwordHashColumn,"varchar(255)",false,true,false)
            .addColumn(emailColumn,"varchar(255)",false,true,true)
            .addColumn(isActiveColumn,"int(1)",false,true,false)
            .addColumn(isBlackListedColumn,"int(1)",false,true,false)
            .addColumn(createdAtColumn,"int(15)",false,true,false)
            .addColumn(updatedAtColumn,"int(15)",false,true,false)
            .addColumn(profilePicturePathColumn,"varchar(255)",false,false,true)
            .addColumn(educationColumn,"varchar(255)",false,false,false)
            .addColumn(positionColumn,"varchar(255)",false,false,false)
            .addColumn(locationColumn,"varchar(255)",false,false,false)
            .addColumn(phoneNumberColumn,"varchar(30)",false,false,true);

    QString query = createTableStatement.toString();

    if(m_db->excuteSqlQuery(query,tablename()+" table created Sucessfull","Failed to create "+tablename()+" table")){
        StatisticsModel  statisticsModel;
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        print("initilizing "+ tablename() +" statistics ...");
#endif
        if(statisticsModel.initStatistic(tablename(),0)){//it will not overwrite if record is already created
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print(tablename() +" statistics initilized ...");
#endif
        }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        else{
            print(tablename() +" statistics  already initilized ...");
        }
#endif
    }

}

bool UserModel::insertNewUser(unsigned long id, const QString &firstName, const QString &lastName,
                              const QString &dateOfBirth,const User::Gender gender,
                              const QString &username, const QString &authKey,
                              const QString &passwordHash, const QString &email,
                              bool isActive, bool isBlackListed, qint64 createdAt,
                              qint64 updatedAt, const QString &profilePicturePath, const QString &education,
                              const QString &position, const QString &location, const QString &phoneNumber)
{
    const QString genderStr = (gender == User::Gender::MALE) ? "M" : "F";//to avoid implicity conversions
    InsertStatement insertStatement(tablename());
    insertStatement.insert(idColumn,id)
            .insert(firstNameColumn,firstName)
            .insert(lastNameColumn,lastName)
            .insert(dateOfBirthColumn,dateOfBirth)
            .insert(genderColumn,genderStr)
            .insert(usernameColumn,username)
            .insert(authKeyColumn,authKey)
            .insert(passwordHashColumn,passwordHash)
            .insert(emailColumn,email)
            .insert(isActiveColumn,isActive)
            .insert(isBlackListedColumn,isBlackListed)
            .insert(createdAtColumn,createdAt)
            .insert(updatedAtColumn,updatedAt);

    if(!profilePicturePath.isEmpty()){//because it is unique and can be null, we avoid empty string
        insertStatement.insert(profilePicturePathColumn,profilePicturePath);
    }

    insertStatement.insert(educationColumn,education)
            .insert(positionColumn,position)
            .insert(locationColumn,location);

    if(!phoneNumber.isEmpty()){
        insertStatement.insert(phoneNumberColumn,phoneNumber);
    }


    if(m_db->excuteSqlQuery(insertStatement.toString(),"new user created Sucessfull","Failed to create new user ;- "+username)){
        //update statistics
        updateStatistics();
        return true;

    }else{
        return false;
    }

}

QString UserModel::generateAuthKey(unsigned long userId)
{
    QString idstr = QString::number(userId);
    int len = (getAuthKeyLength() - (idstr.length()+1));
    QString auth= generateRandomString(len);
    return auth.prepend(idstr+":");
}

int UserModel::getTotalNumberOfUsers()
{
    return getRecordCount();
}

unsigned long UserModel::getUserIdOfTheLastSignedUser()
{
    auto sqlQuery =  m_db->getSqlQuery();
    sqlQuery->exec(QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 1")
                   .arg(idColumn).arg(tablename()).arg(idColumn));
    if(sqlQuery->isActive() && sqlQuery->first()){
        return sqlQuery->value(0).toString().toULong();
    }else{
        return 0;
    }
}

int UserModel::createNewId()
{
    int id = getUserIdOfTheLastSignedUser();
    return (id+1);
}

QString UserModel::toPasswordHash(const QString &password){
    return Security::instance()->encryptToString(password);
}

QString UserModel::toPassword(const QString &passwordHash){
    return Security::instance()->decryptToString(passwordHash);
}

void UserModel::recordUpdated(unsigned long userId){
    qint64 timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    //in this only call the base one to avoid recurcion
    Model::updateRecord(updatedAtColumn,QString::number(timeStamp),idColumn,QString::number(userId));
}

void UserModel::recordUpdated(const QString &username){
    qint64 timeStamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    //in this only call the base one to avoid recurcion
    Model::updateRecord(updatedAtColumn,QString::number(timeStamp),usernameColumn,username);
}

bool UserModel::deleteRecord(const QString &where, const QString &like){
    bool deleted = Model::deleteRecord(where,like);
    if(deleted)
        updateStatistics();
    return deleted;
}

bool UserModel::updateRecord(const QString &columnName, const QString &value, const QString &where, const QString &like)
{
    if(Model::updateRecord(columnName,value,where,like)){
        bool isUserId = false;
        unsigned long userId = like.toULong(&isUserId);
        if(!isUserId){
            userId = getUserId(like);
        }
        if(userId != 0){
            recordUpdated(userId);
        }
        return true;
    }else{
        return false;
    }
}

void UserModel::updateStatistics(){
    StatisticsModel statistics;
    int total = getRecordCount();
    statistics.updateStatistic(tablename(),total);
}
