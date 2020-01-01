/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CURRENTUSERIDMODEL_H
#define CURRENTUSERIDMODEL_H

#include <QObject>
#include "Models/model.h"

class AuthenticationController;

class CurrentUserIdModel : public Model
{
    Q_OBJECT
public:
    explicit CurrentUserIdModel(QObject *parent = nullptr);
    bool saveCurrentId(unsigned long id);
    bool idExist();
    unsigned long getCurrentId();

signals:

public slots:
private:
    friend class AuthenticationController;

    MODEL_COLUMN_NAME(id)
    void createTable();
};

#endif // CURRENTUSERIDMODEL_H
