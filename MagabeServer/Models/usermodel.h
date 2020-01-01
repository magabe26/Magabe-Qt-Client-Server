/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef USERMODEL_H
#define USERMODEL_H
#define SET_AUTHKEY_LENGTH(len) \
    static int getAuthKeyLength() { return len;} \
    const QString authKeyType = QString("varchar(")+#len+")";

#include <QObject>
#include "Models/model.h"
#include "Helpers/user.h"

class UserObjectEditor;
class MessagesModel;

class UserModel : public Model
{
    Q_OBJECT
public:

    explicit UserModel(QObject *parent = nullptr);
    ~UserModel();

    User getUser(unsigned long userId);
    bool updateIsActive(bool isActive, unsigned long userId);//must e here
    bool isEmailTaken(const QString& email);
signals:


private:

    MODEL_COLUMN_NAME(id)
    MODEL_COLUMN_NAME(firstName)
    MODEL_COLUMN_NAME(lastName)
    MODEL_COLUMN_NAME(dateOfBirth)
    MODEL_COLUMN_NAME(gender)
    MODEL_COLUMN_NAME(username)
    MODEL_COLUMN_NAME(authKey)
    MODEL_COLUMN_NAME(passwordHash)
    MODEL_COLUMN_NAME(email)
    MODEL_COLUMN_NAME(isActive) //is currently using the app
    MODEL_COLUMN_NAME(isBlackListed)
    MODEL_COLUMN_NAME(createdAt)
    MODEL_COLUMN_NAME(updatedAt)
    MODEL_COLUMN_NAME(profilePicturePath)
    MODEL_COLUMN_NAME(education)
    MODEL_COLUMN_NAME(position)
    MODEL_COLUMN_NAME(location)
    MODEL_COLUMN_NAME(phoneNumber)
    SET_AUTHKEY_LENGTH(38)

    //only these class can access these methods
    friend class UserObjectEditor;
    friend struct AppUsers;
    friend class MessagesModel;

    bool insertNewUser(const QString &firstName, const QString &lastName, const QString &dateOfBirth,
                       User::Gender gender, const QString &username, const QString &password, const QString &email,
                       const QString &profilePicturePath, const QString &education, const QString & position,
                       const QString &location, const QString &phoneNumber);

    User getUser(const QString &username);
    bool userExists(QString username);
    bool userExists(unsigned long userId);
    bool isValidEmail(const QString & email);
    int getTotalNumberOfUsers();
    unsigned long getUserId(const QString &username);
    QString getUserName(unsigned long userId);
    static QString generateAuthKey(unsigned long userId);
    bool updateAuthKey(const QString &authKey, unsigned long  userId);
    bool updatePhoneNumber(const QString &phoneNumber, unsigned long userId);
    bool updateLocation(const QString &location, unsigned long userId);
    bool updatePosition(const QString &position, unsigned long userId);
    bool updateEducation(const QString &education, unsigned long userId);
    bool updateProfilePicturePath(const QString &profilePicturePath, unsigned long userId);
    bool updateIsBlackListed(bool isBlackListed, unsigned long userId);
    bool updateEmail(const QString &email, unsigned long userId);
    bool updatePassword(const QString &password, unsigned long userId);
    bool updateUsername(const QString &username, unsigned long userId);
    bool updateGender(const User::Gender &gender, unsigned long userId);
    bool updateDateOfBirth(const QString &dateOfBirth, unsigned long userId);
    bool updateFirstName(const QString &firstName, unsigned long userId);
    bool updateLastName(const QString &lastName, unsigned long userId);
    bool deleteUser(unsigned long userId);
    bool deleteUser(const QString &username);

    bool insertNewUser(unsigned long id, const QString &firstName, const QString &lastName, const QString &dateOfBirth,
                       const User::Gender gender, const QString &username, const QString &authKey, const QString &passwordHash, const QString &email,
                       bool isActive, bool isBlackListed, qint64 createdAt, qint64 updatedAt, const QString &profilePicturePath,
                       const QString &education, const QString &position, const QString &location, const QString &phoneNumber);

    unsigned long getUserIdOfTheLastSignedUser();
    int createNewId();
    static QString toPasswordHash(const QString &password);
    static QString toPassword(const QString &passwordHash);
    void recordUpdated(unsigned long userId);
    void recordUpdated(const QString &username);
    void updateStatistics();

    void createTable();

    // Model interface
protected:
    bool deleteRecord(const QString &where,const QString &like);
    bool updateRecord(const QString &columnName,const QString &value,const QString &where,const QString &like);
};

#undef SET_AUTHKEY_LENGTH
#endif // USERMODEL_H
