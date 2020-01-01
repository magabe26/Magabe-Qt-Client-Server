/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef SERVER_FILE_MODEL
#define SERVER_FILE_MODEL

#include <QObject>
#include "Models/model.h"

class ServerFiles;
class ServerFilesModel : public Model
{
    Q_OBJECT
public:
    explicit ServerFilesModel(QObject *parent = nullptr);
    bool isFileOwner(unsigned long userId, const QString &fileNameOrAlias);
    QString getFilePath(const QString &fileNameOrAlias);
    bool fileExists(const QString &fileNameOrAlias);
private:
    friend class ServerFiles;
    bool updateFileAlias(unsigned long userId, const QString &fileName, const QString &newFileAlias);
    QString getFileAlias(const QString &fileName);
    bool deleteFile(unsigned long userId, const QString &fileNameOrAlias);
    bool  recordFile(int userId,const QString &filePath,const QString &fileName
                     ,const QString &fileAlias);

    MODEL_COLUMN_NAME(fileName)
    MODEL_COLUMN_NAME(fileAlias)
    MODEL_COLUMN_NAME(fileOwner_UserId)
    MODEL_COLUMN_NAME(filePath)
    MODEL_COLUMN_NAME(timestamp)

    void createTable();
};

#endif // SERVER_FILE_MODEL
