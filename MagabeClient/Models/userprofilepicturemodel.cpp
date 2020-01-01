/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "userprofilepicturemodel.h"
#include "Helpers/fileutils.h"

UserProfilePictureModel::UserProfilePictureModel(QObject *parent) : Model(parent)
{
    setTablename("userProfilePicture");
    setUserIdColumnName("userId");
}


/**
 * @brief UserProfilePictureModel::getPath
 * @param userId
 * @return
 */
QString UserProfilePictureModel::getPath(int userId){

    auto path = getRecordUsingId(pathColumn,userId,"failed to getProfile picture path");
    if(!FileUtils::fileExistsOnDisk(path)){
        deleteRecord(pathColumn,path);
        return "";
    }else{
        return path;
    }
}


/**
 * @brief UserProfilePictureModel::getPath
 * @param userName
 * @return
 */
QString UserProfilePictureModel::getPath(QString userName){

    auto path = getRecord(pathColumn,userNameColumn,userName,"QString UserProfilePictureModel::getPath(QString userName)");
    if(!FileUtils::fileExistsOnDisk(path)){
        deleteRecord(pathColumn,path);
        return "";
    }else{
        return path;
    }
}

void UserProfilePictureModel::deleteProfilePicture(int userId){
    QString path = getPath(userId);
    if(!path.isEmpty()){
        deleteRecord(pathColumn,path);
        FileUtils::deleteFileFromDisc(path);
    }
}


/**
 * @brief UserProfilePictureModel::savePath
 * @param userId
 * @param path
 */
bool UserProfilePictureModel::savePath(int userId,QString userName,QString path)
{
    int id = getRecord(userIdColumnName(),userIdColumnName(),QString::number(userId),"void UserProfilePictureModel::savePath(int userId,QString path)").toInt();


    if(id == userId){ //there is a save record, so update
        return updateRecord(pathColumn,path,userIdColumnName(),QString::number(userId));
    }else {

        QString query = QString("INSERT INTO \"%1\" (\"userId\",\"userName\",\"path\")"
                                "VALUES (\"%2\",\"%3\",\"%4\")").arg(tablename()).arg(QString::number(userId)).arg(userName).arg(path);



        return  m_db->excuteSqlQuery(query,"user profile picture path  saved to db Sucessfull","Failed to saved user profile picture  path to db");
    }
}


/**
 * @brief UserProfilePictureModel::createTable
 */
void UserProfilePictureModel::createTable()
{
    QString query = "CREATE TABLE IF NOT EXISTS \'userProfilePicture\' ( "
                    "\'userId\' INT(11) PRIMARY KEY NOT NULL  UNIQUE,"
                    "\'userName\' varchar(255) NOT NULL,"
                    "\'path\' varchar(255) NOT NULL"
                    " )";

    m_db->excuteSqlQuery(query,"userProfilePicture table created Sucessfull","Failed to create statisttics table");

}


