/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "magabetime.h"
#include <QDateTime>

#define CONTRUCT_QDATETIME(TIME_STAMP_MSECS)\
    QDateTime datetime;\
    datetime.setMSecsSinceEpoch(TIME_STAMP_MSECS);


QString MagabeTime::getDate(qint64 timeStampMsecs)
{

    CONTRUCT_QDATETIME(timeStampMsecs)
    {
        if(isToday(timeStampMsecs)){
            return QString("Today,%1").arg(datetime.toString(QStringLiteral("dd/MM/yyyy")));
        }else if(isYesterday(timeStampMsecs)){
            return QString("Yesterday,%1").arg(datetime.toString(QStringLiteral("dd/MM/yyyy")));
        }else{
            return datetime.toString(QStringLiteral("dddd,dd/MM/yyyy"));
        }
    }
}

QString MagabeTime::getTime(qint64 timeStampMsecs)
{
    CONTRUCT_QDATETIME(timeStampMsecs)
    {
        // return datetime.toString("hh:mm:ss ap");

        if(isToday(timeStampMsecs)){
            for(int x = 1; x < 60 ; x++){
                if(datetime.toString(QStringLiteral("hh:mmap")) == QDateTime::currentDateTime().addSecs(-60*x).toString(QStringLiteral("hh:mmap"))){
                    return QString::number(x)+QStringLiteral(" minute ago");
                }
            }

            if(datetime.toString(QStringLiteral("hh:mmap")) == QDateTime::currentDateTime().toString(QStringLiteral("hh:mmap"))){
                return QStringLiteral("Just now");
            }else{
                return datetime.toString(QStringLiteral("hh:mm ap"));
            }

        }else{
            return datetime.toString(QStringLiteral("hh:mm ap"));
        }
    }
}


qint64 MagabeTime::getTimeStampMsecsForNow()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

bool MagabeTime::isToday(qint64 timeStampMsecs)
{
    CONTRUCT_QDATETIME(timeStampMsecs)
    {
        if(datetime.toString(QStringLiteral("dd/MM/yyyy")) == QDateTime::currentDateTime().toString(QStringLiteral("dd/MM/yyyy"))){
            return true;
        }else{
            return false;
        }
    }
}

bool MagabeTime::isYesterday(qint64 timeStampMsecs)
{
    CONTRUCT_QDATETIME(timeStampMsecs)
    {
        QDateTime yesterday = QDateTime::currentDateTime().addDays(-1);
        if(datetime.toString(QStringLiteral("dd/MM/yyyy")) == yesterday.toString(QStringLiteral("dd/MM/yyyy"))){
            return true;
        }else{
            return false;
        }
    }
}
