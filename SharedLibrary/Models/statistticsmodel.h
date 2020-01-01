#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include <QObject>
#include "Models/model.h"
class StatisticsModel : public Model
{
    Q_OBJECT

public:
    explicit StatisticsModel(QObject *parent = nullptr);
    bool initStatistic(QString name,int total);
    bool isStatisticExist(QString name);
    bool updateStatistic(QString name,int total);
    bool incrementTotal(QString name);
    bool decrementTotal(QString name);
    int  getTotal(QString name);

signals:
    void newStatisticsCreated(QString name,int total);
    void statisticsChanged(QString name,int total);
public slots:
private:
    void createTable();
};

#endif // STATISTICSMODEL_H
