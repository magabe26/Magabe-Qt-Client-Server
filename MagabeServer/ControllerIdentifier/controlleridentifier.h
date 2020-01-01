/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef CONTROLLERIDENTIFIER_H
#define CONTROLLERIDENTIFIER_H

#include <QObject>
#include <QSslSocket>
#include "Helpers/magabedata.h"
#include <QHash>
#include <QTimer>
#include "Controllers/controller.h"

class Controller;

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
    unsigned long userId_;
    QString mgbSessionId_;
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
    Controller *getControllerWith(const QString& controllerName,const QString& action
                                  ,const QString& mgbSessionId,unsigned long userId
                                  ,Extras receivedData);
    Controller * getControllerWith(const QString &controllerName, const QString &action
                                   , const QString &mgbSessionId, unsigned long userId
                                   ,const QString &MESAE_tag);


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

    bool isControllerRegistered(QString controllerName);
    bool isActionRegistered(const QString &controllerName, const QString &actionName);
signals:
    /*do not change this signal, its connected through ConnectionPipe
     * and  used to close socket connections*/
    void closeClientSocket(QString mgbSessionId);
public slots:
    void onDataReceivedFromDataProcessor(QByteArray data, QString mgbSessionId);
    void onMESAEEnabledChanged(Controller *controller,bool enabled);
    void onMESAE_WaitForInput(Controller *controller,int msecs);

private:
    explicit ControllerIdentifier(QObject *parent=0);

    void identify(QString data, QString mgbSessionId);
    void selectController(MagabeData data);
    void printControllersStatus();
    void printControllerStatus(Controller *controller);
    Controller *getRegisteredController(QString controllerName);
    void print(QString msg);

    //MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
    MESAE_Controllers MESAE_Coontrollers_;

    SimpleCrypt *crypto_;

};

#define CONTROLLER_IDENTIFIER_REGISTER_CONTROLLER(clazz) new clazz(ControllerIdentifier::instance());
#define CONTROLLER_IDENTIFIER_REF &(*ControllerIdentifier::instance())

#endif // CONTROLLERIDENTIFIER_H
