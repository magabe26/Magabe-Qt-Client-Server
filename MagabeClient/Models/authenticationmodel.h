/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef AUTHENTICATIONMODEL_H
#define AUTHENTICATIONMODEL_H

#include <QObject>
#include "Models/model.h"

class AuthenticationController;

class AuthenticationModel : public Model
{
    Q_OBJECT
public:
    explicit AuthenticationModel(QObject *parent = nullptr);

    unsigned long getId(const QString &username);
    QString getUserName(unsigned long id);

    QString getAuthKey(unsigned long id);
    QString getPassword(unsigned long id);

    QString getAuthKey(const QString &username);
    QString getPassword(const QString &username);

    bool updateCredetials(unsigned long id, const QString& username,
                          const QString& authKey, const QString &password);
    bool updateAuthKey(const QString &newAuthKey, unsigned long userId);
signals:

public slots:

private:
    friend class AuthenticationController;
    MODEL_COLUMN_NAME(id)
    MODEL_COLUMN_NAME(username)
    MODEL_COLUMN_NAME(authKey)
    MODEL_COLUMN_NAME(password)

    void createTable();

};

#endif // AUTHENTICATIONMODEL_H
