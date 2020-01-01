/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONTROLLERIDENTIFIER_H
#define CONTROLLERIDENTIFIER_H

#include <QObject>
#include <QSslSocket>
#include "Helpers/magabedata.h"

class Controller;
class MagabeClient;


//MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
/**
 * @brief The MESAE_Coontroller class
 */
class MESAE_Controller
{
public:
    enum class SearchBy{
        ControllerPointer,
        OtherIncludeTag,
        OtherExecludeTag
    };

    MESAE_Controller();
    explicit MESAE_Controller(Controller *controller,SearchBy searchBy = SearchBy::OtherIncludeTag);
    bool operator==(const MESAE_Controller &other);
    bool operator!=(const MESAE_Controller &other);

private:
    friend class MESAE_Controllers;
    QString action_;
    QString controllerName_;
    Controller *controller_;
    QString tag_;
    SearchBy searchBy_;
};

/** --------------MESAE_Controllers -----------------
 * @brief The MESAE_Controllers class
 */
class MESAE_Controllers {
public:
    void add(Controller *controller);
    Controller *getControllerWith(const QString& controllerName, const QString& action
                                  , Extras receivedData);
    Controller * getControllerWith(const QString &controllerName, const QString &action
                                   , const QString &MESAE_tag);


    ~MESAE_Controllers();
    void remove(Controller *controller);
private:
    QList<MESAE_Controller> list_;
};
////////


class SimpleCrypt;

/**
 * @brief The ControllerIdentifier class
 */
class ControllerIdentifier : public QObject
{

    Q_OBJECT

public:    
    ControllerIdentifier(const ControllerIdentifier& rhs) = delete;
    ControllerIdentifier& operator =(const ControllerIdentifier& rhs) = delete;
    static ControllerIdentifier *instance();
    ~ControllerIdentifier();
    void printControllerStatus(Controller *controller);
    bool isControllerRegistered(const QString& controllerName);
    bool isActionRegistered(const QString& controllerName,const QString& actionName);
    bool isSocketConnected();

signals:

public slots:
    void onDataReceivedFromDataProcessor(QByteArray data);
    void onMESAEEnabledChanged(Controller *controller, bool enabled);
    void onMESAE_WaitForInput(Controller *controller, int msecs);

private:
    friend class MagabeClient;

    explicit ControllerIdentifier(QObject *parent=0);
    void printControllersStatus();
    Controller *getRegisteredController(QString controllerName);
    void print(QString msg);
    QSslSocket *socket_;
    void setSocket(QSslSocket *socket);

    //MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
    MESAE_Controllers MESAE_Coontrollers_;

    SimpleCrypt *crypto_;
};

#define CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(clazz) new clazz(ControllerIdentifier::instance());
#define CONTROLLER_IDENTIFIER_REF &(*ControllerIdentifier::instance())

#endif // CONTROLLERIDENTIFIER_H
