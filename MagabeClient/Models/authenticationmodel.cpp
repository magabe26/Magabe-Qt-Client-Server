/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "authenticationmodel.h"
#include <Db/createtablestatement.h>
#include <Db/insertstatement.h>

AuthenticationModel::AuthenticationModel(QObject *parent) : Model(parent)
{
    setTablename("authentication");
    setUserIdColumnName(idColumn);
}

unsigned long AuthenticationModel::getId(const QString& username)
{
    return getRecord(idColumn,usernameColumn,username,"unsigned long AuthenticationModel::getId(const QString& username)").toULong();
}

QString AuthenticationModel::getUserName(unsigned long id)
{
    return Security::instance()->decryptToString(getRecordUsingId(usernameColumn,id,"QString AuthenticationModel::getUserName(unsigned long id)")).trimmed();
}

QString AuthenticationModel::getAuthKey(unsigned long id)
{
    return Security::instance()->decryptToString(getRecordUsingId(authKeyColumn,id,"QString AuthenticationModel::getAuthKey(unsigned long id)"));
}

QString AuthenticationModel::getPassword(unsigned long id)
{
    return Security::instance()->decryptToString(getRecordUsingId(passwordColumn,id,"QString AuthenticationModel::getPassword(unsigned long id)"));
}

QString AuthenticationModel::getAuthKey(const QString &username)
{
    return getAuthKey(getId(username));
}

QString AuthenticationModel::getPassword(const QString &username)
{
    return getPassword(getId(username));
}

void AuthenticationModel::createTable()
{
    CreateTableStatement createTableStatement (tablename());
    createTableStatement.addColumn(idColumn,"int(11)",true,true,true)
            .addColumn(usernameColumn,"varchar(255)",false,true,true)
            .addColumn(authKeyColumn,"varchar(255)"/*for good measure*/,false,true,false)
            .addColumn(passwordColumn,"varchar(255)",false,true,false);

    m_db->excuteSqlQuery(createTableStatement.toString(),"authentication table created Sucessfull","Failed to create user table");

}

bool AuthenticationModel::updateCredetials(unsigned long id, const QString &username
                                           , const QString &authKey,const QString& password)
{
    dropTable();
    createTable();

    QString e_username = Security::instance()->encryptToString(username);
    QString e_authKey = Security::instance()->encryptToString(authKey);
    QString e_password = Security::instance()->encryptToString(password);

    InsertStatement insertStatement(tablename());
    insertStatement.insert(idColumn,id)
            .insert(usernameColumn,e_username)
            .insert(authKeyColumn,e_authKey)
            .insert(passwordColumn,e_password);


  return  m_db->excuteSqlQuery(insertStatement.toString(),"authentication record saved Sucessfull","Failed to saved authentication record ");
}


bool AuthenticationModel::updateAuthKey(const QString& newAuthKey,unsigned long userId)
{
    QString e_newAuthKey = Security::instance()->encryptToString(newAuthKey);
    return updateRecord(authKeyColumn,e_newAuthKey,idColumn,QString::number(userId));
}
