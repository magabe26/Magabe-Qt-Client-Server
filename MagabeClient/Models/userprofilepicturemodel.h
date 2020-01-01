/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef USERPROFILEPICTUREMODEL_H
#define USERPROFILEPICTUREMODEL_H

#include "Models/model.h"

class UserProfilePictureModel : public Model
{
    Q_OBJECT
public:
    explicit UserProfilePictureModel(QObject *parent = nullptr);

    QString getPath(int userId);
    bool savePath(int userId, QString userName, QString path);
    QString getPath(QString userName);
    void deleteProfilePicture(int userId);

signals:

public slots:
private:
    MODEL_COLUMN_NAME(path)
    MODEL_COLUMN_NAME(userName)
    void createTable();
};

#endif // USERPROFILEPICTUREMODEL_H
