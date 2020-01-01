/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONNECTED_CLIENTS_MODEL_H
#define CONNECTED_CLIENTS_MODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <QDebug>
#include "Models/usermodel.h"

class Client :public QStandardItem {

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        UsernameRole,
        AuthKeyRole,
        PasswordRole,
        EmailRole,
        PhoneNumberRole,
        FirstNameRole,
        LastNameRole,
        DateOfBirthRole,
        GenderRole,
        CreatedAtRole,
        UpdatedAtRole,
        ProfilePicturePathRole,
        PositionRole,
        EducationRole,
        LocationRole,
        MgbSessionId
    };

    Client(const unsigned long id,const QString& mgbSessionId);
    Client(const unsigned long id);
    int type() const;
    QVariant data(int role) const;
    unsigned long id() const;
    QString mgbSessionId() const;
    bool isRoot() const;

private:
    unsigned long id_;
    QString mgbSessionId_;
    bool isRoot_;
    UserModel userModel_;

};


class ClientHolder{
public:
    enum class SearchBy{
        UserId,
        Ptr,
        MgbSessionId,
        All
    };

    ClientHolder(const QString& mgbSessionId, unsigned long id,Client *ptr);
    bool operator ==(const ClientHolder& other );
    bool operator != (const ClientHolder& other );
    unsigned long id() const;
    void setId(unsigned long id);
    Client *ptr() const;
    void setPtr(Client *ptr);
    SearchBy searchBy() const;
    void setSearchBy(const SearchBy &searchBy);
private:
    unsigned long id_;
    QString mgbSessionId_;
    Client *ptr_;
    SearchBy searchBy_;
};


class ConnectedClientsModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit ConnectedClientsModel(QObject *parent = nullptr);
    void swap(const QString& mgbSessionId,const unsigned long userId = 0);
    void remove(const QString& mgbSessionId);

signals:

private:
    friend class Client;
    QList<ClientHolder> clients_;

    bool hasRoot(Client *item);
    bool containClientWith(const QString& mgbSessionId);
    Client *getItem(const QString& mgbSessionId);
    Client *getRootItem(Client *item);

};

#endif // CONNECTED_CLIENTS_MODEL_H
