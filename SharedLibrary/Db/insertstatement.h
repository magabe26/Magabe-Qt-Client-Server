/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef INSERTSTATEMENT_H
#define INSERTSTATEMENT_H
#include <QString>
#include <QMap>
class InsertStatement
{

public:
  InsertStatement() = delete;
  InsertStatement(const InsertStatement& rhs)= delete;
  InsertStatement& operator =(const InsertStatement& rhs) = delete;
  InsertStatement(const InsertStatement&& rhs)= delete;
  InsertStatement& operator =(const InsertStatement&& rhs) = delete;
  InsertStatement(const QString& tableName);
  InsertStatement& insert(const QString& columnName,QString value);
  InsertStatement& insert(const QString& columnName,int value);
  InsertStatement& insert(const QString& columnName,float value);
  InsertStatement& insert(const QString& columnName,bool value);
  InsertStatement& insert(const QString& columnName,double value);
  InsertStatement& insert(const QString& columnName,long long int value);
  InsertStatement& insert(const QString& columnName,QChar value);
  InsertStatement& insert(const QString& columnName, unsigned long value);
  InsertStatement& insert(const QString& columnName,unsigned long long value);
  QString toString();
  bool isNumberValue(const QString& columnName);
private:
  QList<QPair<QString,QString>> columnAndValue_;
  QStringList columnWithNumberValue_;
  QString startStr_;
};

#endif // INSERTSTATEMENT_H
