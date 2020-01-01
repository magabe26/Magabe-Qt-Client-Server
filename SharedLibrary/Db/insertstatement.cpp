/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "insertstatement.h"

/**
 * @brief InsertStatement::InsertStatement
 * @param tableName
 */
InsertStatement::InsertStatement(const QString &tableName)
{
    startStr_.append(QString("INSERT INTO \'%1\' (").arg(tableName));
}
/**
 * @brief InsertStatement::insert
 * @param columnName
 * @param value
 * @return
 */
InsertStatement &InsertStatement::insert(const QString &columnName, QString value)
{
    columnAndValue_.append(QPair<QString,QString>(columnName,value));
    return *this;
}
/**
 * @brief InsertStatement::insert
 * @param columnName
 * @param value
 * @return
 */
InsertStatement &InsertStatement::insert(const QString &columnName, int value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));
}
/**
 * @brief InsertStatement::insert
 * @param columnName
 * @param value
 * @return
 */
InsertStatement &InsertStatement::insert(const QString& columnName, float value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));

}

InsertStatement &InsertStatement::insert(const QString &columnName, bool value){
    return insert(columnName,(value ? 1 : 0));
}
/**
 * @brief InsertStatement::insert
 * @param columnName
 * @param value
 * @return
 */
InsertStatement &InsertStatement::insert(const QString &columnName, double value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));

}
/**
 * @brief InsertStatement::insert
 * @param columnName
 * @param value
 * @return
 */
InsertStatement &InsertStatement::insert(const QString& columnName,long long value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));

}

InsertStatement &InsertStatement::insert(const QString& columnName,unsigned long long value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));

}

InsertStatement &InsertStatement::insert(const QString& columnName,unsigned long value)
{
    columnWithNumberValue_.append(columnName);
    return insert(columnName,QString::number(value));

}
InsertStatement &InsertStatement::insert(const QString& columnName,QChar value)
{
    return insert(columnName,QString(value));
}
/**
 * @brief InsertStatement::toString
 * @return
 */
QString InsertStatement::toString()
{
    QString keysStr;
    QString valuesStr;
    foreach(auto pair, columnAndValue_) {
        if(!keysStr.isEmpty()){ keysStr.append(",");}
        keysStr.append(QString("\'%1\'").arg(pair.first));

        if(!valuesStr.isEmpty()){ valuesStr.append(",");}
        if(isNumberValue(pair.first)){
            valuesStr.append(QString("%1").arg(pair.second));
        }else{
            valuesStr.append(QString("\'%1\'").arg(pair.second));
        }
    }
    QString readyStr = startStr_;
    readyStr.append(keysStr).append(")")
            .append(" VALUES (").append(valuesStr).append(")");
    return readyStr;
}
/**
 * @brief InsertStatement::isNumberValue
 * @param columnName
 * @return
 */
bool InsertStatement::isNumberValue(const QString& columnName){
    return columnWithNumberValue_.contains(columnName);
}

