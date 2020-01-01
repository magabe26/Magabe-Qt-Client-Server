#include "model.h"
#include "debug.h"
#include <QDebug>
#include "Helpers/constants.h"
#include "Helpers/connectionpipe.h"

Model::Model(QObject *parent) : QObject(parent),
    m_db(new MagabeDB(this))
{

}

/**
 * @brief Model::tablename
 * @return
 */
QString Model::tablename() const
{
    return tablename_;
}

/**
 * @brief Model::setTablename
 * @param tablename
 */
void Model::setTablename(const QString &tablename)
{
    tablename_ = tablename;
}

/**
 * @brief Model::print
 * @param status
 */
void Model::print(const QString &status)
{
    qDebug()<< status;
}

//set userIdColumnName first to use this method in your model
/**
 * @brief Model::getRecordUsingId
 * @param columnName
 * @param userId
 * @param errorMessage
 * @return
 */
QString Model::getRecordUsingId(const QString &columnName, int userId, const QString &errorMessage)
{
    assert(!userIdColumnName().isEmpty());

    QString results;
    QString query = QString("SELECT  %1 FROM %2 WHERE %3 LIKE \'%4\' LIMIT 1").arg(columnName).arg(tablename()).arg(userIdColumnName()).arg(QString::number(userId));

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive()) {

        if(sqlQuery->first()){

            results = sqlQuery->value(0).toString();

#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print(QString(QString("  :FOUND  %1 ----> %2 WHERE %3 LIKE %4").arg(columnName).arg(results).arg("id").arg(userId)));
#else
            Q_UNUSED(errorMessage)
#endif
        }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        else{
            print(errorMessage);
        }
#endif
    }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    else{
        print(errorMessage);
    }
#endif

    return results;
}

/**
 * @brief Model::getRecord
 * @param columnName
 * @param where
 * @param like
 * @param functionName
 * @return
 */
QString Model::getRecord(const QString &columnName,const QString &where,const QString &like,const QString &functionName)
{


    QString tableName = tablename();
    QString results;
    QString query = QString("SELECT  %1 FROM %2 WHERE %3 LIKE \'%4\' LIMIT 1").arg(columnName).arg(tableName).arg(where).arg(like) ;

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive()) {

        if(sqlQuery->first()){

            results = sqlQuery->value(0).toString();

#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print(QString("%1 : FOUND  %2 ----> %3 WHERE %4 LIKE %5").arg(functionName).arg(columnName).arg(results).arg(where).arg(like));
#else
            Q_UNUSED(functionName)
#endif
        }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        else{
            print(QString("%1 : RECORD NOT FOUND, WHERE %4 LIKE %5").arg(functionName).arg(where).arg(like));
        }
#endif

    }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    else{
        print(QString("%1 : %2  ; FAILED :ERROR ;- %3").arg(functionName).arg(query).arg(sqlQuery->lastError().text()));
    }
#endif

    return results.trimmed();
}

/**
 * @brief Model::getTotalnumberOfRecordInaTable
 * @param tableName
 * @return
 */
int Model::getRecordCount()
{
    QString query = QString("SELECT COUNT(*) FROM %1 WHERE 1").arg(tablename());
    return getRecordCountImpl(query);
}

int  Model::getRecordCount(QStringList whereColumns, QStringList likeValues,const Range* rangeValues) {
    QString query = QString("SELECT COUNT(*) FROM %1").arg(tablename());

    int indexWhere = 0;
    foreach (QString column , whereColumns) {
        if (indexWhere == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));

        }
        indexWhere++;
    }


    if(indexWhere == 0){
        query.append(" WHERE 1");
    }

    //range
    if (rangeValues) {
        query.append(QString(" AND %1 BETWEEN \"%2\" AND \"%3\"").arg(rangeValues->column()).arg(rangeValues->first()).arg(rangeValues->last()));
    }

    query.append(";");

    return getRecordCountImpl(query);
}

int Model::getNextRowId(const int currentRowId,QStringList whereColumns, QStringList likeValues)
{
    QString query = QString("SELECT %1 FROM %2")
            .arg(rowidColumn).arg(tablename());

    int indexWhere = 0;
    foreach (QString column , whereColumns) {
        if (indexWhere == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));

        }
        indexWhere++;
    }

    query.append(QString(" AND \"%1\" > \"%2\" LIMIT 1").arg(rowidColumn).arg(currentRowId));
    query.append(";");

    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);

    if(sqlQuery->isActive() && sqlQuery->first()) {
        auto results = sqlQuery->value(0).toString();
        if(results.isEmpty()){
            return -1;
        }else{
            bool ok = false;
            int rowId = results.toInt(&ok);
            if(ok){
                return rowId;
            }else{
                return -1;
            }
        }
    }else{
        return -1;
    }


}

int Model::getRecordCountImpl(const QString &query)
{
    if(query.isEmpty()){
        return 0;
    }
    int recordTotal = 0;
    auto sqlQuery = m_db->getSqlQuery();
    sqlQuery->exec(query);
    if(sqlQuery->isActive()) {
        if(sqlQuery->first())
            recordTotal = sqlQuery->value(0).toInt();
    }

#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    else{
        print(QString("%1 ; FAILED : ERROR ;- %2").arg(query).arg(sqlQuery->lastError().text()));
    }
#endif

    return recordTotal;
}

/**
 * @brief Model::deleteRecord
 * @param where
 * @param like
 */
bool Model::deleteRecord(const QString &where, const QString &like)
{
    bool deleted = false;
    const int prevCount = getRecordCount();
    QString query = QString("DELETE FROM \"%1\" WHERE \"%2\" LIKE \"%3\"").arg(tablename()).arg(where).arg(like);
    if(m_db->excuteSqlQuery(query,"","")){
        //means query excuted not necessary deleted
        const int nowCount = getRecordCount();
        deleted = (nowCount < prevCount); //means record deleted, not necessary true in multithreaded app, but fair
    }else{
        deleted =  false;
    }

#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    (deleted)
            ? print(QString("%1 deleted from  %2").arg(like).arg(tablename()))
            : print(QString("failed to delete %1 from  %2, may be it doesnt exists").arg(like).arg(tablename()));
#endif

    return deleted;
}


/**
 * @brief Model::generateNewAuthKey
 * @param numberOfChararacter
 * @return
 */
QString Model::generateRandomString(int numberOfChararacter)
{
    QString str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopgrst1234567890");

    QString generatedStr;
    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int x = 1; x <= numberOfChararacter;x++){
        int i = rand()%str.size();
        generatedStr += str.at(i) ;
    }

    return generatedStr;
}

/**
 * @brief Model::updateRecord
 * @param columnName
 * @param value
 * @param where
 * @param like
 * @return
 */
bool Model::updateRecord(const QString &columnName, const QString &value, const QString &where, const QString &like)
{
    QString query = QString("UPDATE \"%1\" SET \"%2\" = \"%3\" WHERE  \"%4\" = \"%5\"").arg(tablename()).arg(columnName).arg(value).arg(where).arg(like);
    return m_db->excuteSqlQuery(query,QString("%1 updated to  %2").arg(columnName).arg(value),QString("failed to update %1 to  %2").arg(columnName).arg(value));

}

/**
 * @brief Model::generateRandomStringWithNoNumbers
 * @param numberOfChararacter
 * @return
 */
QString Model::generateRandomStringWithNoNumbers(int numberOfChararacter)
{
    QString str("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopgrst");

    QString generatedStr;
    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int x = 1; x <= numberOfChararacter;x++){

        int i = rand()%str.size();
        generatedStr += str.at(i) ;
    }

    return generatedStr;
}

/**
 * @brief Model::getTimeSTamp
 * @return
 */
QString Model::getTimestamp()
{
    return QString::number( QDateTime::currentDateTime().toMSecsSinceEpoch());
}

/**
 * @brief Model::deleteFileFromDisc
 * @param filePath
 * @return
 */
bool Model::deleteFileFromDisc(const QString &filePath)
{
    QDir filepathDir;
    return (filepathDir.remove(filePath));
}

bool Model::isSignalRegistered(const QString &className, const QString &signalName)
{
    return ConnectionPipe::isSignalRegistered(className,signalName);
}

bool Model::isSignalRegistered(const QString &signalName)
{
    return ConnectionPipe::isSignalRegistered(signalName);
}

bool Model::emitSignal(const QString &senderClassName, const QString &signalName, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9){
    return ConnectionPipe::emitSignal(senderClassName,signalName,val0, val1,val2,val3,val4,val5,val6,val7,val8,val9);
}

bool Model::connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName){
    return ConnectionPipe::connectSlot(senderClassName,signalName,receiver,slotName);
}

QStringList Model::getSignalClassNames(const QString &signalName){
    return  ConnectionPipe::getSignalClassNames(signalName);
}


void Model::registerModelSignals()
{
    ConnectionPipe::registerObject(this);

}

void Model::unRegisterModel()
{
    ConnectionPipe::unRegisterObject(this);

}

/**
 * @brief Model::dropTable
 */
void Model::dropTable()
{
    QString query = QString("DROP TABLE \'%1\'").arg(tablename());
    m_db->excuteSqlQuery(query,QString("%1 droped Sucessfull").arg(tablename()),QString("Failed to drop %1 droped table").arg(tablename()));

}

/**
 * @brief Model::userIdColumnName
 * @return
 */
QString Model::userIdColumnName() const
{
    return userIdColumnName_;
}

/**
 * @brief Model::setUserIdColumnName
 * @param userIdColumnName
 */
void Model::setUserIdColumnName(const QString &userIdColumnName)
{
    userIdColumnName_ = userIdColumnName;
}

QString Model::generateUpdateQuery(QStringList columns, QStringList values, QStringList whereColumns, QStringList likeValues) {
    QString query(QString("UPDATE \"%1\"").arg(tablename()));

    //set
    query.append(" SET ");
    int indexSet = 0;
    foreach (QString column , columns) {
        if (indexSet == 0) {
            query.append(QString(" \"%1\" = \"%2\"").arg(column).arg(values.at(indexSet)));
        } else {
            query.append(QString(", \"%1\" = \"%2\"").arg(column).arg(values.at(indexSet)));
        }
        indexSet++;
    }

    //where
    int indexWhere = 0;
    foreach (QString column , whereColumns) {
        if (indexWhere == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(indexWhere)));

        }
        indexWhere++;
    }

    return query;
}

QString  Model::generateDeleteQuery(QStringList whereColumns, QStringList likeValues) {
    QString query(QString("DELETE FROM \"%1\"").arg(tablename()));
    int index = 0;
    foreach (QString column , whereColumns) {
        if (index == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        }
        index++;
    }


    return query;
}

QString  Model::generateSelectQuery(QStringList selectColumns, QStringList whereColumns, QStringList likeValues, const Range* rangeValues, int limit) {


    QString selectedColumnsStr;

    //selectedColumns
    int index = 0;
    if((selectColumns.length() == 1) && (selectColumns.at(0) == "*")){
        selectedColumnsStr =  "*";
    }else{
        foreach (QString column , selectColumns) {
            if (index == 0) {
                selectedColumnsStr.append(QString(" \"%1\"").arg(column));
            } else {
                selectedColumnsStr.append(QString(", \"%1\"").arg(column));

            }
            index++;
        }
    }

    QString query;

    query.append(QString("SELECT %1 FROM \"%2\"").arg(selectedColumnsStr).arg(tablename()));

    //where
    index = 0;//reset index
    foreach (QString column , whereColumns) {
        if (index == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        }
        index++;
    }

    //range
    if (rangeValues) {
        query.append(QString(" AND %1 BETWEEN \"%2\" AND \"%3\"").arg(rangeValues->column()).arg(rangeValues->first()).arg(rangeValues->last()));
    }

    if(limit > 0){
        query.append(QString(" LIMIT %1").arg(limit));
    }

    return query;
}


QString  Model::generateSelectQuery(QStringList selectColumns, QStringList whereColumns, QStringList likeValues, const QString &orderBy, const Sort::Order &sortOrder, int limit) {


    QString selectedColumnsStr;

    //selectedColumns
    int index = 0;
    if((selectColumns.length() == 1) && (selectColumns.at(0) == "*")){
        selectedColumnsStr =  "*";
    }else{
        foreach (QString column , selectColumns) {
            if (index == 0) {
                selectedColumnsStr.append(QString(" \"%1\"").arg(column));
            } else {
                selectedColumnsStr.append(QString(", \"%1\"").arg(column));

            }
            index++;
        }
    }

    QString query;

    query.append(QString("SELECT %1 FROM \"%2\"").arg(selectedColumnsStr).arg(tablename()));

    //where
    index = 0;//reset index
    foreach (QString column , whereColumns) {
        if (index == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        }
        index++;
    }

    if(!orderBy.isEmpty()){
        if(sortOrder == Sort::Order::DESC){
            query.append(QString(" ORDER BY %1 DESC").arg(orderBy));
        }else{
            query.append(QString(" ORDER BY %1 ASC").arg(orderBy));
        }
    }

    if(limit > 0){
        query.append(QString(" LIMIT %1").arg(limit));
    }

    return query;
}


/////////////////////
QString  Model::generateSelectQuery(QStringList selectColumns, QStringList whereColumns, QStringList likeValues
                                    , const QString &orderBy, const Sort::Order &sortOrder, int limit, int offset) {


    QString selectedColumnsStr;

    //selectedColumns
    int index = 0;
    if((selectColumns.length() == 1) && (selectColumns.at(0) == "*")){
        selectedColumnsStr =  "*";
    }else{
        foreach (QString column , selectColumns) {
            if (index == 0) {
                selectedColumnsStr.append(QString(" \"%1\"").arg(column));
            } else {
                selectedColumnsStr.append(QString(", \"%1\"").arg(column));

            }
            index++;
        }
    }

    QString query;

    query.append(QString("SELECT %1 FROM \"%2\"").arg(selectedColumnsStr).arg(tablename()));

    //where
    index = 0;//reset index
    foreach (QString column , whereColumns) {
        if (index == 0) {
            query.append(QString(" WHERE \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        } else {
            query.append(QString(" AND \"%1\" = \"%2\"").arg(column).arg(likeValues.at(index)));
        }
        index++;
    }

    if(!orderBy.isEmpty()){
        if(sortOrder == Sort::Order::DESC){
            query.append(QString(" ORDER BY %1 DESC LIMIT %2 OFFSET %3").arg(orderBy).arg(limit).arg(offset));
        }else{
            query.append(QString(" ORDER BY %1 ASC LIMIT %2 OFFSET %3").arg(orderBy).arg(limit).arg(offset));
        }
    }

    return query;
}
/////////////
/**
 * @brief Model::Range::Range
 * @param first
 * @param last
 */
Model::Range::Range(const QString &column, const QString &first, const QString &last) {
    first_ = first;
    last_ = last;
    column_ = column;
}

Model::Range::Range(const QString& column,int first, int last, bool inclusive) {
    column_ = column;

    if (!inclusive) {
        first_ = QString::number(first + 1);
        last_ = QString::number(last - 1);

    } else {
        first_ = QString::number(first);
        last_ = QString::number(last);
    }
}

Model::Range::Range(const QString& column,long first, long last, bool inclusive) {
    column_ = column;

    if (!inclusive) {
        first_ = QString::number(first + 1);
        last_ = QString::number(last - 1);

    } else {
        first_ = QString::number(first);
        last_ = QString::number(last);
    }
}

Model::Range::Range(const QString& column,unsigned long first, unsigned long last, bool inclusive) {
    column_ = column;

    if (!inclusive) {
        first_ = QString::number(first + 1);
        last_ = QString::number(last - 1);

    } else {
        first_ = QString::number(first);
        last_ = QString::number(last);
    }
}

Model::Range::Range(const QString& column,double first, double last, bool inclusive) {
    column_ = column;

    if (!inclusive) {
        first_ = QString::number(first + 1);
        last_ = QString::number(last - 1);

    } else {
        first_ = QString::number(first);
        last_ = QString::number(last);
    }
}

Model::Range::Range(const QString& column,unsigned long long first, unsigned long long last, bool inclusive) {
    column_ = column;

    if (!inclusive) {
        first_ = QString::number(first + 1);
        last_ = QString::number(last - 1);

    } else {
        first_ = QString::number(first);
        last_ = QString::number(last);
    }
}

QString Model::Range::last() const
{
    return last_;
}

QString Model::Range::column() const{
    return column_;
}

QString Model::Range::first() const
{
    return first_;
}
