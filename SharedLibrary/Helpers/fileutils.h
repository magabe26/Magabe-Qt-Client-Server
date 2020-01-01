/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QString>
#include <QByteArray>
#include <QSharedPointer>
#include <QDir>//do not remove,used by a macro

#define MAKE_PATH_IF_NOT_EXISTS(path)\
QDir dir(path);\
if(!dir.exists()){\
    dir.mkpath(path);\
}

class FileUtils
{
public:
    FileUtils();
    static  bool fileExistsOnDisk(const QString &filePath);
    static  qint64 getFileSize(const QString &filePath);
    static  QString getFileExtension(const QString &filePath);
    static  QString getFileName(const QString &filePath);
    static  bool copyFile(const QString &currentPath, const QString &newPath);
    static  QString getFileFolderPath(const QString &filePath);
    static  QString getAppRootPath();
    static  QString getFileBaseName(const QString &filePath);
    static  QString generateFilePath(const QString &folderName, const QString &fileName);
    static  bool deleteFileFromDisc(const QString &filePath);
    static  QString saveFile(QSharedPointer<QByteArray> fileBytes, const QString &folderName, const QString &fileName, bool* saved);
    static  QSharedPointer<QByteArray> getFileByteArray(const QString &filePath);
    static  QSharedPointer<QByteArray> getScaledImage(const QString &imagePath, int quality, int width, int height, Qt::AspectRatioMode aspectMode, bool *ok);
    static  QString scaleImageAndSaveToFolder(const QString &imagePath, const QString &folderName, const QString &fileName, int quality, int width, int height, Qt::AspectRatioMode aspectMode, bool *saved);
    static  bool renameFile(const QString &oldName, const QString &newName);
    static  QString generateAppRootFilePath(const QString &fileName);
    static  bool moveFile(const QString &oldLocation, const QString &newLocation);
    static  QFileInfo getFileInfo(const QString &filePath);
};

#endif // FILEUTILS_H
