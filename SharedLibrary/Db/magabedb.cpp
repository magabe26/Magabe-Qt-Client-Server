/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabedb.h"
#include <QDebug>
#include "debug.h"
#include <QDateTime>
#include "Helpers/fileutils.h"

/** ---------------------MSqlQuery------------------------
 * @brief MSqlQuery::MSqlQuery
 * @param db
 */
MSqlQuery::MSqlQuery(QSqlDatabase db)
    :QSqlQuery(db)
{
    connectionName_ = db.connectionName();
}

QString MSqlQuery::connectionName() const
{
    return connectionName_;
}

void MSqlQuery::deleter(MSqlQuery *query)
{
    if(query){
        QString connectionName = query->connectionName();
        query->finish();
        delete query;
        query = nullptr;

        if(!connectionName.isEmpty()){
            QSqlDatabase::removeDatabase(connectionName);
#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            qDebug() << "MagabeDB:: MSqlQuery::deleter , Database removed , connectionName == " << connectionName;
#endif
        }else{
            qDebug() << "MagabeDB:: MSqlQuery::deleter :: Error , connectionName is Empty";
        }
    }else{
        qDebug() << "MagabeDB:: MSqlQuery::deleter :: Error , query is null";
    }
}



/**---------------MagabeDB-------------------------------
 * @brief MagabeDB::MagabeDB
 * @param parent
 */
MagabeDB::MagabeDB(QObject *parent) : QObject(parent)
{
    setDbName(DB_NAME);
}

/*use  create new query
 * HEY FUTURE ME;do not worry about  closing db using db.close(),
 db will be closed when all queries are done*/
QSharedPointer<MSqlQuery> MagabeDB::queryCreateNew(){

    /*------------------------OPENNING DATABASE CONNECTION-----------------*/

    QSqlDatabase db;
    bool dbIsValidAndOpen = false;
    QByteArray connectionName;

    if(!dbIsValidAndOpen){

tryOpening:

        if(dbOpenAttempts_ >= MAX_MAGABEDB_OPEN_ATTEMPTS){ //prevent memory stack overfallow
            //this debug are necessary,do not surround with #if
            print("MagabeDB::Error | Failed to Open Db,"
                  "Check your database configurations");

            dbIsValidAndOpen = false;

            goto prepareQSqlQuery;
        }else{

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print("MagabeDB::Opening "+getDbName()+" database ... , attempt#"+QString::number(dbOpenAttempts_));
#endif

        }

        dbOpenAttempts_ ++; //very important

        /*--------------DATABASE CONNECTION  SETTINGS----------------*/

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        print("MagabeDB::Creating a new db connection ...");
#endif

        //very time consuming , must be here before creating, connectionName//AVOID duplicate connection name
        QString sqliteDbPath = FileUtils::generateAppRootFilePath(getDbName());


createConnectionName:
        connectionName = generateConnectionName(182).toLatin1();//182 is good.decrease the chance of duplicate names,that we Dont want
        if(QSqlDatabase::contains(connectionName)){//check1,very very important
            goto  createConnectionName;
        }
        if(QSqlDatabase::contains(connectionName)){//check2,very very important
            goto  createConnectionName;
        }

        /*-----------FOR QSQLITE DB------------ */
        //#ifdef MGB_CLIENT

        db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"),connectionName);
        db.setDatabaseName(sqliteDbPath);

        //#endif
        /*-----------FOR QMYSQL DB------------ */
        //#ifdef MGB_SERVER

//                db = QSqlDatabase::addDatabase("QMYSQL",connectionName);
//                db.setHostName("127.0.0.1");
//                db.setDatabaseName("magabeserverapp");
//                db.setUserName("root@localhost");
//                db.setPassword("");

        //#endif
        /*------------------------------------------------------------*/


        if (db.open()) {

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print("MagabeDB::"+getDbName()+" database opened(Connection Name = "+ connectionName +").");
#endif
            dbIsValidAndOpen = true;
        }else {
            if(QSqlDatabase::contains(connectionName)){
                db.removeDatabase(connectionName);
            }
            goto tryOpening;
        }
    }

    /*---------------------PREPARING QUERY AND RETURN---------------------------------------*/

prepareQSqlQuery:

    if(!dbIsValidAndOpen){
        qWarning () << "MagabeDB::queryCreateNew():: CriticalError databaseConnection failed to Open, We are in Trouble,Quries will fail to work";
    }

    QSharedPointer<MSqlQuery> query = QSharedPointer<MSqlQuery>(new MSqlQuery(db),&MSqlQuery::deleter);

    dbOpenAttempts_ = 0; //very important

    return query;
}


/**
 * @brief MagabeDB::excuteSqlQuery
 * @param queryStr
 * @param successMessage
 * @param errorMessage
 * @return
 */
bool MagabeDB::excuteSqlQuery(QString queryStr, QString successMessage, QString errorMessage)
{
    auto query = queryCreateNew();

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    print(queryStr);
#else
    Q_UNUSED(successMessage)
    Q_UNUSED(errorMessage)
#endif

    query->exec(queryStr);
    const  bool executed =  query->isActive();

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
    (executed) ? print(successMessage) : print(errorMessage +" :  "+query->lastError().text());
#endif

    return executed;
}


/**
 * @brief MagabeDB::getSqlQuery
 * @return
 */
QSharedPointer<QSqlQuery> MagabeDB::getSqlQuery()
{
    return queryCreateNew();
}


/**
 * @brief MagabeDB::getDbName
 * @return
 */
QString MagabeDB::getDbName() const
{
    return dbName_;
}


/**
 * @brief MagabeDB::setDbName
 * @param dbName
 */
void MagabeDB::setDbName(const QString &dbName)
{
    dbName_ = dbName;
}

/**
 * @brief MagabeDB::generateConnectionName
 * @param numberOfChararacter
 * @return
 */
QString MagabeDB::generateConnectionName(int minNumberOfChararacter)
{
    QString objAddress(QString::number((unsigned long long)this));
    QString str(QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopgrst1234567890"));
    QString generatedStr;
    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    for(int x = 1; x <= minNumberOfChararacter;x++){
        int i = rand()%str.size();
        generatedStr += str.at(i) ;
    }

    generatedStr.append(objAddress).append(QString::number(QDateTime::currentMSecsSinceEpoch()));//min mize duplicates

    while (QSqlDatabase::contains(generatedStr)) { //avoid duplicated

#if PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        print(QStringLiteral("MagabeDB::Connection Name =")+generatedStr+QStringLiteral(" exists ,generating another one"));
#endif
        generatedStr = generateConnectionName(minNumberOfChararacter);
    }

    return generatedStr;
}


/**
 * @brief MagabeDB::print
 * @param status
 */
void MagabeDB::print(QString status)
{
    qDebug() << status <<"\n";
}



