/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "createtablestatement.h"
/**
 * @brief CreateTableStatement::CreateTableStatement
 * @param tableName
 */
CreateTableStatement::CreateTableStatement(const QString &tableName):tableName_(tableName)
{
  startStr_.append(QString("CREATE TABLE IF NOT EXISTS \'%1\' ( ").arg(tableName_));
}

/**
 * @brief CreateTableStatement::CreateTableStatement
 * @param tableName
 */
CreateTableStatement::CreateTableStatement(const char *tableName):tableName_(tableName)
{
  startStr_.append(QString("CREATE TABLE IF NOT EXISTS \'%1\' ( ").arg(tableName_));
}

/**
 * @brief CreateTableStatement::tableName
 * @return
 */
QString CreateTableStatement::tableName() const
{
  return tableName_;
}

/**
 * @brief CreateTableStatement::setTableName
 * @param tableName
 */
void CreateTableStatement::setTableName(const QString &tableName)
{
  tableName_ = tableName;
}

/**
 * @brief CreateTableStatement::addColumn
 * @param columnName
 * @param dataType
 * @param primaryKey
 * @param notNull
 * @param unique
 * @return
 */
CreateTableStatement &CreateTableStatement::addColumn(const QString &columnName, QString dataType, bool primaryKey, bool notNull, bool unique)
{
  QString clmn;
  clmn.append(QString("\'%1\' %2").arg(columnName).arg(dataType));
  if(primaryKey){clmn.append(" PRIMARY KEY");}
  if(notNull){
      clmn.append(" NOT NULL");
    }else{
      clmn.append(" NULL");
    }
  if(unique) {clmn.append(" UNIQUE");}

  if(createStatementStr_.contains(startStr_,Qt::CaseSensitive)){
      createStatementStr_.remove(terminatingChar_);
      createStatementStr_.append(",").append(clmn);

    }else{
      createStatementStr_.append(startStr_).append(clmn);
    }
  createStatementStr_.append(" "+terminatingChar_);
  return *this;
}

/**
 * @brief CreateTableStatement::addColumn
 * @param columnName
 * @param dataType
 * @param primaryKey
 * @param notNull
 * @param unique
 * @return
 */
CreateTableStatement &CreateTableStatement::addColumn(const char *columnName, const char *dataType, bool primaryKey, bool notNull, bool unique)
{
  return addColumn(QString(columnName), QString(dataType),primaryKey,notNull,unique);
}

/**
 * @brief CreateTableStatement::toString
 * @return
 */
QString CreateTableStatement::toString() const
{
  QString readyStr= createStatementStr_;
  readyStr.replace(terminatingChar_,")");
  return readyStr;
}
