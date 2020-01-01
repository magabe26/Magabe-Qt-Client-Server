#ifndef MODEL_H
#define MODEL_H
#define MODEL_COLUMN_NAME(name) constexpr static const char* name##Column = #name;
#define CREATE_TABLE_IF_NOT_EXISTS(Model) \
    Model t##Model##_;\
    TableCreator Model##Table##CREATOR_(&t##Model##_);

#include <QObject>
#include <iostream>
#include "Db/magabedb.h"
#include <QJsonObject>
#include <QSqlQuery>
#include "security.h"
#include <QSettings>
#include <QDateTime>
#include <QDir>
#include "Helpers/sort.h"

#include <QDebug>

/*NOTE : AVOID CALL createTable()  IN MODEL CONSTRUCTOR
          USING  CREATE_TABLE_IF_NOT_EXISTSINSTEAD IN main.cpp */


class Model : public QObject
{
    Q_OBJECT
public:

    class Range {
    public:
        explicit  Range(const QString& column,const QString& first,const QString& last);
        explicit  Range(const QString& column,int first, int last, bool inclusive);
        explicit  Range(const QString& column,long first, long last, bool inclusive);
        explicit  Range(const QString& column,unsigned long first,unsigned long last, bool inclusive);
        explicit  Range(const QString& column,double first, double last, bool inclusive);
        explicit  Range(const QString& column,unsigned long long first, unsigned long long last, bool inclusive);
        QString first() const;
        QString last() const;
        QString column()const;

    private:
        QString first_;
        QString last_;
        QString column_;
    };

    explicit Model(QObject *parent = nullptr);
    int getRecordCount();
    static QString generateRandomString(int numberOfChararacter);
    static QString generateRandomStringWithNoNumbers(int numberOfChararacter);
    static QString getTimestamp();
    static bool deleteFileFromDisc(const QString &filePath);

    static bool isSignalRegistered(const QString &className, const QString &signalName);
    static bool isSignalRegistered(const QString &signalName);
    static bool emitSignal(const QString &senderClassName, const QString &signalName, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9);
    static bool connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName);
    static QStringList getSignalClassNames(const QString &signalName);

protected:
    MODEL_COLUMN_NAME(rowid)//every table has this by default

    /*derived class must call this on Constractor if they wish
    to register signals,inorder be able to connect to those signals
    using Controller::connectSlot OR ConnectionPipe::connectSlot
    and to be able to emit those signals from any where in a program*/
    void registerModelSignals();
    /*derived class must call this in  destructor  to unregister
      signals registered by registerModelSignals*/
    void unRegisterModel();

    virtual bool deleteRecord(const QString &where,const QString &like);
    //set userIdColumnName first to use this method in your model
    virtual QString getRecordUsingId(const QString &columnName, int userId,const QString &errorMessage);
    virtual QString getRecord(const QString &columnName,const QString &where,const QString &like,const QString &functionName);
    virtual bool updateRecord(const QString &columnName, const QString &value,const QString &where,const QString &like);
    virtual void print(const QString &status);
    QString tablename() const;
    void setTablename(const QString &tablename);
    QString userIdColumnName() const;
    void setUserIdColumnName(const QString &userIdColumnName);
    QString generateUpdateQuery(QStringList columns, QStringList values, QStringList whereColumns, QStringList likeValues);
    QString generateDeleteQuery(QStringList whereColumns, QStringList likeValues);
    QString generateSelectQuery(QStringList columns, QStringList whereColumns, QStringList likeValues
                                ,const Range *rangeValues, int limit);
    QString generateSelectQuery(QStringList selectColumns, QStringList whereColumns, QStringList likeValues, const QString &orderBy,const Sort::Order& sortOrder, int limit);
    QString generateSelectQuery(QStringList selectColumns, QStringList whereColumns
                                , QStringList likeValues
                                ,const QString &orderBy,const Sort::Order &sortOrder,int limit,int offset);

    int getRecordCount(QStringList whereColumns, QStringList likeValues, const Range *rangeValues = nullptr);
    int getNextRowId(const int currentRowId, QStringList whereColumns, QStringList likeValues);

    void dropTable();
    MagabeDB *m_db;
    int getRecordCountImpl(const QString& query);

private:
    friend struct TableCreator;
    virtual void createTable() = 0;
    QString tablename_;
    QString userIdColumnName_;
};

struct TableCreator{
    TableCreator(Model* model){
        if(model){
            model->createTable();
        }
    }
};

#endif // MODEL_H
