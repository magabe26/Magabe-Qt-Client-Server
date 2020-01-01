/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MAGABETIME_H
#define MAGABETIME_H
#include <QString>

class MagabeTime
{
public:
   static  QString getDate(qint64 timeStampMsecs);
   static  QString getTime(qint64 timeStampMsecs);
   static  bool isToday(qint64 timeStampMsecs);
   static  bool isYesterday(qint64 timeStampMsecs);
   static  qint64 getTimeStampMsecsForNow();
};

#endif // MAGABETIME_H
