/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONNECTNADB_H
#define CONNECTNADB_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
//#include <QSqlDriver>
#include <QSqlQuery>
#include "Helpers/constants.h"
#include <QSharedPointer>

class MSqlQuery : public QSqlQuery
{
public:
    explicit MSqlQuery(QSqlDatabase db);
    QString connectionName() const;
    static void deleter(MSqlQuery *query);
private:
    QString connectionName_;
};

/**
 * @brief The MagabeDB class
 */
class MagabeDB : public QObject
{
    Q_OBJECT

public:
    explicit MagabeDB(QObject *parent = 0);
    bool excuteSqlQuery(QString queryStr,QString successMessage,QString errorMessage);
    QSharedPointer<QSqlQuery> getSqlQuery();
    QString getDbName() const;
    void setDbName(const QString &dbName);

signals:

public slots:
private:
    QString dbName_;
    int dbOpenAttempts_ = 0;
    QSharedPointer<MSqlQuery> queryCreateNew();
    //for generating database name with random name
    //each model obj uses its on database name
    QString generateConnectionName(int minNumberOfChararacter);
    void print(QString status);

};

#endif // CONNECTNADB_H
