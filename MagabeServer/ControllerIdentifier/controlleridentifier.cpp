/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#include "controlleridentifier.h"
#include <iostream>
#include <QDebug>
#include <QThreadPool>
#include "Server/connectedusers.h"
#include "security.h"
#include "debug.h"
#include "Helpers/connectionpipe.h"

//MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE

/**           --------------MESAE_Controller--------------------
 * @brief MESAE_Controller::MESAE_Controller
 * @param controller
 * @param action
 * @param mgbSessionId
 * @param userId
 */
MESAE_Controller::MESAE_Controller()
    :controller_(nullptr)
    ,searchBy_(SearchBy::OtherIncludeTag)
{}

MESAE_Controller::MESAE_Controller(Controller *controller, SearchBy searchBy){
    if(controller){
        auto user = controller->user_;
        this->controllerName_ = controller->name();
        this->action_ = controller->getCurrentExecutingAction();
        this->mgbSessionId_ = user->getThisClientMgbSessionId();
        this->userId_ = user->id();
        this->controller_ = controller;
        this->tag_ = controller->MESAE_tag_;
    }else{
        this->userId_ = 0;
        this->controller_ = nullptr;
    }
    this->searchBy_ = searchBy;
}

bool MESAE_Controller::operator==(const MESAE_Controller &other)
{

    switch (other.searchBy_) {

    case SearchBy::ControllerPointer:
        return (this->controller_ == other.controller_);

    case SearchBy::OtherIncludeTag:
        return ((this->userId_ == other.userId_)
                && (this->action_ == other.action_)
                && (this->controllerName_ == other.controllerName_)
                && (this->mgbSessionId_ == other.mgbSessionId_)
                && (this->tag_ == other.tag_));

    case SearchBy::OtherExecludeTag:
    default:
        return ((this->userId_ == other.userId_)
                && (this->action_ == other.action_)
                && (this->controllerName_ == other.controllerName_)
                && (this->mgbSessionId_ == other.mgbSessionId_));
    }

}

bool MESAE_Controller::operator !=(const MESAE_Controller &other)
{

    switch (other.searchBy_) {

    case SearchBy::ControllerPointer:
        return (this->controller_ != other.controller_);

    case SearchBy::OtherIncludeTag:
        return ((this->userId_ != other.userId_)
                || (this->action_ != other.action_)
                || (this->controllerName_ != other.controllerName_)
                || (this->mgbSessionId_ != other.mgbSessionId_)
                || (this->tag_ != other.tag_));

    case SearchBy::OtherExecludeTag:
    default:
        return ((this->userId_ != other.userId_)
                || (this->action_ != other.action_)
                || (this->controllerName_ != other.controllerName_)
                || (this->mgbSessionId_ != other.mgbSessionId_));
    }
}


/** -------------------MESAE_Controllers------------------
 * @brief MESAE_Controllers::add
 * @param controller
 */
void MESAE_Controllers::add(Controller *controller){
    if(controller){
        list_.append(MESAE_Controller(controller));
#if PRINT_MESAE_DEBUG_MESSAGES
        controller->print(QString("MESAE_Coontroller %1:%2 for userId# %3 (MESAE_Tag = %4) ;- ADDED TO ACTIVE LIST")
                          .arg(controller->name()).arg(controller->getCurrentExecutingAction())
                          .arg(controller->user_->id()).arg( controller->MESAE_tag_));
#endif
    }
}

void MESAE_Controllers::remove(Controller *controller){
    if(!controller){
        return;
    }

    MESAE_Controller signature;
    signature.searchBy_ = MESAE_Controller::SearchBy::ControllerPointer;
    signature.controller_ = controller;

    for(int index = list_.indexOf(signature);
        index != -1;
        index = list_.indexOf(signature)){//no need to increment , bcoz we are removing it

        Controller *ctrlr = list_.takeAt(index).controller_;//remove the item
        if(ctrlr){
#if PRINT_MESAE_DEBUG_MESSAGES
            ctrlr->print(QString("MESAE_Coontroller %1:%2 for userId# %3 (MESAE_Tag = %4) ;- REMOVED FROM ACTIVE LIST")
                         .arg(ctrlr->name()).arg(ctrlr->getCurrentExecutingAction())
                         .arg(ctrlr->user_->id()).arg(ctrlr->MESAE_tag_));
#endif
        }

    }
}

Controller * MESAE_Controllers::getControllerWith(const QString &controllerName, const QString &action, const QString &mgbSessionId, unsigned long userId, Extras receivedData){

    Controller *controller = nullptr;
    MESAE_Controller signature;
    signature.searchBy_ = MESAE_Controller::SearchBy::OtherExecludeTag;//this means we needed to validate the tag
    signature.controllerName_ = controllerName;
    signature.action_ = action;
    signature.mgbSessionId_ = mgbSessionId;
    signature.userId_ = userId;

    bool tagValidated = false;
    QList<int> withNullController;
    for(int index = list_.indexOf(signature);
        index != -1;
        index = list_.indexOf(signature,(++index)) ){

        Controller *ctrlr = list_.at(index).controller_;
        if(ctrlr){

            if(ctrlr->MESAE_onValidateTagCallback_){

                if(ctrlr->MESAE_onValidateTagCallback_(ctrlr->MESAE_tag_,receivedData)){
                    controller = ctrlr;
                    tagValidated = true;
                    break;
                }

            }else{ //MESAE_onValidateTagCallback is not set/null use default behaviour
#if PRINT_MESAE_DEBUG_MESSAGES
                ctrlr->print(QString("<2>MESAE_Coontroller %1:%2 for userId# %3 (MESAE_Tag = %4 ) ;- FOUND")
                             .arg(ctrlr->name()).arg(ctrlr->getCurrentExecutingAction())
                             .arg(ctrlr->user_->id()).arg(ctrlr->MESAE_tag_));
#endif
                controller = ctrlr;
                break;
            }

        }else{
            withNullController.append(index);
        }

    }

    foreach (int index, withNullController) {
#if PRINT_MESAE_DEBUG_MESSAGES
        qWarning() << "MESAE_Controllers::getControllerWith | EXPIRED MESAE_Controller REMOVED FROM ACTIVE LIST, removeAt = " << index;
#endif
        list_.removeAt(index);
    }

    if(tagValidated && controller){
#if PRINT_MESAE_DEBUG_MESSAGES
        controller->print(QString("MESAE_Coontroller %1:%2 for userId# %3 (MESAE_Tag = %4 ) ;- MESAE TAG VALIDATED to TRUE")
                     .arg(controller->name()).arg(controller->getCurrentExecutingAction())
                     .arg(controller->user_->id()).arg(controller->MESAE_tag_));
#endif

    }
    return controller;
}

Controller *MESAE_Controllers::getControllerWith(const QString &controllerName, const QString &action
                                                 , const QString &mgbSessionId
                                                 , unsigned long userId, const QString &MESAE_tag){

    Controller *controller = nullptr;
    MESAE_Controller signature;
    signature.searchBy_ = MESAE_Controller::SearchBy::OtherIncludeTag;
    signature.controllerName_ = controllerName;
    signature.action_ = action;
    signature.mgbSessionId_ = mgbSessionId;
    signature.userId_ = userId;
    signature.tag_ = MESAE_tag;

    QList<int> withNullController;
    for(int index = list_.indexOf(signature);
        index != -1;
        index = list_.indexOf(signature,(++index)) ){
        Controller *ctrlr = list_.at(index).controller_;
        if(ctrlr){
            controller = ctrlr;
#if PRINT_MESAE_DEBUG_MESSAGES
            ctrlr->print(QString("<2>MESAE_Coontroller %1:%2 for userId# %3 (MESAE_Tag = %4 ) ;- FOUND")
                         .arg(ctrlr->name()).arg(ctrlr->getCurrentExecutingAction())
                         .arg(ctrlr->user_->id()).arg(ctrlr->MESAE_tag_));
#endif
            break;
        }else{
            withNullController.append(index);
        }

    }

    foreach (int index, withNullController) {
#if PRINT_MESAE_DEBUG_MESSAGES
        qWarning() << "MESAE_Controllers::getControllerWith | EXPIRED MESAE_Controller REMOVED FROM ACTIVE LIST, removeAt = " << index;
#endif
        list_.removeAt(index);
    }

    return controller;
}


MESAE_Controllers::~MESAE_Controllers(){
    list_.clear();
}




/** -------------------ControllerIdentifier--------------------
 * @brief ControllerIdentifier::ControllerIdentifier
 * @param parent
 */
ControllerIdentifier::ControllerIdentifier(QObject *parent) : QObject(parent)
{
    crypto_ = Security::instance();
    ConnectionPipe::registerObject(this); //using my awesome signal manager
}

ControllerIdentifier *ControllerIdentifier::instance(){
    // no need to delete it, it will be deleted by Os, when app exit,coz it is static
    static ControllerIdentifier *obj;
    if(!obj){
        try{
            obj = new ControllerIdentifier();
        }catch(std::bad_alloc e){
            qDebug() << "ControllerIdentifier:: " << e.what();
            return instance();
        }catch(...){
            qDebug() << "ControllerIdentifier:: memory allocation error";
            return instance();
        }
    }
    return obj;
}

ControllerIdentifier::~ControllerIdentifier(){
    ConnectionPipe::unRegisterObject(this);
}

bool ControllerIdentifier::isControllerRegistered(QString controllerName)
{
    QObjectList controllerIdentifierObjects = children();
    foreach (QObject *obj, controllerIdentifierObjects) {
        if(obj->inherits("Controller")){
            Controller *appController  = qobject_cast<Controller *>(obj);
            if(controllerName == appController->name()){
                return true;
            }
        }
    }
    return false;
}

bool ControllerIdentifier::isActionRegistered(const QString &controllerName, const QString &actionName)
{
    QObjectList controllerIdentifierObjects = children();
    foreach (QObject *obj, controllerIdentifierObjects) {
        if(obj->inherits("Controller")){
            Controller *appController  = qobject_cast<Controller *>(obj);
            if((appController) && (controllerName == appController->name())){
                if(!appController->isRegisteredAction(actionName)){
                    appController->registerActions();
                }
                return (appController->isRegisteredAction(actionName));
            }
        }
    }
    return false;
}

Controller * ControllerIdentifier::getRegisteredController(QString controllerName)
{
    QObjectList controllerIdentifierObjects = children();
    foreach (QObject *obj, controllerIdentifierObjects) {
        if(obj->inherits("Controller")){
            Controller *appController  = qobject_cast<Controller *>(obj);
            if(controllerName == appController->name()){
                return appController;
            }
        }
    }
    return nullptr;
}

/**
 * @brief ControllerIdentifier::identify
 * @param data
 * @param mgbSessionId
 */
void ControllerIdentifier::identify(QString data, QString mgbSessionId)
{
#if PRINT_DATA_RECEIVED
    print("ControllerIdentifier::identify called");
#endif
    MagabeData magabeData(data);

    if(magabeData.isEmpty()){
        print("ControllerIdentifier::Error, received empty data!");
        return;
    }

    //Adding mgbSessionId
    magabeData.setMgbSessionId(mgbSessionId);

    selectController(magabeData);

}

/**
 * @brief ControllerIdentifier::selectController
 * @param data
 */
void ControllerIdentifier::selectController(MagabeData data)
{

    QString controllerName = data.controller();
    controllerName[0] = controllerName[0].toUpper();

    const auto mgbSessionId = data.getMgbSessionId();
    unsigned long userId = ConnectedUserList::instance()->getUserId(mgbSessionId);
    QString actionName = data.action();
    Extras receivedData(data.toByteArray());

    Controller * MESAE_Coontroller = MESAE_Coontrollers_.getControllerWith(controllerName,actionName
                                                                           ,mgbSessionId,userId,receivedData);
    if(MESAE_Coontroller){

#if PRINT_MESAE_DEBUG_MESSAGES
        print("ControllerIdentifier::selectController ;-is a MESAE_Coontrollers UpdatingInputExtras ...");
#endif

        MESAE_Coontroller->MESAE_UpdateInputExtras(data);
        return;
    }

#if PRINT_MESAE_DEBUG_MESSAGES
    print("ControllerIdentifier::selectController ;-Begin new Execution context");
#endif

    Controller * controller = getRegisteredController(controllerName);
    if(controller){
        Controller *worker = controller->createNew();//will be deleted once the wrork is done

        connect(worker,&Controller::closeClientSocket,this,&ControllerIdentifier::closeClientSocket);

        connect(&worker->MESAESignalDispatcher_,
                &MESAESignalDispatcher::MESAEEnabledChanged
                ,this,&ControllerIdentifier::onMESAEEnabledChanged);

        connect(&worker->MESAESignalDispatcher_,
                &MESAESignalDispatcher::MESAE_WaitForInput
                ,this,&ControllerIdentifier::onMESAE_WaitForInput);

        Controller::performAction(worker,data);
    }else{
        print("ControllerIdentifier::selectController ;-ERROR Invalid Controller name ;- "+controllerName);
    }

}

/**
 * @brief ControllerIdentifier::printControllerStatus
 * @param controller
 */
void ControllerIdentifier::printControllerStatus(Controller *controller)
{
    if(controller->enabled()){
        print(controller->name()+":  Enabled");
    }else{
        print(controller->name()+":  Disabled");
    }

}

/**
 * @brief ControllerIdentifier::printControllersStatus
 */
void ControllerIdentifier::printControllersStatus()
{
    //print controller status
    QObjectList controllerIdentifierObjects = children();
    foreach (QObject *obj, controllerIdentifierObjects) {
        if(obj->inherits("Controller")){
            Controller *appController  = qobject_cast<Controller *>(obj);
            if(appController){
                printControllerStatus(appController);
            }
        }
    }

}

/**
 * @brief ControllerIdentifier::onDataReceivedFromServer
 * @param data
 * @param mgbSessionId
 */
void ControllerIdentifier::onDataReceivedFromDataProcessor(QByteArray data, QString mgbSessionId)
{
    identify(crypto_->decryptToString(QString(data)),mgbSessionId);
}

void ControllerIdentifier::onMESAEEnabledChanged(Controller *controller, bool enabled)
{
    if(controller){
        if(enabled){
            MESAE_Coontrollers_.add(controller);
        }else{
            MESAE_Coontrollers_.remove(controller);
        }
    }
}


void ControllerIdentifier::onMESAE_WaitForInput(Controller *controller, int msecs){

    if(controller){

        controller->MESAE_hasActiveTimer_ = true;
        //must be here
        unsigned long  userId = controller->user().id();
        const QString& mgbSessionId = controller->user().getThisClientMgbSessionId();
        const QString& action = controller->currentExecutingAction_;
        const QString& controllerName = controller->name();
        const QString& MESAE_tag = controller->MESAE_tag_;

        QTimer::singleShot(msecs,[msecs,this,userId,action,controllerName,mgbSessionId,MESAE_tag](){// store the value in a closure
            Controller *MESAE_Controller = MESAE_Coontrollers_.getControllerWith(controllerName,action,mgbSessionId,userId,MESAE_tag);
            if(MESAE_Controller){
                if(MESAE_Controller->MESAE_isEnabled()){

#if PRINT_MESAE_DEBUG_MESSAGES
                    qDebug()<< "TIME OUT..........";
#endif
                    MESAE_Controller->onMESAE_timeout(msecs);
                }
            }
        });
    }
}

/**
 * @brief ControllerIdentifier::print
 * @param msg
 */
void ControllerIdentifier::print(QString msg)
{
    qDebug() << msg <<"\n";
}



