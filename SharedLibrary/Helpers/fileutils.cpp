/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "fileutils.h"
#include <QFileInfo>
#include "Helpers/constants.h"
#include <QDebug>
#include <QBuffer>
#include <QPixmap>
#include <QImage>
#include <QImageReader>

FileUtils::FileUtils()
{

}

bool FileUtils::fileExistsOnDisk(const QString &filePath){
    return QFileInfo::exists(filePath);
}

qint64 FileUtils::getFileSize(const QString &filePath){
    return getFileInfo(filePath).size();
}

QString FileUtils::getFileExtension(const QString &filePath){
    if(filePath.isEmpty()) return "";
    return getFileInfo(filePath).completeSuffix();
}

QString FileUtils::getFileName(const QString &filePath){
    if(filePath.isEmpty()) return "";
    return getFileInfo(filePath).fileName();
}

QFileInfo FileUtils::getFileInfo(const QString &filePath){
    return QFileInfo(filePath);
}

bool FileUtils::copyFile(const QString &currentPath, const QString &newPath)
{
    if(QFile::exists(newPath)){//if the file already exists
        return true;
    }
    return QFile::copy(currentPath,newPath);
}

bool FileUtils::moveFile(const QString &oldLocation, const QString &newLocation){
    if(oldLocation.isEmpty() || newLocation.isEmpty()){
        return false;
    }

    { MAKE_PATH_IF_NOT_EXISTS(getFileFolderPath(newLocation)) }

    if(FileUtils::fileExistsOnDisk(newLocation)){
        FileUtils::deleteFileFromDisc(newLocation);
    }
    if(FileUtils::copyFile(oldLocation,newLocation)){
        return FileUtils::deleteFileFromDisc(oldLocation);
    }else{
        return false;
    }
}

QString FileUtils::getFileFolderPath(const QString &filePath){
    if(filePath.isEmpty()) return "";
    return  getFileInfo(filePath).absoluteDir().absolutePath();
}

QString FileUtils::getAppRootPath(){
    return QDir::homePath().append(QString("/.%1").arg(APP_FOLDER));
}

QString FileUtils::getFileBaseName(const QString &filePath){
    if(filePath.isEmpty()) return "";
    return  getFileInfo(filePath).baseName();
}

QString FileUtils::generateFilePath(const QString &folderName,const QString &fileName){
    QString folder = getAppRootPath().append(QString("/%1").arg(folderName));
    { MAKE_PATH_IF_NOT_EXISTS(folder) }
    return folder.append(QString("/%1").arg(fileName));
}

QString FileUtils::generateAppRootFilePath(const QString &fileName){
    QString folder = getAppRootPath();
    { MAKE_PATH_IF_NOT_EXISTS(folder) }
    return folder.append(QString("/%1").arg(fileName));
}

bool FileUtils::deleteFileFromDisc(const QString &filePath)
{
    QDir filepathDir;
    return filepathDir.remove(filePath);
}

QString FileUtils::saveFile(QSharedPointer<QByteArray> fileBytes,const QString &folderName,const QString &fileName, bool *saved)
{
    if(saved){
        *saved = false;
    }
    QString filePath = generateFilePath(folderName,fileName);
    if(fileExistsOnDisk(filePath)){
        deleteFileFromDisc(filePath);
    }
    QFile  file(filePath);
    bool fileSaved = false;
    if(!file.open(QIODevice::WriteOnly)){
        qWarning()<< "failed to open"+filePath+"for serving";
        fileSaved = false;
    }else{
        file.write(fileBytes->data(),fileBytes->size());
        fileSaved = file.flush();
        file.close();
    }
    if(saved){
        *saved = fileSaved;
    }
    return filePath;
}

QSharedPointer<QByteArray> FileUtils::getFileByteArray(const QString &filePath)
{

    QFile file(filePath);
    QSharedPointer<QByteArray> byteArray = QSharedPointer<QByteArray>(new QByteArray);
    if(file.open(QIODevice::ReadOnly)){
        byteArray->append(file.readAll());
        file.close();
    }

    return byteArray;
}

QSharedPointer<QByteArray> FileUtils::getScaledImage(const QString& imagePath, int quality
                                                     , int width, int height
                                                     , Qt::AspectRatioMode aspectMode,bool* ok){

    QByteArray fileExtension = getFileExtension(imagePath).toLatin1();
    QImageReader imgReader(imagePath,fileExtension);

    QSharedPointer<QByteArray> byteArray = QSharedPointer<QByteArray>(new QByteArray);

    bool allGood = imgReader.canRead();
    if(ok){
        *ok = allGood;
    }
    if(!allGood){
        return byteArray;
    }

    imgReader.setQuality(quality);
    auto scaledImg = imgReader.read().scaled(width,height,aspectMode,Qt::SmoothTransformation);

    QBuffer buffer(byteArray.data());
    allGood = buffer.open(QIODevice::WriteOnly);
    allGood = scaledImg.save(&buffer,fileExtension);
    buffer.close();

    if(ok){
        *ok = allGood;
    }
    //return image with small bytes size
    if(getFileSize(imagePath) > byteArray->size()){
        return byteArray;
    }else{
        return getFileByteArray(imagePath);
    }
}

QString FileUtils::scaleImageAndSaveToFolder(const QString &imagePath,const QString &folderName,const QString &fileName
                                             ,int quality, int width, int height
                                             , Qt::AspectRatioMode aspectMode,bool* saved){
    bool scaled = false;
    auto imgData =  getScaledImage(imagePath,quality,width,height,aspectMode,&scaled);
    if(saved){
        *saved = scaled;
    }
    if(!scaled){
        imgData.clear();
        return "";
    }
    return saveFile(imgData,folderName,fileName,saved);
}

bool FileUtils::renameFile(const QString &oldName, const QString &newName){
    return QFile::rename(oldName,newName);
}
