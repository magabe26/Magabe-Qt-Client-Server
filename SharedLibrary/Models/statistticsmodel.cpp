#include "statistticsmodel.h"

StatisticsModel::StatisticsModel(QObject *parent) : Model(parent)
{
    setTablename("statisttics");
}

void StatisticsModel::createTable()
{
    QString query = "CREATE TABLE IF NOT EXISTS \'statisttics\' ( "
                    "\'name\' varchar(255) PRIMARY KEY NOT NULL  UNIQUE,"
                    "\'total\' INT(11) NOT NULL"
                    " )";

    //  writeToStausPage(query);

    m_db->excuteSqlQuery(query,"statisttics table created Sucessfull","Failed to create statisttics table");

}


bool StatisticsModel::initStatistic(QString name, int total)
{
    QString query = QString("INSERT INTO \"%1\" (\"name\",\"total\") VALUES (\"%2\",\"%3\")").arg(tablename()).arg(name).arg(QString::number(total));
    return  m_db->excuteSqlQuery(query,"new record inserted","Failed to insert new record");

}

bool StatisticsModel::isStatisticExist(QString name)
{
    QString dbRecord = getRecord("name","name",name,"bool StatistticsModel::isRecordExist(QString Name)");
    return (!dbRecord.isEmpty() && (dbRecord == name));
}

bool StatisticsModel::updateStatistic(QString name, int total)
{
    if(!isStatisticExist(name)){
        bool result = initStatistic(name,total);
        emit newStatisticsCreated(name,total);
        return result;
    }else{
        bool result = updateRecord("total",QString::number(total),"name",name);
        emit statisticsChanged(name,total);
        return result;
    }
}

bool StatisticsModel::incrementTotal(QString name)
{
    int total = getTotal(name);
    total ++;

    return  updateStatistic(name,total);

}

bool StatisticsModel::decrementTotal(QString name)
{

    int total = getTotal(name);
    total --;

    return  updateStatistic(name,total);
}

int StatisticsModel::getTotal(QString name)
{
    if(!isStatisticExist(name)){

        return 0;
    }else{

        return getRecord("total","name",name,"int StatisticsModel::getTotal(QString Name)").toInt();

    }
}
