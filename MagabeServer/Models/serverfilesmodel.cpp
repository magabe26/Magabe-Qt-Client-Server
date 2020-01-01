/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */



#include "Models/serverfilesmodel.h"
#include "Models/statistticsmodel.h"
#include "Db/createtablestatement.h"
#include "Db/insertstatement.h"

ServerFilesModel::ServerFilesModel(QObject *parent) : Model(parent)
{
    setTablename("ServerFiles");
    setUserIdColumnName("userId");
}

QString ServerFilesModel::getFilePath(const QString &fileNameOrAlias)
{
start:
    QString result = getRecord(filePathColumn,fileNameColumn,fileNameOrAlias,"QString ServerFilesModel::getFilePath(const QString &fileNameOrAlias)");
    if(result.isEmpty()){
        result = getRecord(filePathColumn,fileAliasColumn,fileNameOrAlias,"QString ServerFilesModel::getFilePath(const QString &fileNameOrAlias)");
    }
    if((!result.isEmpty()) && (!QFile::exists(result))){
        auto fAlias = getFileAlias(fileNameOrAlias); //only Alias is guranteed to be unique
        if(!fAlias.isEmpty()){
            deleteRecord(fileAliasColumn,fAlias);
        }
        result.clear();
        goto start;
    }
    return result;
}

QString ServerFilesModel::getFileAlias(const QString &fileName)
{
    QString result = getRecord(fileAliasColumn,fileNameColumn,fileName,"ServerFilesModel::getFileAlias");
    if(result.isEmpty()){
        result = getRecord(fileAliasColumn,fileAliasColumn,fileName,"ServerFilesModel::getFileAlias");
    }
    return result;
}

bool ServerFilesModel::recordFile(int userId, const QString &filePath, const QString &fileName, const QString &fileAlias)
{
    bool isFilePreviousExist = isFileOwner(userId,fileName);
    InsertStatement insertStatement(tablename());
    insertStatement.insert(fileOwner_UserIdColumn,userId)
            .insert(fileNameColumn,fileName)
            .insert(fileAliasColumn,fileAlias)
            .insert(filePathColumn,filePath)
            .insert(timestampColumn,getTimestamp());

    if(m_db->excuteSqlQuery(insertStatement.toString(),"--> New File recorded To Db","--> Failed to recorded  new File To Db  ")){
        if(!isFilePreviousExist){
            StatisticsModel().incrementTotal(tablename());
        }

        return true;
    }else{
        return false;
    }
}


bool ServerFilesModel::deleteFile(unsigned long userId, const QString &fileNameOrAlias)
{
    if(fileExists(fileNameOrAlias)){
        const QString filePath = getFilePath(fileNameOrAlias);

        QStringList whereColumns;
        whereColumns << fileOwner_UserIdColumn << fileAliasColumn;

        QStringList likeValues;
        likeValues << QString::number(userId) << getFileAlias(fileNameOrAlias);

        QString query = generateDeleteQuery(whereColumns,likeValues);

        if(m_db->excuteSqlQuery(query,"ServerFilesModel::deleteFile ;File deleted","ServerFilesModel::deleteFile ;File  deletio failed")){
            return  deleteFileFromDisc(filePath);
        }else{
            qWarning() << "ServerFilesModel::deleteFile Failed to delete db record";
            return  false;
        }

    }else{
        return false;
    }
}


bool ServerFilesModel::isFileOwner(unsigned long userId, const QString &fileNameOrAlias)
{
    if((userId == 0) || (fileNameOrAlias.isEmpty())){
        return false;
    }

    QString result = getRecord(fileOwner_UserIdColumn,fileNameColumn,fileNameOrAlias,"ServerFilesModel::isFileOwner");
    if(result.isEmpty()){
        result = getRecord(fileOwner_UserIdColumn,fileAliasColumn,fileNameOrAlias,"ServerFilesModel::isFileOwner");
    }
    if(result.isEmpty()){
        return false;
    }
    bool ok = false;
    unsigned long id = result.toULong(&ok);
    return (ok)? ( id == userId) : false;
}

bool ServerFilesModel::fileExists(const QString &fileNameOrAlias)
{
    QString filePath = getFilePath(fileNameOrAlias);
    bool exists =  (!filePath.isEmpty());
    if(!exists){//does not exists on db
        return false;
    }else{
        exists = QFile::exists(filePath); //check if exist on disc
        if(!exists){//this means only the db record exists,so delete it
            deleteRecord(filePathColumn,filePath);
        }
        return exists;
    }
}

bool ServerFilesModel::updateFileAlias(unsigned long userId,const QString &fileName, const QString &newFileAlias)
{
    QStringList colums;
    colums << fileAliasColumn;

    QStringList values;
    values << newFileAlias;

    QStringList whereColumns;
    whereColumns << fileOwner_UserIdColumn << fileNameColumn;

    QStringList likeValues;
    likeValues << QString::number(userId) << fileName;

    QString query = generateUpdateQuery(colums,values,whereColumns,likeValues);

    return m_db->excuteSqlQuery(query,QString("%1 updated to  %2").arg(fileAliasColumn).arg(newFileAlias),QString("failed to update %1 to  %2").arg(fileAliasColumn).arg(newFileAlias));

}

void ServerFilesModel::createTable()
{
    CreateTableStatement createTableStatement(tablename());
    createTableStatement.addColumn(fileOwner_UserIdColumn,"INT(11)",false,true,false)
            .addColumn(fileNameColumn,"varchar(180)",false,true,false)
            .addColumn(fileAliasColumn,"varchar(160)",true,true,true)
            .addColumn(filePathColumn,"varchar(255)",false,true,false)
            .addColumn(timestampColumn,"int(11)",false,true,false);

    if(m_db->excuteSqlQuery(createTableStatement.toString(),tablename()+" table created Sucessfull","Failed to create "+tablename()+ "table")){
        StatisticsModel  statisticsModel;
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        print("initilizing "+ tablename() +" statistics ...");
#endif
        if(statisticsModel.initStatistic(tablename(),0)){//it will not overwrite if record is already created
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
            print(tablename() +" statistics initilized ...");
#endif
        }
#if  PRINT_MAGABE_DB_AND_MODEL_MESSAGES
        else{
            print(tablename() +" statistics  already initilized ...");
        }
#endif
    }
}
