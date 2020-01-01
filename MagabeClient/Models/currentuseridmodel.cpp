/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "currentuseridmodel.h"
#include <Db/createtablestatement.h>
#include <Db/insertstatement.h>
#include "security.h"

CurrentUserIdModel::CurrentUserIdModel(QObject *parent) : Model(parent)
{
    setTablename("currentUserId");
}

void CurrentUserIdModel::createTable()
{
    CreateTableStatement createTableStatement(tablename());
    createTableStatement.addColumn(idColumn,"varchar(255)"/*because of encryption*/,true,true,true);
    m_db->excuteSqlQuery(createTableStatement.toString(),"currentUserId table created Sucessfull","Failed to create currentUserId table");

}

bool CurrentUserIdModel::saveCurrentId(unsigned long id)
{
    if(idExist()){
        dropTable();
        createTable();
    }
    InsertStatement insertStatement(tablename());
    insertStatement.insert(idColumn,Security::instance()->encryptToString(QString::number(id)));
    return  m_db->excuteSqlQuery(insertStatement.toString(),"currentUserId saved Sucessfull","Failed to save currentUserId ");
}

bool CurrentUserIdModel::idExist()
{
    return (getRecordCount() > 0);
}

unsigned long CurrentUserIdModel::getCurrentId()
{
    if(idExist()){

        QString query = QString("SELECT %1 FROM %2 LIMIT 1").arg(idColumn).arg(tablename());
        auto sqlQuery = m_db->getSqlQuery();
        sqlQuery->exec(query);
        if(sqlQuery->isActive() && sqlQuery->first()) {
            return Security::instance()->decryptToString(sqlQuery->value(0).toString()).toULong();
        }else{
            return 0;
        }

    }else{
        return 0;
    }
}

