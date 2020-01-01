/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef CREATETABLESTATEMENT_H
#define CREATETABLESTATEMENT_H
#include <QString>

class CreateTableStatement
{
public:
  CreateTableStatement() = delete;
  CreateTableStatement(const CreateTableStatement& rhs)= delete;
  CreateTableStatement& operator =(const CreateTableStatement& rhs) = delete;
  CreateTableStatement(const CreateTableStatement&& rhs)= delete;
  CreateTableStatement& operator =(const CreateTableStatement&& rhs) = delete;
  CreateTableStatement(const QString& tableName);
  CreateTableStatement(const char * tableName);
  QString tableName() const;
  void setTableName(const QString &tableName);
  CreateTableStatement& addColumn(const QString& columnName, QString dataType, bool primaryKey, bool notNull, bool unique);
  CreateTableStatement& addColumn(const char* columnName, const char* dataType, bool primaryKey, bool notNull, bool unique);
  QString toString() const;

private:
  QString startStr_;
  const QString terminatingChar_ ="#";
  QString createStatementStr_;
  QString tableName_;

};

#endif // CREATETABLESTATEMENT_H
