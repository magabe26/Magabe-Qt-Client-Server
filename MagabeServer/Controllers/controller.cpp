/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "controller.h"
#include "debug.h"

#include <QThread>
#include <QThreadPool>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "Helpers/datawritter.h"
#include "Server/connectedusers.h"
#include <QDebug>
#include <QHash>
#include <QVariant>
#include <QMetaMethod>
#include <QMap>
#include <QHostAddress>
#include <QFileInfo>
#include "registeredobjects.h"
#include "Helpers/fileutils.h"

/**   --------------------MSignalsManager---------------------------
 * @brief The MSignalsManager class
 */

class MSignalsManager
{

public:
    MSignalsManager(const MSignalsManager& rhs) = delete;
    MSignalsManager& operator =(const MSignalsManager& rhs) = delete;
    static MSignalsManager *instance(){
        // no need to delete it, it will be deleted by Os, when app exit,coz it is static
        static MSignalsManager *obj;
        if(!obj){
            try{
                obj = new MSignalsManager;
            }catch(std::bad_alloc e){
                qDebug() << "MSignalsManager:: " << e.what();
                return instance();
            }catch(...){
                qDebug() << "MSignalsManager:: memory allocation error";
                return instance();
            }
        }
        return obj;
    }

    struct PairedSlotObject{
        QString  magabeMethodSignature;
        QString classSpecificMagabeMethodSignature;
        QObject* slotClassObj;
        QMetaMethod slot;
        QMetaMethod signalToConnecedTo;

        //JUST FOT QLIST::CONTAIN TO WORK
        bool operator ==(MSignalsManager::PairedSlotObject rhs){
            //uee only classSpecificMagabeMethodSignature , so that i can query object with same classSpecificMagabeMethodSignature on QLIst
            return (this->classSpecificMagabeMethodSignature == rhs.classSpecificMagabeMethodSignature);
        }

        bool operator !=(MSignalsManager::PairedSlotObject rhs){
            //uee only classSpecificMagabeMethodSignature , so that i can query object with same classSpecificMagabeMethodSignature on QLIst
            return (this->classSpecificMagabeMethodSignature != rhs.classSpecificMagabeMethodSignature);
        }
    };

    struct UnPairedSlotObject{
        QString  magabeMethodSignature;
        QString classSpecificMagabeMethodSignature;
        QObject* slotClassObj;
        QMetaMethod slot;
        QString signalClassName;
        QString signalName;

        //JUST FOT QLIST::CONTAIN TO WORK
        bool operator ==(MSignalsManager::PairedSlotObject rhs){
            //uee only classSpecificMagabeMethodSignature , so that i can query object with same classSpecificMagabeMethodSignature on QLIst
            return (this->classSpecificMagabeMethodSignature == rhs.classSpecificMagabeMethodSignature);
        }

        bool operator !=(MSignalsManager::PairedSlotObject rhs){
            //uee only classSpecificMagabeMethodSignature , so that i can query object with same classSpecificMagabeMethodSignature on QLIst
            return (this->classSpecificMagabeMethodSignature != rhs.classSpecificMagabeMethodSignature);
        }
    };

    /** --------------UnConnectedSignal---------------
     * must set SignalToConnectTo to make it valid

     * @brief The UnConnectedSignal class
     */
    class UnConnectedSignal{
    public:
        enum class Type{
            UnknownSignal = 0,
            SenderSignal = 13,
            ReceiverSignal = 23
        };
        UnConnectedSignal():type_(Type::UnknownSignal){}
        UnConnectedSignal(const QString& className,const QString& signalName
                          ,UnConnectedSignal::Type type){
            className_ = className.trimmed();
            signalName_ = signalName.trimmed();
            type_ = type;
        }

        bool isValid()const{
            return ((!className_.isEmpty())
                    && (!signalName_.isEmpty())
                    && ((type_ == Type::SenderSignal) || (type_ == Type::ReceiverSignal)));
        }

        bool hasSignalToConnectTo(){
            return getSignalToConnectTo().isValid();
        }

        Type type()const{
            return type_;
        }

        bool operator ==(const UnConnectedSignal& rhs){
            return ((this->className_ == rhs.className_)
                    && (this->signalName_ == rhs.signalName_)
                    && (this->type_ == rhs.type_));
        }

        bool operator !=(const UnConnectedSignal& rhs){
            return ((this->className_ != rhs.className_)
                    || (this->signalName_ != rhs.signalName_)
                    || (this->type_ != rhs.type_));
        }

        UnConnectedSignal getSignalToConnectTo(){
            if(signalToConnectToSignature_.isEmpty()){
                return UnConnectedSignal("","",Type::UnknownSignal);//return a invalid one
            }else{
                QStringList list = signalToConnectToSignature_.split('.',QString::KeepEmptyParts);
                if(list.count() == 3){
                    return UnConnectedSignal(list.at(0),list.at(1),(Type)list.at(2).toInt());
                }else{
                    return UnConnectedSignal("","",Type::UnknownSignal);//return a invalid one
                }
            }
        }

        bool setSignalToConnectTo(const UnConnectedSignal& signalToConnectTo){
            if(signalToConnectTo.isValid()){
                signalToConnectToSignature_ = QString("%1.%2.%3")
                        .arg(signalToConnectTo.className_)
                        .arg(signalToConnectTo.signalName_)
                        .arg((int)signalToConnectTo.type_);
                return true;
            }else{
                return false;
            }

        }

        QString getClassName() const
        {
            return className_;
        }

        void setClassName(const QString &className)
        {
            className_ = className;
        }

        QString getSignalName() const
        {
            return signalName_;
        }

        void setSignalName(const QString &signalName)
        {
            signalName_ = signalName;
        }

        void setType(const Type &type)
        {
            type_ = type;
        }

    private:
        QString className_;
        QString signalName_;
        Type type_;
        //my werired hndle
        QString signalToConnectToSignature_;
    };/* UnConnectedSignal */


    /* Magabe hack,optimizzed for customized search*/
    class RegisteredSignal{
    public:
        enum SearchBy{
            ClassName,
            SignalMethodName,
            BothClassNameAndSignalMethodName,
            SignalMethod,
            SignalObject

        };

        RegisteredSignal():searchBy_(SearchBy::SignalMethod)
          ,signalObject_(nullptr)
          ,registeredAt_(QDateTime::currentMSecsSinceEpoch())
        {}
        RegisteredSignal(const QMetaMethod& method):searchBy_(SearchBy::SignalMethod)
          ,signalObject_(nullptr)
          ,registeredAt_(QDateTime::currentMSecsSinceEpoch())
        {
            this->method_ = method;
            this->className_ = method.enclosingMetaObject()->className();
            this->methodName_ = method.name();
        }

        bool operator ==(const RegisteredSignal& rhs){
            switch (rhs.searchBy_) {
            case SearchBy::ClassName:
                return (this->className_ == rhs.className_);
            case SearchBy::SignalMethodName:
                return (this->methodName_ == rhs.methodName_);
            case SearchBy::BothClassNameAndSignalMethodName:
                return ((this->className_ == rhs.className_)
                        && (this->methodName_ == rhs.methodName_));
            case SearchBy::SignalMethod:
                return (this->method_ == rhs.method_);
            case SearchBy::SignalObject:
                return (signalObject_ == rhs.signalObject_);
            default:
                return (this->method_ == rhs.method_);
            }
        }

        bool operator !=(const RegisteredSignal& rhs){
            switch (rhs.searchBy_) {
            case SearchBy::ClassName:
                return (this->className_ != rhs.className_);
            case SearchBy::SignalMethodName:
                return (this->methodName_ != rhs.methodName_);
            case SearchBy::BothClassNameAndSignalMethodName:
                return ((this->className_ != rhs.className_)
                        && (this->methodName_ != rhs.methodName_));
            case SearchBy::SignalMethod:
                return (this->method_ != rhs.method_);
            case SearchBy::SignalObject:
                return (signalObject_ != rhs.signalObject_);
            default:
                return (this->method_ != rhs.method_);
            }
        }


        bool operator < (const RegisteredSignal& rhs){
            return (this->registeredAt_ < rhs.registeredAt_);
        }

        bool operator > (const RegisteredSignal& rhs){
            return (this->registeredAt_ > rhs.registeredAt_);
        }

        RegisteredSignal& setClassName(const QString &className)
        {
            className_ = className;
            return *this;
        }

        RegisteredSignal& setSignalName(const QString &signalMethodName)
        {
            methodName_ = signalMethodName;
            return *this;
        }

        QMetaMethod signalMethod() const
        {
            return method_;
        }

        RegisteredSignal& setSearchBy(const SearchBy &searchBy)
        {
            searchBy_ = searchBy;
            return *this;
        }


        RegisteredSignal& setSignalMethod(const QMetaMethod &signalMethod)
        {
            method_ = signalMethod;
            return *this;
        }


        const QObject *signalObject() const
        {
            return signalObject_;
        }

        void setSignalObject(const QObject *signalObject)
        {
            signalObject_ = const_cast<QObject *>(signalObject);
        }


    private:
        QString className_;
        QString methodName_;
        SearchBy searchBy_;
        QMetaMethod method_;
        QObject* signalObject_;
        qint64 registeredAt_;
    };/*-----RegisteredSignal-----*/






    void registerObject(const QObject *object){
        if(!object){
            return;
        }

        const int classMethodCount = object->metaObject()->methodCount();
        const QByteArray className(object->metaObject()->className());
        QStringList classRegisteredSignals;

        for(int methodIndex = 0; methodIndex < classMethodCount; methodIndex++){
            QMetaMethod method = object->metaObject()->method(methodIndex);

            const QMetaObject *methodEnclosingMetaObject = method.enclosingMetaObject();
            if((method.methodType() == QMetaMethod::MethodType::Signal)
                    && (method.access() == QMetaMethod::Access::Public)
                    && (className == methodEnclosingMetaObject->className())){

                const char *signalName =  method.name();

                if(QMetaType::Void  != method.returnType()){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning() << "MSignalsManager::registerObject ERROR ;- Failed to connect to "
                               << className+"::"+signalName+ " SIGNAL"
                               << ",The SIGNAL returnType must be void";
#endif
                    continue;
                }

                classRegisteredSignals.append(signalName);

                //add signal
                RegisteredSignal signalToRegister(method);
                signalToRegister.setSignalObject(object);//set the objct

                registeredSignals_.append(signalToRegister);//MUST APPEND

#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qWarning() << "MSignalsManager::registerObject"
                           << QString("Object# %1 ").arg((unsigned long long)object) << className+"::"+signalName+ " SIGNAL registered";
#endif

                //CONECTING TO PAIRED SLOTS
                bool foundSlotToConnectTo = false;
                foreach (auto pairedSlot, pairedSlotObjects_) {
                    if(pairedSlot.signalToConnecedTo == method){
                        foundSlotToConnectTo = true;
                        auto receiver = pairedSlot.slotClassObj;
                        if(receiver){
                            QMetaObject::connect(object, method.methodIndex(),receiver,pairedSlot.slot.methodIndex());
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                            qDebug() << "MSignalsManager::registerObject : SLOT Found !";
#endif
                        }
                    }
                }

                //CONNECT TO UNPAIRED
                int unPairedSlotObjectIndex = 0;
                const QString  signalMagabeMethodSignature = createMagabeMethodSignature(method.parameterTypes(),method.returnType());

                foreach (auto unPairedSlot, unpairedSlotObjects_) {

                    if( (unPairedSlot.signalName == method.name())
                            && (unPairedSlot.signalClassName == method.enclosingMetaObject()->className())
                            && (unPairedSlot.magabeMethodSignature == signalMagabeMethodSignature) ){
                        auto receiver = unPairedSlot.slotClassObj;

                        if(receiver){

                            foundSlotToConnectTo = true;

#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                            qDebug() << QString("MSignalsManager::registerObject : Object# %1 %2::%3  Connected To Object# %4 %5").arg(QString::number((unsigned long long)object)).arg(className.data()).arg(signalName)
                                        .arg(QString::number((unsigned long long)receiver)).arg(unPairedSlot.classSpecificMagabeMethodSignature);
#endif
                            if((object == receiver) && (method.methodIndex() == unPairedSlot.slot.methodIndex())){
                                //necessry
                                qDebug() << QString("MSignalsManager::registerObject : Error:: Can not connect Object# %1 %2::%3  to itself").arg(QString::number((unsigned long long)object)).arg(className.data()).arg(signalName);
                            }else{

                                QMetaObject::connect(object, method.methodIndex(),receiver,unPairedSlot.slot.methodIndex());

                                //remove from unpaired
                                unpairedSlotObjects_.removeAt(unPairedSlotObjectIndex);
                                //move to paired
                                PairedSlotObject pairedSlotObject;
                                pairedSlotObject.magabeMethodSignature = unPairedSlot.magabeMethodSignature;
                                pairedSlotObject.classSpecificMagabeMethodSignature = unPairedSlot.classSpecificMagabeMethodSignature;
                                pairedSlotObject.slotClassObj = receiver;
                                pairedSlotObject.slot = unPairedSlot.slot;
                                pairedSlotObject.signalToConnecedTo = method;
                                pairedSlotObjects_.append(pairedSlotObject);
                                //
                            }
                        }
                    }
                    unPairedSlotObjectIndex ++;
                }
                //

                if(!foundSlotToConnectTo){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qDebug() << "MSignalsManager::registerObject: "+className+"::"+signalName+ " SIGNAL"+" Did not find slot to connect to .., May be App just started";
#endif
                }

            }
        }

        //NOW UnConnectedSignals,SIGNAL TO SIGNAL CONNECTION
        if(!classRegisteredSignals.isEmpty()){
            foreach (auto name, classRegisteredSignals) {
                connectUnConnectedSignals(className,name);
            }
        }

    }

    /**
     * @brief unRegisterObject
     * @param object
     */
    void unRegisterObject(const QObject *object){
        if(!object){
            return;
        }

        //start removing
        RegisteredSignal signalToFind;
        signalToFind.setSignalObject(object);
        signalToFind.setSearchBy(RegisteredSignal::SearchBy::SignalObject);

        for(int index = registeredSignals_.indexOf(signalToFind);
            index != -1 ; index = registeredSignals_.indexOf(signalToFind)){//i do not increment ,bcoz i am using takeAt

            RegisteredSignal signalFound = registeredSignals_.takeAt(index);//remove and return
            const QObject* obj = signalFound.signalObject();
            if(obj){
                auto signalMethod = signalFound.signalMethod();
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qDebug() << "MSignalsManager::unRegisterObject "
                         << QString("QObject#%1:%2::%3 Signal ; Object removed").arg((unsigned long)obj).arg(signalMethod.enclosingMetaObject()->className()).arg(signalMethod.name().data());
#else
                Q_UNUSED(signalMethod)

#endif

            }else{
                qWarning() << "MSignalsManager ::unRegisterObject Error a nullptr signalObject found";
            }

        }
    }

    bool isSignalRegistered(const QString&  className,const QString&  signalName)
    {
        return classHasSignal(className,signalName);
    }

    /**
     * @brief isSignalRegistered
     * @param signalName
     * @return
     */
    bool isSignalRegistered(const QString&  signalName)
    {     RegisteredSignal signalToFind;
          signalToFind.setSignalName(signalName)
                  .setSearchBy(RegisteredSignal::SearchBy::SignalMethodName);
            return registeredSignals_.contains(signalToFind);
    }

    /**
     * @brief emitSignal
     * @param senderClassName
     * @param signalName
     * @param val0
     * @param val1
     * @param val2
     * @param val3
     * @param val4
     * @param val5
     * @param val6
     * @param val7
     * @param val8
     * @param val9
     * @return
     */
    bool emitSignal(const QString&  senderClassName,const QString&  signalName,
                    QGenericArgument val0 = QGenericArgument(Q_NULLPTR),
                    QGenericArgument val1 = QGenericArgument(),
                    QGenericArgument val2 = QGenericArgument(),
                    QGenericArgument val3 = QGenericArgument(),
                    QGenericArgument val4 = QGenericArgument(),
                    QGenericArgument val5 = QGenericArgument(),
                    QGenericArgument val6 = QGenericArgument(),
                    QGenericArgument val7 = QGenericArgument(),
                    QGenericArgument val8 = QGenericArgument(),
                    QGenericArgument val9 = QGenericArgument()){

        auto method = getMethod(senderClassName,signalName);
        if(!method.isValid()){
            //nesessary
            qWarning() <<  QString("MSignalsManager::emitSignal , Error (%1::%2 signal) is not registered")
                           .arg(senderClassName)
                           .arg(signalName);

            return false;
        }

        QObject* sender = const_cast<QObject*>(getMostRecentRegisteredSignal(method).signalObject());

        if(sender){

            const bool invocked = method.invoke(sender,val0,val1,val2
                                                ,val3,val4,val5,val6,val7
                                                ,val8,val9 );
            if(invocked){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qDebug() <<"MSignalsManager::emitSignal:: " << senderClassName +"::"+ signalName + " signal emited !";
#endif
            }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qDebug() <<  QString("MSignalsManager::emitSignal | UnknownError Failed to emit (%1::%2 signal) ")
                             .arg(senderClassName)
                             .arg(signalName);
#endif
            }
            return  invocked;
        }else{
            //nesessary
            qWarning() <<  QString("MSignalsManager::emitSignal , Unexpected Error  unable to find a valid object for (%1::%2 signal)")
                           .arg(senderClassName)
                           .arg(signalName);
            return false;
        }

    }


    bool connectSignals(const QString &senderClassName, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName)
    {
        if(receiver){
            return connectSignals(senderClassName,senderSignalName,receiver->metaObject()->className(), receiverSignalName);
        }else{
            return false;
        }
    }


    bool connectSignals(const QObject *sender, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName)
    {
        if((receiver) && (sender)){
            return connectSignals(sender->metaObject()->className(),senderSignalName,receiver->metaObject()->className(), receiverSignalName);
        }else{
            return false;
        }
    }

    /////////////////
    /**
     * @brief connectSignals
     * @param senderClassName
     * @param senderSignalName
     * @param receiverClassName
     * @param receiverSignalName
     * @return
     */
    bool connectSignals(const QString&  senderClassName,const QString&  senderSignalName,
                        const QString&  receiverClassName,const QString&  receiverSignalName){

        if((senderClassName.isEmpty()) || (senderSignalName.isEmpty())
                || (receiverClassName.isEmpty()) || (receiverSignalName.isEmpty())){
            return false;
        }


        if((senderClassName == receiverClassName) && (senderSignalName == receiverSignalName)){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
            qDebug() <<"MSignalsManager::connectSignals :: Error ; Can not connect to the same signal!" << senderClassName +"::"+ senderSignalName;
#endif
            return false;
        }

        bool senderSignalFound = isSignalRegistered(senderClassName,senderSignalName);
        bool receiverSignalFound = isSignalRegistered(receiverClassName,receiverSignalName);
        bool signalsConnected = false;

        if(senderSignalFound && receiverSignalFound){
            QMetaMethod senderSignal = getMethod(senderClassName,senderSignalName);
            QMetaMethod receiverSignal = getMethod(receiverClassName,receiverSignalName);


            if(senderSignal.isValid() && receiverSignal.isValid()){

                const QObject*  sender  = getMostRecentRegisteredSignal(senderSignal).signalObject();
                const QObject*  receiver = getMostRecentRegisteredSignal(receiverSignal).signalObject();
                //CONNECT ONLY MOST RECENT REGISTERED SIGNALS,this makes possible to connect
                //controller signals that have short life spn and avoid crashes
                if((sender) && (receiver)){

                    const bool isValidReceiver = ( (receiver->metaObject()->className() == receiverClassName));
                    if(isValidReceiver){

                        QMetaObject::connect(sender, senderSignal.methodIndex(),receiver,receiverSignal.methodIndex());

#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                        qDebug() << QString("Object# %1 %2 SIGNAL Connected To Object# %%3 %4 SIGNAL")
                                    .arg(QString::number((unsigned long long)sender))
                                    .arg(createClassSpecificMagabeMethodSignature(senderSignal))
                                    .arg(QString::number((unsigned long long)receiver))
                                    .arg(createClassSpecificMagabeMethodSignature(receiverSignal));
#endif

                        signalsConnected = true;

                    }else{//class names dont match
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                        qWarning()  <<  "MSignalsManager connectSignals2 :: Unexpected ERROR occured";
#endif
                    }

                }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning()  <<  "MSignalsManager connectSignals2 :: ERROR ,Null signal object found";
#endif

                }


            }

        }

        if(!signalsConnected){
            UnConnectedSignal unConnectedSender(senderClassName,senderSignalName,UnConnectedSignal::Type::SenderSignal);
            UnConnectedSignal unConnectedReceiver(receiverClassName,receiverSignalName,UnConnectedSignal::Type::ReceiverSignal);

            unConnectedSender.setSignalToConnectTo(unConnectedReceiver);
            addUnConnectedSignal(unConnectedSender);

            signalsConnected = true;
        }

        return signalsConnected;

    }/*connectSignal*/

    RegisteredSignal getMostRecentRegisteredSignal(QMetaMethod signalMethod){
        RegisteredSignal mostRecentRegisteredSignal;

        if(!signalMethod.isValid()){
            return mostRecentRegisteredSignal;
        }

        RegisteredSignal signalToFind(signalMethod);
        signalToFind.setSearchBy(RegisteredSignal::SearchBy::SignalMethod);

        bool set = false;
        for(int index = registeredSignals_.indexOf(signalToFind);
            index != -1 ; index = registeredSignals_.indexOf(signalToFind,(index+1))){
            auto registeredSignal = registeredSignals_.at(index);;
            if(!set){
                mostRecentRegisteredSignal = registeredSignal;
                set = true;
            }else{
                if(registeredSignal > mostRecentRegisteredSignal){
                    mostRecentRegisteredSignal = registeredSignal;
                }
            }
        }

        return mostRecentRegisteredSignal;
    }

    bool connectSlot(const QObject *sender, const QString &signalName, const QObject *receiver, const QString &slotName)
    {
        if((sender) && (receiver)){
            return connectSlot(sender->metaObject()->className(),signalName,receiver,slotName);
        }else{
            return false;
        }
    }

    bool connectSlot(const QString&  senderClassName,const QString&  signalName,
                     const QObject *receiver,const QString&  slotName){

        if((senderClassName.isEmpty()) || (signalName.isEmpty())
                || (!receiver) || (slotName.isEmpty())){
            return false;
        }

        QMetaMethod signalMethod = getMethod(senderClassName,signalName);
        const bool signalFound = signalMethod.isValid();

        const int classMethodCount = receiver->metaObject()->methodCount();
        const QByteArray slotClassName(receiver->metaObject()->className());

        PairedSlotObject pairedSlotObject;
        UnPairedSlotObject unPairedSlotObject;
        bool slotFound = false;

        for(int slotMethodIndex = 0; slotMethodIndex < classMethodCount; slotMethodIndex++){
            QMetaMethod slot = receiver->metaObject()->method(slotMethodIndex);
            const QMetaObject *methodEnclosingMetaObject = slot.enclosingMetaObject();
            if((slot.methodType() == QMetaMethod::MethodType::Slot)
                    && (slotClassName == methodEnclosingMetaObject->className())
                    && (slot.name() == slotName)){

                slotFound = true;

                const QString  slotMagabeMethodSignature = createMagabeMethodSignature(slot);
                const QString  classSpecificMagabeMethodSignature = createClassSpecificMagabeMethodSignature(slot);

                if(signalFound){
                    pairedSlotObject.magabeMethodSignature = slotMagabeMethodSignature;
                    pairedSlotObject.classSpecificMagabeMethodSignature = classSpecificMagabeMethodSignature;
                    pairedSlotObject.slotClassObj = const_cast<QObject *>(receiver);
                    pairedSlotObject.slot = slot;
                    pairedSlotObject.signalToConnecedTo = signalMethod;
                    //add
                    pairedSlotObjects_.append(pairedSlotObject);
                }else{
                    unPairedSlotObject.magabeMethodSignature = slotMagabeMethodSignature;
                    unPairedSlotObject.classSpecificMagabeMethodSignature = classSpecificMagabeMethodSignature;
                    unPairedSlotObject.slotClassObj = const_cast<QObject *>(receiver);
                    unPairedSlotObject.slot = slot;
                    unPairedSlotObject.signalClassName = senderClassName;
                    unPairedSlotObject.signalName = signalName;
                    //add
                    unpairedSlotObjects_.append(unPairedSlotObject);
                }

                break;
            }
        }

        if(slotFound && signalFound){
            //connect only recently registered object of class of senderClassName
            QObject* sender = const_cast<QObject*>(getMostRecentRegisteredSignal(signalMethod).signalObject());
            if(!sender){
                //necessary, do not surround with #if
                qWarning() <<"MSignalsManager::connectSlot | ERROR , can not connect to slot , " << slotClassName +"::"+ slotName << " slot not found";
                return false;
            }

            if((sender == receiver)
                    && (signalMethod.methodIndex() == pairedSlotObject.slot.methodIndex())){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qDebug() << "MSignalsManager::connectSlot | ERROR "
                         << QString("Object#%1:%2::%3 SIGNAL can not connect to  itself").arg((unsigned long)sender).arg(signalMethod.enclosingMetaObject()->className()).arg(signalName);
#endif
                return false;
            }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                qDebug() << "MSignalsManager::connectSlot | "
                         << QString("Object#%1:%2::%3 SIGNAL Connected To Object#%4:%5::%6 SLOT").arg((unsigned long)sender).arg(signalMethod.enclosingMetaObject()->className()).arg(signalName)
                            .arg((unsigned long)receiver).arg(slotClassName.data()).arg(slotName);
#endif

                QMetaObject::connect(sender, signalMethod.methodIndex(),receiver,pairedSlotObject.slot.methodIndex());
                return true;
            }

        }else{
            if(slotFound){
                return true; //becoz we  gonna pair / connect it later
            }else{
                //necessary, do not surround with #if
                qWarning() <<"connectSlot::Error, can not connect to slot , " << slotClassName +"::"+ slotName << " slot not found";
                return false;
            }
        }

    }

    //return class names that have the signalName,May return an empty list
    //but that does not means U cant connect a slot to the signal
    //the slot we be connected later, when the signal is registred
    QStringList getSignalClassNames(const QString&  signalName){
        QStringList list;
        RegisteredSignal signalToFind;
        signalToFind.setSignalName(signalName)
                .setSearchBy(RegisteredSignal::SearchBy::SignalMethodName);
        for(int index = registeredSignals_.indexOf(signalToFind);
            index != -1 ; index = registeredSignals_.indexOf(signalToFind,(index+1))){
            list.append(registeredSignals_.at(index).signalMethod().enclosingMetaObject()->className());
        }
        return list;
    }

protected:
    MSignalsManager(){}

private:

    QList<RegisteredSignal> registeredSignals_;

    QList<PairedSlotObject> pairedSlotObjects_;
    QList<UnPairedSlotObject> unpairedSlotObjects_;

    QList<UnConnectedSignal> unConnectedSignals_;

    //return false if  UnConnectedSignal is invalid
    bool addUnConnectedSignal(UnConnectedSignal& notYetRegisredSignal){
        if(notYetRegisredSignal.isValid()){
            unConnectedSignals_.append(notYetRegisredSignal);
            return true;
        }else{
            return false;
        }
    }

    bool hasUnConnectedSignal(const QString&  className,const QString&  signalName){
        if(unConnectedSignals_.contains(UnConnectedSignal(className,signalName
                                                          ,UnConnectedSignal::Type::SenderSignal))){
            return true;
        }else{
            return unConnectedSignals_.contains(UnConnectedSignal(className,signalName
                                                                  ,UnConnectedSignal::Type::ReceiverSignal));
        }

    }

    void connectUnConnectedSignals(const QString&  className,const QString&  signalName){
        if(hasUnConnectedSignal(className,signalName)){
            QList<UnConnectedSignal> senderSignals;
            QList<UnConnectedSignal> receiverSignals;

            //FINDING SENDERS
            auto senderSignal = UnConnectedSignal(className,signalName
                                                  ,UnConnectedSignal::Type::SenderSignal);
            for(int senderSignalIndex = unConnectedSignals_.indexOf(senderSignal,0);
                senderSignalIndex != -1 ;
                senderSignalIndex = unConnectedSignals_.indexOf(senderSignal,(senderSignalIndex + 1))){

                auto unConnectedSignal = unConnectedSignals_.value(senderSignalIndex);
                if(unConnectedSignal.isValid()){
                    senderSignals.append(unConnectedSignal);
                }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning() << "connectUnConnectedSignals ::Alert Implementation error, found invalid  UnConnectedSignal of type == Type::SenderSignal In unConnectedSignals_";
#endif
                }
                //NOTE I DO NOT REMOVE THE ITEM IN HERE,BCOZ I AM INCREMENTING INDEXIES
            }

            //now remove all receiverSignals
            unConnectedSignals_.removeAll(senderSignal);

            //FINDING RECEIVERS
            auto receiverSignal = UnConnectedSignal(className,signalName
                                                    ,UnConnectedSignal::Type::ReceiverSignal);
            for(int receiverSignalIndex = unConnectedSignals_.indexOf(receiverSignal,0);
                receiverSignalIndex != -1 ;
                receiverSignalIndex = unConnectedSignals_.indexOf(receiverSignal,(receiverSignalIndex + 1))){

                auto unConnectedSignal = unConnectedSignals_.value(receiverSignalIndex);
                if(unConnectedSignal.isValid()){
                    receiverSignals.append(unConnectedSignal);
                }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning() << "connectUnConnectedSignals ::Alert Implementation error, found invalid  UnConnectedSignal of type == Type::ReceiverSignal In unConnectedSignals_";
#endif

                }
                //NOTE I DO NOT REMOVE THE ITEM IN HERE,BCOZ I AM INCREMENTING INDEXIES
            }

            //now remove all receiverSignals
            unConnectedSignals_.removeAll(receiverSignal);

            foreach (auto senderSgnl, senderSignals) { //more likely this will be executed, bcoz i am only adding the one with Type::SenderSignal in connectSignals
                auto receiverSgnl  = senderSgnl.getSignalToConnectTo();
                if((senderSgnl.type() == UnConnectedSignal::Type::SenderSignal)
                        &&  receiverSgnl.isValid()
                        && (receiverSgnl.type() == UnConnectedSignal::Type::ReceiverSignal)){
                    connectSignals(senderSgnl.getClassName(),senderSgnl.getSignalName()
                                   ,receiverSgnl.getClassName(),receiverSgnl.getSignalName());
                }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning() << "connectUnConnectedSignals ::Alert Implementation error, illegal state TAG# 1";
#endif

                }
            }

            foreach (auto receiverSgnl , receiverSignals) { //less likely this will be executed, bcoz i am only adding the one with Type::SenderSignal in connectSignals
                auto senderSgnl  = receiverSgnl.getSignalToConnectTo();
                if((senderSgnl.type() == UnConnectedSignal::Type::SenderSignal)
                        && senderSgnl .isValid()
                        && (receiverSgnl.type() == UnConnectedSignal::Type::ReceiverSignal)){
                    connectSignals(senderSgnl.getClassName(),senderSgnl.getSignalName()
                                   ,receiverSgnl.getClassName(),receiverSgnl.getSignalName());
                }else{
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
                    qWarning() << "connectUnConnectedSignals ::Alert Implementation error, illegal state TAG# 1";
#endif

                }
            }
        }
    }

    //returns MagabeMethodSignature eg (2:QString,int)void
    QString createMagabeMethodSignature(QList<QByteArray>parameterTypes, int returnType){
        QString signature;
        const int parameterCount = parameterTypes.count();
        signature.append("(")
                .append(QString::number(parameterCount))
                .append(":");

        int i = 0;
        foreach (auto pTypes, parameterTypes) {
            signature.append(pTypes);
            if(i != (parameterCount - 1)){
                signature.append(",");
            }
            i++;

        }
        if(QMetaType::Void  == returnType){
            signature.append(")").append("void");
        }else{
            signature.append(")").append(QString::number(returnType));
        }
        return signature;
    }

    QString createMagabeMethodSignature(const QMetaMethod& method)
    {
        if(method.isValid()){
            return createMagabeMethodSignature(method.parameterTypes(),method.returnType());
        }else{
            return "";
        }
    }

    //returns ClassSpecificMagabeMethodSignature eg Controller::someSignal(2:QString,int)void
    QString createClassSpecificMagabeMethodSignature(const QString&  className
                                                     ,const QString&  signalName
                                                     ,const QString& magabeMethodSignature)
    {
        return className + "::" +signalName + magabeMethodSignature;
    }

    QString createClassSpecificMagabeMethodSignature(const QMetaMethod& method)
    {
        auto magabeMethodSignature = createMagabeMethodSignature(method);
        if(!magabeMethodSignature.isEmpty()){
            return createClassSpecificMagabeMethodSignature(method.enclosingMetaObject()->className(),method.name(),magabeMethodSignature);
        }else{
            return "";
        }
    }


    bool classHasSignal(const QString&  className,const QString&  signalName)
    {
        auto list = getSignalClassNames(signalName);
        if(list.length() == 0){
            return false;
        }else{
            return list.contains(className);
        }
    }

    QMetaMethod getMethod(const QString &className, const QString &signalName){

        RegisteredSignal signalToFind;
        signalToFind.setClassName(className)
                .setSignalName(signalName)
                .setSearchBy(RegisteredSignal::SearchBy::BothClassNameAndSignalMethodName);
        const int count = registeredSignals_.count(signalToFind);
        if(count == 0){
            return QMetaMethod();
        }
        QList<QMetaMethod> list;
        for(int index = registeredSignals_.indexOf(signalToFind);
            index != -1 ; index = registeredSignals_.indexOf(signalToFind,(index+1))){
            list.append(registeredSignals_.value(index).signalMethod());
        }
        if(count > 1){
#if PRINT_MSignalsManagerClass_DEBUG_MESSAGES
            qWarning() << QString("MSignalsManager::getMethod Alert!! , Found %1 Instances of Class %2 that registered %3 signal, Selecting the most recent registered Object | signal method ... ").arg(count).arg(className).arg(signalName);
#endif
        }
        return list.last(); //returm most recent
    }

}; /* MSignalsManager */





/**                 ----------Response::FileSender---------------
 *
 *
 * @brief Response::FileSender::lastError
 * @return
 */

Response::FileSender::FileSender(): crypto_(Q_UINT64_C(0xbd4f9fefd46f3a27)){}

Response::FileSender::~FileSender(){
    closeOpenFiles();
}

Response::FileSender::FileSenderError &Response::FileSender::lastError(){
    return fileSenderError_;
}

QString Response::FileSender::compressAndEncrypt(const QByteArray &fileByteArray)
{//they only way to make it work
    QByteArray compressedData(qCompress(fileByteArray));
    QString BinaryData(crypto_.encryptToString(compressedData));
    return  BinaryData;
}

bool Response::FileSender::sendFile(const QString &path,quint64 startPosition)

{
    bool opened = false;
    FileSenderError::ErrorType error;
    OpenedFile openedFile =  openFile(path,&opened,&error);

    if(opened && (!openedFile.isNull())){

        currentFile_.path_ = openedFile.path();
        currentFile_.baseName_ = openedFile.baseName();
        currentFile_.fileExtension_ = openedFile.fileExtension();
        currentFile_.fileSize_ =openedFile.fileSize();

        quint64 fsize = openedFile.fileSize();
        quint64 position = startPosition;

        if(position > fsize){
            fileSenderError_.errorType_ = FileSenderError::InvalidFilePositionError;
            return false;
        }

        QFile *file = openedFile.file();

        quint64 bytesAlreadySent = 0;
        quint64 remainingBytes = fsize;

        if(position > 0 && position < fsize)
        {
            bytesAlreadySent = position;
            remainingBytes = fsize - bytesAlreadySent;

            file->seek(position);
        }

        if(!file->atEnd()) {

            //bytes to read and send
            quint64 chunkSize =((remainingBytes < transferRate_)
                                ? remainingBytes : transferRate_);

            QByteArray chunkBytes(file->read(chunkSize));
            chunkSize = chunkBytes.size();

            if((chunkBytes.isEmpty() && (chunkSize > 0))){//error
                closeFile(openedFile.path());
                fileSenderError_.errorType_ = FileSenderError::UnknownError;
                return false;
            }else{
                remainingBytes -= chunkSize;
                bytesAlreadySent += chunkSize;

                bool bytesAvailable = (remainingBytes != 0);

                response_->
                        addExtras(MFile::ExtrasNames::senderUsernameExtras,currentFile_.fileSender_)
                        .addExtras(MFile::ExtrasNames::receiverUsernameExtras,currentFile_.fileReceiver_)
                        .addExtras(MFile::ExtrasNames::chunkBytesExtras,compressAndEncrypt(chunkBytes))
                        .addExtras(MFile::ExtrasNames::chunkSizeExtras,chunkSize)
                        .addExtras(MFile::ExtrasNames::fileSizeExtras,fsize)
                        .addExtras(MFile::ExtrasNames::fileBaseNameExtras,openedFile.baseName())
                        .addExtras(MFile::ExtrasNames::fileExtensionExtras,openedFile.fileExtension())
                        .addExtras(MFile::ExtrasNames::senderFposExtras,file->pos())
                        .addExtras(MFile::ExtrasNames::receiverFposExtras,position)
                        .addExtras(MFile::ExtrasNames::fileLocationExtras,openedFile.path())
                        .addExtras(MFile::ExtrasNames::bytesStillAvailableExtras,bytesAvailable)
                        .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag_);

                //after the above intensive prosessing
                //check again if we are still connected
                if(response_->cuser_->thisClientConnectionIsActive()){
                    if(response_->queueBypassSend()){
                        position += chunkSize;

                        currentFile_.position_= position;

                        openedFile.savePosition(position);
                        if(!bytesAvailable){//all bytes sent close file
                            closeFile(openedFile.path());
                        }
                        fileSenderError_.errorType_ = FileSenderError::NoError;
                        return true;
                    }else{
                        fileSenderError_.errorType_ = FileSenderError::CausedByAResponseError;
                        return false;
                    }
                }else{
                    closeFile(openedFile.path());//close file if disconnected
                    response_->responseError_.errorType_ = ResponseError::UserDisconnectedError;
                    fileSenderError_.errorType_ = FileSenderError::CausedByAResponseError;//ie UserDisconnectedError
                    return false;
                }

            }

        }else{//is is at end the close it
            fileSenderError_.errorType_ = FileSenderError::NoError;
            closeFile(openedFile.path());
            return (remainingBytes == 0);
        }

    }else{
        fileSenderError_.errorType_ = error;
        return false;
    }

}

bool Response::FileSender::sendFile(){
    return sendFile(fpath_,position_);
}

bool Response::FileSender::sendFile(const QString &path){
    return  sendFile(path,position_);
}

void Response::FileSender::closeOpenFiles(){
    QMapIterator<QString,OpenedFile> i(openedFileMap_);
    while (i.hasNext()) {
        i.next();
        removeAndCloseOpenedFile(i.key());
    }
}


void Response::FileSender::setTransferRate(quint64 bytes){
    transferRate_ = bytes;
}

void Response::FileSender::setStartPosition(quint64 startPosition){
    position_ = startPosition;
}

bool Response::FileSender::setFilePath(const QString &path){
    QFileInfo finfo(path);

    if(finfo.exists()){
        fpath_ = path;
        return true;
    }else{
        return false;
    }
}

QString Response::FileSender::getFilePath(){
    return fpath_;
}

quint64 Response::FileSender::getPosition() const
{
    return position_;
}

Response::FileSender::OpenedFile Response::FileSender::openFile(const QString &path, bool *opened, FileSenderError::ErrorType *errorType){

    if(path.isEmpty()){
        *opened = false;
        *errorType = FileSenderError::InvalidFilePathError;;
        return  OpenedFile();
    }

    if(isFileOpen(path)){
        *opened = true;
        *errorType = FileSenderError::NoError;
        return getOpened(path);
    }

    QFileInfo finfo(path);
    quint64 fsize;
    QString fileBaseName;
    QString fileExtension;
    if(finfo.exists()){
        QFile *file = new QFile(path);
        if(file->open(QIODevice::ReadOnly)){
            qDebug() << "New file opened ,path ->" << path;
            fsize = file->size();
            fileBaseName = finfo.baseName();
            fileExtension = finfo.completeSuffix();
            OpenedFile openedFile(path,fileBaseName,fileExtension,file,fsize,0);
            addOpenedFile(path,openedFile);
            *opened = true;
            *errorType = FileSenderError::NoError;
            return openedFile;
        }else{
            file->deleteLater();
            *opened = false;
            *errorType =  FileSenderError::CantOpenFileError;
            return  OpenedFile();
        }
    }else{
        *opened = false;
        *errorType =  FileSenderError::FileDoesNotExistsError;
        return OpenedFile();
    }
}

void Response::FileSender::closeFile(const QString &path){
    removeAndCloseOpenedFile(path);
}

Response::FileSender::CurrentFile &Response::FileSender::currentFile(){
    return currentFile_;
}

void Response::FileSender::setFileTag(const QString &fileTag)
{
    fileTag_ = fileTag;
}

void Response::FileSender::addOpenedFile(const QString &path, Response::FileSender::OpenedFile &openedFile){
    if(!openedFileMap_.contains(path)){
        openedFileMap_.insert(path,openedFile);
    }
}

void Response::FileSender::removeAndCloseOpenedFile(const QString &path){
    if(isFileOpen(path)){
        OpenedFile  file = openedFileMap_.value(path);
        openedFileMap_.remove(path);
        if(auto fl = file.file()){
            if(fl->isOpen()){
                fl->close();
                qDebug() << "File closed = " << path;
            }
            fl->deleteLater();
        }
    }
}

Response::FileSender::OpenedFile Response::FileSender::getOpened(const QString &path){
    return openedFileMap_.value(path);
}

bool Response::FileSender::isFileOpen(const QString &path){
    return openedFileMap_.contains(path);
}

FileSenderError::FileSenderError()
    :errorType_(NoError)
    ,userId(0){}

FileSenderError::FileSenderError(FileSenderError::ErrorType type)
    :errorType_(type)
    ,userId(0){}

QString FileSenderError::toString(){
    QString err;
    switch (errorType_) {
    case  NoError:
        err = "NoError";
        break;
    case  FileDoesNotExistsError:
        err = "FileSenderError# FileDoesNotExistsError";
        break;
    case  CantOpenFileError:
        err = "FileSenderError# CantOpenFileError";
        break;
    case  InvalidFilePositionError:
        err = "FileSenderError# InvalidFilePositionError";
        break;
    case  EmptyFileError:
        err = "FileSenderError# EmptyFileError";
        break;
    case  UnknownError:
        err = "FileSenderError# UnknownError";
        break;
    case  CausedByAResponseError:
        err = "FileSenderError# CausedByAResponseError";
        break;
    case  InvalidFilePathError:
        err = "FileSenderError# InvalidFilePathError";
        break;
    default:
        err  = "NoError";
    }
    err.append(" | userId: " +QString::number(userId));
    return err;
}

FileSenderError::ErrorType FileSenderError::type(){
    return errorType_;
}

bool FileSenderError::operator ==(FileSenderError::ErrorType type){
    return (this->errorType_ == type);
}

bool FileSenderError::operator ==(Response::FileSender::FileSenderError error){
    return (this->errorType_ == error.errorType_);
}

bool FileSenderError::operator !=(Response::FileSender::FileSenderError error){
    return (this->errorType_ != error.errorType_);
}

bool FileSenderError::operator !=(FileSenderError::ErrorType type){
    return (this->errorType_ != type);
}


/**  ------------------ResponseError-------------------
 * @brief ResponseError::toString
 * @return
 */
QString ResponseError::toString(){
    QString err;
    switch (errorType_) {
    case  NoError:
        err = "NoError";
        break;
    case  ControllerNotSetError:
        err = "ResponseError# ControllerNotSetError";
        break;
    case  ActionNotSetError:
        err = "ResponseError# ActionNotSetError";
        break;
    case  UserDisconnectedError:
        err = "ResponseError# UserDisconnectedError";
        break;
    case  DataNotSetError:
        err = "ResponseError# DataNotSetError";
        break;
    default:
        err  = "NoError";
    }
    err.append(" | userId: " +QString::number(userId));
    return err;
}

ResponseError::ErrorType ResponseError::type(){
    return errorType_;
}

bool ResponseError::operator ==(ResponseError::ErrorType type){
    return (this->errorType_ == type);
}

bool ResponseError::operator ==(Response::ResponseError error){
    return (this->errorType_ == error.errorType_);
}



/** --------------------Response------------------------
 * @brief Response::Response
 * @param cuser
 */
Response::Response(ConnectedUser *cuser):cuser_(cuser){
    if(cuser){
        responseError_.userId = cuser->id();
        fileSender_.response_ = this;//they both share the same response object
    }
}

Response &Response::addExtras(const QString &name, int value){
    data_.addExtras(name, value);
    return *this;
}

Response &Response::addExtras(const QString &name, bool value)
{
    data_.addExtras(name,value);
    return *this;
}
Response &Response::addExtras(const QString &name, float value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name, double value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name, long value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name, unsigned long value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name, qint64 value)//long long
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name,const QByteArray& value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name,const char *value){
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name, const QJsonObject &object)
{
    data_.addExtras(name,object);
    return *this;
}

Response &Response::addExtras(const QString &name, const QJsonArray &array)
{
    data_.addExtras(name,array);
    return *this;
}

Response &Response::addExtras(const QString &name, const QStringList &list)
{
    data_.addExtras(name,list);
    return *this;
}

Response &Response::addExtras(const char *name, const QStringList &list)
{
    data_.addExtras(name,list);
    return *this;
}

Response &Response::addExtras(const QString &name, quint64 value){
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const QString &name,const QString &value)
{
    data_.addExtras(name,value);
    return *this;
}
Response &Response::removeExtras(const QString &name){
    data_.removeExtras(name);
    return *this;
}

Response &Response::removeExtras(const char *name){
    data_.removeExtras(name);
    return *this;
}

bool Response::containsExtras(const QString &name){

    return data_.containsExtras(name);
}

bool Response::containsExtras(const char *name){
    return data_.containsExtras(name);
}

int Response::getExtrasCount(){
    return data_.getExtrasCount();
}
void Response::clear(){
    data_.clear();
}

QString Response::controller(){
    return data_.controller();
}

Response &Response::setController(const QString &controller){
    data_.setController(controller);
    return *this;
}

QString Response::action(){
    return data_.action();
}

Response &Response::setAction(const QString &action){
    data_.setAction(action);
    return *this;
}

QJsonObject Response::getJsonObjectExtras(const QString &name)
{
    return data_.getJsonObjectExtras(name);
}

QJsonArray Response::getJsonArrayExtras(const QString &name)
{
    return data_.getJsonArrayExtras(name);
}

QJsonObject Response::getJsonObjectExtras(const char *name)
{
    return data_.getJsonObjectExtras(name);
}

QJsonArray Response::getJsonArrayExtras(const char *name)
{
    return data_.getJsonArrayExtras(name);
}

QVariantList Response::getVariantListExtras(const QString &name)
{
    return data_.getVariantListExtras(name);
}

Response &Response::addExtras(const char *name, const char *value){
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const char* name,const QByteArray& value){

    data_.addExtras(name, value);
    return *this;
}
Response &Response::addExtras(const char* name, const QString &value){
    data_.addExtras(name, value);
    return *this;
}

Response &Response::addExtras(const char* name, int value){
    data_.addExtras(name, value);
    return *this;

}

Response &Response::addExtras(const char* name, bool value){//call the QString version results to 3 calls, wiil this wayony 2
    data_.addExtras(name, value);
    return *this;

}

Response &Response::addExtras(const char* name, float value){
    data_.addExtras(name,value);
    return *this;

}

Response &Response::addExtras(const char* name, double value){
    data_.addExtras(name,value);
    return *this;

}

Response &Response::addExtras(const char* name, long value){
    data_.addExtras(name,value);
    return *this;

}

Response &Response::addExtras(const char *name, unsigned long value)
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const char *name, qint64 value)//long long
{
    data_.addExtras(name,value);
    return *this;
}

Response &Response::addExtras(const char* name, quint64 value){
    data_.addExtras(name,value);
    return *this;

}

Response &Response::addExtras(const char *name, const QJsonObject &object)
{
    data_.addExtras(name,object);
    return *this;
}

Response &Response::addExtras(const char *name, const QJsonArray &array)
{
    data_.addExtras(name,array);
    return *this;
}


bool Response::send(){
    responseError_.errorType_ = ResponseError::NoError;
    //must be thefirst
    if(!cuser_){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }
    if(!cuser_->thisClientConnectionIsActive()){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }

    if(data_.isEmpty()){
        responseError_.errorType_ = ResponseError::DataNotSetError;
    }

    if(data_.controller() == "NON"){
        responseError_.errorType_ = ResponseError::ControllerNotSetError;
    }

    if(data_.action() == "NON"){
        responseError_.errorType_ = ResponseError::ActionNotSetError;
    }

    if(responseError_.errorType_ == ResponseError::NoError){
        cuser_->send(data_);

        QString controller = data_.controller();
        data_.clear();
        //restore controller
        data_.setController(controller);
        return true;
    }else{
        return false;
    }
}

bool Response::send(MagabeData &data){
    responseError_.errorType_ = ResponseError::NoError;
    //must be thefirst
    if(!cuser_){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }
    if(!cuser_->thisClientConnectionIsActive()){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }

    if(data.isEmpty()){
        responseError_.errorType_ = ResponseError::DataNotSetError;
    }

    if(data.controller() == "NON"){
        responseError_.errorType_ = ResponseError::ControllerNotSetError;
    }

    if(data.action() == "NON"){
        responseError_.errorType_ = ResponseError::ActionNotSetError;
    }

    if(responseError_.errorType_ == ResponseError::NoError){
        cuser_->send(data);
        data_.clear();
        return true;
    }else{
        return false;
    }
}

Response::ResponseError &Response::lastError(){
    return responseError_;
}

Response::FileSender &Response::fileSender(){
    return fileSender_;
}

bool Response::queueBypassSend()
{
    responseError_.errorType_ = ResponseError::NoError;
    //must be thefirst
    if(!cuser_){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }
    if(!cuser_->thisClientConnectionIsActive()){
        responseError_.errorType_ = ResponseError::UserDisconnectedError;
    }

    if(data_.isEmpty()){
        responseError_.errorType_ = ResponseError::DataNotSetError;
    }

    if(data_.controller() == "NON"){
        responseError_.errorType_ = ResponseError::ControllerNotSetError;
    }

    if(data_.action() == "NON"){
        responseError_.errorType_ = ResponseError::ActionNotSetError;
    }

    if(responseError_.errorType_ == ResponseError::NoError){
        cuser_->queueBypassSend(data_);

        QString controller = data_.controller();
        data_.clear();
        //restore controller
        data_.setController(controller);
        return true;
    }else{
        return false;
    }
}





/**  -------------------UserObjectEditor------------------------
 * @brief The UserObjectEditor class
 */
class UserObjectEditor{
public:
    UserObjectEditor(User &user):model_(new UserModel),user_(user){}

    ~UserObjectEditor(){
        if(model_)
            model_->deleteLater();
    }

    /// updater
    bool updatePhoneNumber(const QString &phoneNumber)
    {
        bool updated = model_->updatePhoneNumber(phoneNumber,user_.id());
        if(updated){
            user_.phoneNumber_ = phoneNumber;
        }
        return updated;
    }

    bool updateLocation(const QString &location)
    {
        bool updated = model_->updateLocation(location,user_.id());
        if(updated){
            user_.location_ = location;
        }
        return updated;
    }

    bool updatePosition(const QString &position)
    {
        bool updated = model_->updatePosition(position,user_.id());
        if(updated){
            user_.position_ = position;
        }
        return updated;
    }

    bool updateEducation(const QString &education)
    {
        bool updated = model_->updateEducation(education,user_.id());
        if(updated){
            user_.education_ = education;
        }
        return updated;
    }

    bool updateProfilePicturePath(const QString &profilePicturePath)
    {
        bool updated = model_->updateProfilePicturePath(profilePicturePath,user_.id());
        if(updated){
            user_.profilePicturePath_ = profilePicturePath;
        }
        return updated;
    }

    bool updateIsBlackListed(bool isBlackListed)
    {
        bool updated =  model_->updateIsBlackListed(isBlackListed,user_.id());
        if(updated){
            user_.isBlackListed_ = isBlackListed;
        }
        return updated;
    }

    bool updateIsActive(bool isActive)
    {
        bool updated =  model_->updateIsBlackListed(isActive,user_.id());
        if(updated){
            user_.isActive_ = isActive;
        }
        return updated;
    }

    bool updateEmail(const QString &email)
    {
        bool updated = model_->updateEmail(email,user_.id());
        if(updated){
            user_.email_ = email;
        }
        return updated;
    }

    bool updatePassword(const QString &password)
    {
        bool updated = model_->updatePassword(password,user_.id());
        if(updated){
            user_.password_ = password;
        }
        return updated;
    }

    bool updateUsername(const QString &username)
    {
        bool updated = model_->updateUsername(username,user_.id());
        if(updated){
            user_.username_ = username;
        }
        return updated;
    }

    bool updateGender(const User::Gender &gender)
    {
        bool updated =  model_->updateGender(gender,user_.id());
        if(updated){
            user_.gender_ = gender;
        }
        return updated;
    }

    bool updateDateOfBirth(const QString &dateOfBirth)
    {
        bool updated = model_->updateDateOfBirth(dateOfBirth,user_.id());
        if(updated){
            user_.dateOfBirth_ = dateOfBirth;
        }
        return updated;
    }

    bool updateFirstName(const QString &firstName)
    {
        bool updated = model_->updateDateOfBirth(firstName,user_.id());
        if(updated){
            user_.firstName_ = firstName;
        }
        return updated;
    }

    bool updateLastName(const QString &lastName)
    {
        bool updated = model_->updateLastName(lastName,user_.id());
        if(updated){
            user_.lastName_ = lastName;
        }
        return updated;
    }

    bool updateAuthKey(const QString &authKey)
    {

        bool updated =  model_->updateAuthKey(authKey,user_.id());
        if(updated){
            user_.authKey_ = authKey;
        }
        return updated;
    }

    QString generateAuthKey()
    {
        return model_->generateAuthKey(user_.id());
    }


    ///           setters
    void setUserId(unsigned long id)
    {
        user_.id_ = id;
    }

    void setPhoneNumber(const QString &phoneNumber)
    {
        user_.phoneNumber_ = phoneNumber;
    }

    void setLocation(const QString &location)
    {
        user_.location_ = location;
    }

    void setPosition(const QString &position)
    {
        user_.position_ = position;
    }

    void setEducation(const QString &education)
    {
        user_.education_ = education;
    }

    void setProfilePicturePath(const QString &profilePicturePath)
    {
        user_.profilePicturePath_ = profilePicturePath;
    }

    void setIsBlackListed(bool isBlackListed)
    {
        user_.isBlackListed_ = isBlackListed;
    }

    void setIsActive(bool isActive)
    {
        user_.isActive_ = isActive;
    }

    void setEmail(const QString &email)
    {
        user_.email_ = email;
    }

    void setPassword(const QString &password)
    {
        user_.password_ = password;
    }

    void setUsername(const QString &username)
    {
        user_.username_ = username;
    }

    void setGender(const User::Gender &gender)
    {
        user_.gender_ = gender;
    }

    void setDateOfBirth(const QString &dateOfBirth)
    {
        user_.dateOfBirth_ = dateOfBirth;
    }

    void setFirstName(const QString &firstName)
    {
        user_.firstName_ = firstName;
    }

    void setLastName(const QString &lastName)
    {
        user_.lastName_ = lastName;
    }

    ///////////////


private:
    friend bool userSetThisClientSocket(User &user,const QSslSocket *socket);
    friend bool userSetThisClientMgbSessionId(User &user,const QString &mgbSessionId);
    friend User& getUserRef(const UserObjectEditor& editor);
    friend bool userSetThisClientController(User &user,Controller *controller);

    bool setSocket(const QSslSocket *socket){
        if(socket){
            user_.setThisClientSocket(socket);
            return true;
        }else{
            return false;
        }
    }
    bool setThisClientMgbSessionId(const QString &mgbSessionId){
        if(ConnectedUserList::instance()->userExists(mgbSessionId)){
            user_.thisClientMgbSessionId_ = mgbSessionId;
            return true;
        }else{
            return false;
        }
    }

    bool setController(Controller *controller){
        if(controller){
            user_.thisClientController_ = controller;
            return true;
        }else{
            return false;
        }
    }
    UserModel *model_;
    User &user_;
};


bool userSetThisClientSocket(User &user,const QSslSocket *socket){
    UserObjectEditor editor(user);
    return editor.setSocket(socket);
}

bool userSetThisClientMgbSessionId(User &user,const QString &mgbSessionId){
    UserObjectEditor editor(user);
    return editor.setThisClientMgbSessionId(mgbSessionId);
}

User& getUserRef(const UserObjectEditor& editor){
    return editor.user_;
}

bool userSetThisClientController(User &user,Controller *controller){
    UserObjectEditor editor(user);
    return editor.setController(controller);
}





/**  ------------------ConnectedUser-----------------------
 * @brief The ConnectedUser struct
 */
ConnectedUser::ConnectedUser(const User &user)//donot use initilizer list for non nullptr initilization,the state is not valid yet
    :appUsers_(nullptr)
{
    user_ = user;//initilize first
    editor_ = new UserObjectEditor(user_);//then pass the reference
    //finally
    response_ = new Response(this);
}

ConnectedUser::~ConnectedUser(){
    if(editor_){
        delete editor_;
        editor_ = nullptr;
    }

    if(response_){
        delete response_;
        response_ = nullptr;
    }
}

bool ConnectedUser::isRegistered(){
    return (user_.id() != 0);
}

bool ConnectedUser::closeConnection(){
    return user_.closethisClientConnection();
}

QString ConnectedUser::firstName() const
{
    return user_.firstName();
}

QString ConnectedUser::lastName() const{
    return user_.lastName();
}

QString ConnectedUser::dateOfBirth() const{
    return user_.dateOfBirth();
}

User::Gender ConnectedUser::gender() const{
    return user_.gender();
}

QString ConnectedUser::username() const{
    return user_.username();
}

QString ConnectedUser::authKey() const{
    return user_.authKey();
}

QString ConnectedUser::password() const{
    return user_.password();
}

QString ConnectedUser::email() const{
    return user_.email();
}

bool ConnectedUser::isBlackListed() const{
    return user_.isBlackListed();
}

qint64 ConnectedUser::createdAt() const{
    return user_.createdAt();
}

qint64 ConnectedUser::updatedAt() const{
    return user_.updatedAt();
}

QString ConnectedUser::profilePicturePath() const{
    return user_.profilePicturePath();
}

QString ConnectedUser::education() const{
    return user_.education();
}

QString ConnectedUser::position() const{
    return user_.position();
}

QString ConnectedUser::location() const{
    return user_.location();
}

QString ConnectedUser::phoneNumber() const{
    return user_.phoneNumber();
}

QString ConnectedUser::getThisClientMgbSessionId() const{
    return user_.getThisClientMgbSessionId();
}

unsigned long ConnectedUser::id() const{
    return user_.id();
}

bool ConnectedUser::thisClientConnectionIsActive(){
    return  user_.thisClientConnectionIsActive();
}

bool ConnectedUser::updatePhoneNumber(const QString &phoneNumber){
    return editor_->updatePhoneNumber(phoneNumber);
}

bool ConnectedUser::updateLocation(const QString &location)
{
    return editor_->updateLocation(location);
}

bool ConnectedUser::updatePosition(const QString &position)
{
    return editor_->updatePosition(position);
}

bool ConnectedUser::updateEducation(const QString &education)
{
    return editor_->updateEducation(education);
}

bool ConnectedUser::updateProfilePicturePath(const QString &profilePicturePath)
{
    return editor_->updateProfilePicturePath(profilePicturePath);
}

bool ConnectedUser::updateIsBlackListed(bool isBlackListed)
{
    return editor_->updateIsBlackListed(isBlackListed);
}

bool ConnectedUser::updateIsActive(bool isActive)
{
    return editor_->updateIsActive(isActive);
}

bool ConnectedUser::updateEmail(const QString &email)
{
    return editor_->updateEmail(email);
}

bool ConnectedUser::updatePassword(const QString &password)
{
    return editor_->updatePassword(password);
}

bool ConnectedUser::updateUsername(const QString &username)
{
    return editor_->updateUsername(username);
}

bool ConnectedUser::updateGender(const User::Gender &gender)
{
    return editor_->updateGender(gender);
}

bool ConnectedUser::updateDateOfBirth(const QString &dateOfBirth)
{
    return editor_->updateDateOfBirth(dateOfBirth);
}

bool ConnectedUser::updateFirstName(const QString &firstName)
{
    return editor_->updateFirstName(firstName);
}

bool ConnectedUser::updateLastName(const QString &lastName)
{
    return editor_->updateLastName(lastName);
}

bool ConnectedUser::updateAuthKey(const QString &authKey)
{
    return editor_->updateAuthKey(authKey);
}

QString ConnectedUser::generateAuthKey()
{
    return editor_->generateAuthKey();
}

bool ConnectedUser::isClientAuthenticated()
{

    bool retVal = false;
    QMetaObject::invokeMethod(ConnectedUserList::instance()
                              ,"isAuthenticated", Qt::DirectConnection
                              ,Q_RETURN_ARG(bool, retVal)
                              ,Q_ARG(QString, user_.getThisClientMgbSessionId()));
    return retVal;
}

void ConnectedUser::send(MagabeData &data)
{
    //all no file writes form user must go througt the same DWritter queue
    DWritter::instance()->sendData(data,user_.getThisClientMgbSessionId());
}

void ConnectedUser::queueBypassSend(MagabeData &data){
    DWritter::instance()->queueBypassSend(data,user_.getThisClientMgbSessionId());
}

bool ConnectedUser::hasDataToSend(){
    return DWritter::instance()->hasDataToSend(user_.getThisClientMgbSessionId());
}

QString ConnectedUser::getJsonUserInfo(){
    return user_.getJsonUserInfo();
}

User::ConnectionState ConnectedUser::getConnectionState()
{
    return user_.getThisClientConnectionState();
}

QString ConnectedUser::getConnectionStateHuman()
{
    return user_.getConnectionStateHuman();
}

bool ConnectedUser::logOut()
{

    bool retVal = false;
    QMetaObject::invokeMethod(ConnectedUserList::instance()
                              ,"onLogOut", Qt::DirectConnection
                              ,Q_RETURN_ARG(bool, retVal)
                              ,Q_ARG(QString, user_.getThisClientMgbSessionId()));
    return retVal;
}

bool ConnectedUser::logIn(const QString &username, const QString &password,Errors::AuthenticationError *const authenticatinError)
{
    Errors::AuthenticationError error = Errors::AuthenticationError::NoError ;
    const QString emptyAuthKey = "";
    bool authenticated =  authenticate(username,emptyAuthKey,password,&error,0/*i will take it fron user obj*/,false);
    if(!authenticated && (error == Errors::AuthenticationError::IllegalAppUsersState)){
        qWarning() << "ConnectedUser::logIn :: Critical Implementation Error , appUsers_ is null";
    }
    if(authenticatinError){
        *authenticatinError = error;
    }
    return authenticated;
}

bool ConnectedUser::authenticate(const QString &username, const QString &authKey, const QString &password
                                 ,Errors::AuthenticationError *const authenticatinError, QString *const newAuthKey,bool checkAuthKey = true)
{

    if(newAuthKey){
        *newAuthKey = "";
    }

    if(authenticatinError){
        *authenticatinError = Errors::AuthenticationError::NoError;
    }

    if(username.isEmpty()){
        if(authenticatinError){
            *authenticatinError = Errors::AuthenticationError::UserNameNotSet;
        }
        return false;
    }

    if(checkAuthKey && authKey.isEmpty()){
        if(authenticatinError){
            *authenticatinError = Errors::AuthenticationError::AuthKeyNotSet;
        }
        return false;
    }

    if(password.isEmpty()){
        if(authenticatinError){
            *authenticatinError = Errors::AuthenticationError::PasswordNotSet;
        }
        return false;
    }


    if(!appUsers_){
        if(authenticatinError){
            *authenticatinError = Errors::AuthenticationError::IllegalAppUsersState;
        }
        return false;
    }

    //prevent the user from being authenticate again and again if he logs a
    //again using diffrent device or a copy of client instance
    //by doing the same thing , during client login,
    if( !isClientAuthenticated()){

        bool registered = appUsers_->userRegistered(username);

        if(registered){

            auto user = appUsers_->getUser(username);
            if(user.id() == 0){
                qWarning() << "Implementation Error :: This registered user :" << username << " has id = 0";
                if(authenticatinError){
                    *authenticatinError = Errors::AuthenticationError::UnknownError;
                }
                return false;
            }

            if(user.isBlackListed()){
                if(authenticatinError){
                    *authenticatinError = Errors::AuthenticationError::UserIsBlacklisted;
                }
                return false;
            }

            bool hasValidCredentials = false;
            if(checkAuthKey){
                hasValidCredentials =  ( (username == user.username())
                                         && (authKey == user.authKey())
                                         && (password == user.password()) );
            }else{
                hasValidCredentials =  ( (username == user.username())
                                         && (password == user.password()) );
            }

            if(hasValidCredentials){

                const unsigned long userId = user.id();
                const QString mgbSessionId = user_.getThisClientMgbSessionId();

                bool retVal = false;
                bool invoked = QMetaObject::invokeMethod(ConnectedUserList::instance()
                                                         ,"onAuthenticationProcessSucceded", Qt::DirectConnection
                                                         ,Q_RETURN_ARG(bool, retVal)
                                                         ,Q_ARG(QString, mgbSessionId)
                                                         ,Q_ARG(unsigned long, userId));
                if(invoked && retVal){
                    //UPDATE THIS CONNECTED USER
                    //first
                    editor_->setUserId(userId);
                    //then genereate new authkey
                    auto key = generateAuthKey();//ust be here ,it depends on id
                    while (key == user.authKey()) {//we want a defferent key
                        key = generateAuthKey();
                    }

                    if(editor_->updateAuthKey(key)){//for key i use updata
                        //after update key
                        if(newAuthKey){
                            *newAuthKey = this->authKey();
                        }

                        editor_->setUsername(user.username());
                        editor_->setFirstName(user.firstName());
                        editor_->setLastName(user.lastName());
                        editor_->setPhoneNumber(user.phoneNumber());
                        editor_->setLocation(user.location());
                        editor_->setPosition(user.position());
                        editor_->setEducation(user.education());
                        editor_->setProfilePicturePath(user.profilePicturePath());
                        editor_->setIsBlackListed(user.isBlackListed());
                        editor_->setEmail(user.email());
                        editor_->setPassword(user.password());
                        editor_->setGender(user.gender());
                        editor_->setDateOfBirth(user.dateOfBirth());

                        if(authenticatinError){
                            *authenticatinError = Errors::AuthenticationError::NoError;
                        }
                        return true;
                    }else{
                        qWarning() << "Implementation Error :: database writes error ,Failed to update AuthKey ";
                        if(authenticatinError){
                            *authenticatinError = Errors::AuthenticationError::UnknownError;
                        }
                        return false;
                    }

                }else{
                    qWarning() << "Implementation Error :: Failed to invoke ConnectedUserList::onAuthenticationProcessSucceded";
                    if(authenticatinError){
                        *authenticatinError = Errors::AuthenticationError::UnknownError;
                    }
                    return false;
                }

            }else{
                if(authenticatinError){
                    *authenticatinError = Errors::AuthenticationError::InvalidCredentials;
                }
                return false;
            }

        }else{
            if(authenticatinError){
                *authenticatinError = Errors::AuthenticationError::UserNotRegistered;
            }
            return false;
        }

    }else {
        if(authenticatinError){
            *authenticatinError = Errors::AuthenticationError::UserAlreadyAuthenticated;
        }
        return false;
    }
}

QString ConnectedUser::authenticationErrorToString(Errors::AuthenticationError authenticationError){
    switch (authenticationError) {
    case Errors::AuthenticationError::NoError :return "NoError";
    case Errors::AuthenticationError::UserAlreadyAuthenticated: return "UserAlreadyAuthenticatedError";
    case Errors::AuthenticationError::IllegalAppUsersState: return "IllegalAppUsersStateError";
    case Errors::AuthenticationError::UserNotRegistered: return "UserNotRegisteredError";
    case Errors::AuthenticationError::UserIsBlacklisted: return "UserIsBlacklistedError";
    case Errors::AuthenticationError::UnknownError: return "UnknownError";
    case Errors::AuthenticationError::UserNameNotSet: return "UserNameNotSetError";
    case Errors::AuthenticationError::AuthKeyNotSet: return "AuthKeyNotSetError";
    case Errors::AuthenticationError::PasswordNotSet: return "PasswordNotSetError";
    case Errors::AuthenticationError::InvalidCredentials: return "InvalidCredentialsError";
    default: return "NoError";
    }
}

Response &ConnectedUser::response(){
    return *response_;
}

void ConnectedUser::setAppUsers(AppUsers *appUsers)
{
    if(appUsers){
        appUsers_ = appUsers;
    }
}


UserObjectEditor& getUserObjectEditor(ConnectedUser *cuser){
    return *(cuser->editor_);
}






/**  --------------------Controller-------------------
 * @brief Controller::Controller
 * @param parent
 */
Controller::Controller(QObject *parent) : QObject(parent)
  ,user_(new ConnectedUser(User()))
  ,appUsers_(new AppUsers())
  ,serverFiles_(new ServerFiles())
  ,MESAE_timeout_(false)
  ,MESAE_Enabled_(false)
  ,MESAE_InputExtras_(nullptr)
  ,MESAE_semaphore_(1)
  ,MESAE_hasActiveTimer_(false)
  ,MESAE_onRestartExecutionCallback_(0)
  ,MESAE_onValidateTagCallback_(0)

{
    setEnabled(true);
}

/**
 * @brief Controller::~Controller
 */
Controller::~Controller()
{
    if(this->user_){
        delete this->user_;
        this->user_ = nullptr;
    }

    if(this->appUsers_){
        delete this->appUsers_;
        this->appUsers_ = nullptr;
    }

    if(this->serverFiles_){
        delete this->serverFiles_;
        this->serverFiles_ = nullptr;
    }
}


/**
 * @brief Controller::sendDataToClient
 * @param data
 * @param mgbSessionId
 */



/**
 * @brief Controller::print
 * @param status
 */
void Controller::print(QString status)
{
    qDebug()<< status <<"\n";
}


/**
 * @brief Controller::name
 * @return
 */
QString Controller::name() const
{
    return name_;
}

/**
 * @brief Controller::setName
 * @param name
 */
void Controller::setName(const QString &name)
{
    name_ = name;
}


/**
 * @brief Controller::enabled
 * @return
 */
bool Controller::enabled() const
{
    return enabled_;
}

/**
 * @brief Controller::setEnabled
 * @param enabled
 */
void Controller::setEnabled(bool enabled)
{
    enabled_ = enabled;
}


/**
 * @brief Controller::getTimeSTamp
 * @return
 */
QString Controller::getTimestamp()
{
    return QString::number( QDateTime::currentDateTime().toMSecsSinceEpoch());
}


/**
 * @brief Controller::printStatus
 */
void Controller::printStatus()
{
    enabled() ? print(name()+":  Enabled") : print(name()+":  Disabled") ;

}

/**
 * @brief Controller::performAction
 * @param object
 * @param data
 */
void Controller::performAction(Controller *controller,MagabeData data){//data must be a copy
    Q_ASSERT(controller != nullptr);
    //remove the initial user , which is always invalid ie contain invalid User object
    if(controller->user_){
        delete controller->user_;
        controller->user_ = nullptr;
    }

    auto connectedUserList = ConnectedUserList::instance();

    //attarch the valid one
    QString mgbSessionId = data.getMgbSessionId();
    const QSslSocket *socket = connectedUserList->getSocket(mgbSessionId);
    unsigned long userId = connectedUserList->getUserId(mgbSessionId);
    UserModel userModel;

    controller->user_ = new ConnectedUser(userModel.getUser(userId))/*will bw deleted by controller destructor*/;

    //set socket so we can disconnect
    User& userRef = getUserRef(getUserObjectEditor(controller->user_)); //cause ConnectedUser may create a copy User object,
    //so we dont trust the one we passed in
    userSetThisClientSocket(userRef,socket);
    //setting connection details
    if(userRef.id() != 0){//not new user ie user exists in our record
        userSetThisClientMgbSessionId(userRef,mgbSessionId);
        userModel.updateIsActive(true,userId);
    }else{//id == 0 , means,may be a new user trying to register
        userSetThisClientMgbSessionId(userRef,mgbSessionId);
    }

    userSetThisClientController(userRef,controller);

    //init appUsers instance
    controller->user_->setAppUsers(controller->appUsers_);

    //connect signals to all app slots, quit a journey
    MSignalsManager::instance()->registerObject(controller);

    //registering actions
    bool registered =  QMetaObject::invokeMethod(controller,"registerActions");
    if(!registered){
        controller->printActionRegistrationError();
        controller->disconnect();
        controller->deleteLater();
        return;//We dont need to start the thread
    }

    //register controller methods
    controller->registerControllerMethods(controller);

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>();
    connect(watcher,&QFutureWatcher<void>::finished,controller,&Controller::onPerformActionFinished);
    connect(watcher,&QFutureWatcher<void>::finished,[controller](){
        //QString name = object->name();
        controller->disconnect();
        //then
        MSignalsManager::instance()->unRegisterObject(controller);
        //finally
        controller->deleteLater();
    });

    /*----------connect common signals----------*/



    /*---------------------------------------------*/
    //print authentication status of user
#if MAGABE_SERVER_PRINT_AUTHENTICATION_STATUS_IN_ControllerClass_performActionMethod
    if(controller->user_->isAuthenticated()){

        controller->print(QString("[%1] User on %2:%3 with mgbSessionId : %4 is authenticated")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort())
                          .arg(userRef.mgbSessionId()));
    }else{
        controller->print(QString("[%1] User on %2:%3 with mgbSessionId : %4 is not authenticated")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss.zzz ap"))
                          .arg(socket->peerAddress().toString())
                          .arg(socket->peerPort())
                          .arg(userRef.mgbSessionId()));
    }
#endif

    //finally running on diffrent thread
    watcher->setFuture(QtConcurrent::run(QThreadPool::globalInstance(),[controller,data](){ //QtConcurrent::run return a future
        QMetaObject::invokeMethod(controller,"triggerAction",Qt::DirectConnection,Q_ARG(MagabeData,data));
    }));
}

/**
 * @brief Controller::onPerformActionFinished
 */
void Controller::onPerformActionFinished(){
    QFutureWatcher<void> *watcher = dynamic_cast<QFutureWatcher<void> *>(sender());
    watcher->deleteLater();//use only deleteLater, to avoid memory leaks
}

void Controller::registerActions(){
    printActionRegistrationError();
}


void Controller::printActionTriggered(const QString &action){
    print(QString("---> %1: Triggered").arg(action));
}

bool Controller::isRegisteredAction(const QString &action){
    return actions_.contains(action);
}

void Controller::registerAction(const QString &action, const int enumValue){
    actions_.insert(action,enumValue);
}

int Controller::getIndexOfControllerMethod(const QString &methodName)
{
    return  methods_.value(methodName,-1);
}

ConnectedUser &Controller::user() const{
    return *user_;
}

AppUsers &Controller::appUsers(){
    return *appUsers_;
}

ServerFiles &Controller::serverFiles(){
    return *serverFiles_;
}


void Controller::triggerAction(MagabeData data/*must be a copy*/)
{

    if(enabled()){
        //by defaut the actions are triggered for authenticated users only
        if(user_->isClientAuthenticated()){
            triggerActionDoContinue(data);
        }else {
            print(name()+":: ALERT: User not Authenticated");
        }

    }else{
        print("ALERT: "+name()+"  is Disabled");
    }
}

void Controller::triggerActionDoContinue(MagabeData& data){
    if(actions_.isEmpty()){
        qWarning() << name() << "::Error ,Action not set!";
        return;
    }

    const QString action = data.action();
    const QString methodName =  "on"+action+"Triggered";

    if(!isRegisteredAction(action)){
        print(QString("%1::Error ---> %2: is not a registered action!").arg(name()).arg(action));
        return;
    }

    if(-1 == getIndexOfControllerMethod(methodName)){
        print(QString("%1::Error ---> %2 method not found!").arg(name()).arg(methodName));
        return;
    }

    //set curent action
    currentExecutingAction_ = action;

#if PRINT_ACTION_TRIGGERED_MESSAGES
    //print action trigered
    printActionTriggered(action);
#endif

    //run action
    QGenericReturnArgument returnType(Q_NULLPTR);
    QMetaObject::invokeMethod(this,methodName.toStdString().c_str(),Qt::DirectConnection,
                              returnType,Q_ARG(Extras,data));
}


void Controller::wait(unsigned long secs){
    QThread::sleep(secs);
}


void Controller::printActionRegistrationError(){
    qWarning() << "CRITICAL ERROR:: Actions are not registered in " <<  name()
               << " ,The controller will not work .Register Actions in " + name()
               << " by putting a REGISTER_ACTIONS Macro on"
                  " header file, see Examples" ;

}


QString Controller::getCurrentExecutingAction() const
{
    return currentExecutingAction_;
}

/**
////////////MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
 * @brief Controller::isSingleInstanceActionFeatureEnabled
 * @return
 */
bool Controller::MESAE_isEnabled() const
{
    return MESAE_Enabled_;
}

bool Controller::MESAE_hasTimeout() const
{
    return MESAE_timeout_;
}

void Controller::MESAE_setOnRestartExecutionCallback(std::function<void ()> callback)
{
    MESAE_onRestartExecutionCallback_ = callback;
}

std::function<void ()> Controller::MESAE_getOnRestartExecutionCallback() const
{
    return MESAE_onRestartExecutionCallback_;
}

void Controller::initFileUploading(const QString &controller, const QString &action, const QString &callbackController, const QString &callbackAction, const QString &fileLocation,const QString &mgbSessionId){
    MagabeData data;
    data.setController(controller)
            .setAction(action)
            .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)
            .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)
            .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,fileLocation)
            .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,0)
            .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,false);
    QMetaObject::invokeMethod(DWritter::instance(),"sendData"
                              ,Q_ARG(const QString &,data.toByteArray())
                              ,Q_ARG( const QString &,mgbSessionId));
}

void Controller::registerObject(const QObject *object){
    MSignalsManager::instance()->registerObject(object);
}

void Controller::unRegisterObject(const QObject *object){
    MSignalsManager::instance()->unRegisterObject(object);
}

void Controller::registerControllerMethods(Controller *controller){
    if(controller){
        const int classMethodCount = controller->metaObject()->methodCount();
        const QByteArray className(controller->metaObject()->className());

        //  print(QString("%1::--->: initilizing methods ; methodCount = %2").arg(controller->name()).arg(classMethodCount));

        for(int methodIndex = 0; methodIndex < classMethodCount; methodIndex++){
            QMetaMethod method = controller->metaObject()->method(methodIndex);
            const QMetaObject *methodEnclosingMetaObject = method.enclosingMetaObject();
            if((className == methodEnclosingMetaObject->className())){
                controller->methods_.insert(method.name(),methodIndex);
            }
        }
    }
}

QStringList Controller::getSignalClassNames(const QString&  signalName){
    return  MSignalsManager::instance()->getSignalClassNames(signalName);
}

bool Controller::isSignalRegistered(const QString &className, const QString &signalName)
{
    return MSignalsManager::instance()->isSignalRegistered(className,signalName);
}

bool Controller::isSignalRegistered(const QString &signalName)
{
    return MSignalsManager::instance()->isSignalRegistered(signalName);
}

bool Controller::emitSignal(const QString &senderClassName, const QString &signalName, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9){
    return MSignalsManager::instance()->emitSignal(senderClassName,signalName,val0, val1,val2,val3,val4,val5,val6,val7,val8,val9);
}

bool Controller::connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName){
    return MSignalsManager::instance()->connectSlot(senderClassName,signalName,receiver,slotName);
}

bool Controller::connectSignals(const QString &senderClassName, const QString &senderSignalName, const QString &receiverClassName, const QString &receiverSignalName)
{
    return MSignalsManager::instance()->connectSignals(senderClassName,senderSignalName,receiverClassName, receiverSignalName);
}

bool MESAE_EnableFeature(Controller * controller,Extras *inputExtras
                         ,const QString& MESAE_tag
                         ,Controller::MESAE_OnValidateTagCallback MESAE_onValidateTagCallback){

    if(controller && inputExtras){
        controller->MESAE_InputExtras_ = inputExtras;
        controller->MESAE_Enabled_ = true;
        controller->MESAE_tag_ = MESAE_tag;
        controller->MESAE_onValidateTagCallback_ = MESAE_onValidateTagCallback;
        //add to list of MESAE controllers
        emit controller->MESAESignalDispatcher_
                .MESAEEnabledChanged(controller,true);
        return true;
    }else{
        return false;
    }
}

bool MESAE_DisableFeature(Controller * controller){
    if(controller){//WARNING do not put MESAE_isWaitForInput_ in here

        controller->MESAE_InputExtras_ = nullptr;
        controller->MESAE_Enabled_ = false;
        //remove from list MESAE controllers
        emit controller->MESAESignalDispatcher_
                .MESAEEnabledChanged(controller,false);
        //then
        controller->MESAE_hasActiveTimer_ = false;
        if(!controller->MESAE_semaphore_.available()){ //still no imput,gurd so we dont release more than 1 semaphore
            controller->MESAE_timeout_ = true;
            controller->MESAE_semaphore_.release(1);//release so we can exit thread
        }
        return true;
    }else{
        return false;
    }

}

void MESAE_WaitForInput(Controller * controller,int msecs){

    if(controller){
        controller->MESAE_InputExtras_->clear();//clear the existing input
        if(!controller->MESAE_hasActiveTimer_){ //only when has no active timer
            controller->MESAE_timeout_ = false;
            emit controller->MESAESignalDispatcher_.MESAE_WaitForInput(controller,msecs);
        }
    }
}

void Controller::onMESAE_timeout(int msecs){
    Q_UNUSED(msecs)
    MESAE_hasActiveTimer_ = false;
    if(!MESAE_semaphore_.available()){ //still no imput,gurd so we dont release more than 1 semaphore
        MESAE_timeout_ = true;
        MESAE_semaphore_.release(1);//release so we can exit thread or restart
    }else{//we have input,very late
        MESAE_timeout_ = false;
    }
}

//blocks the thread if no new input
void MESAE_Semaphore_acquire(Controller * controller){
    if(controller){
        controller->MESAE_semaphore_.acquire(1);
    }
}

void Controller::MESAE_UpdateInputExtras(Extras &extras){

    if((MESAE_InputExtras_) && (!extras.isEmpty())){

        MESAE_InputExtras_->setData(extras.toByteArray());
        if(!MESAE_InputExtras_->isEmpty()){
            MESAE_timeout_ = false;
            if(!MESAE_semaphore_.available()){ //gurd so we dont release more than 1 semaphore
                MESAE_semaphore_.release(1);//release so we can ready
            }
        }else{
            qDebug() << "CRITICAL ERROR::RECEIVED EMPTY EXTRAS";
        }
    }else{
        qDebug() << "CRITICAL ERROR::InputExtras is invalid";
    }
}

void Controller::MESAE_PauseThread(unsigned long secs){
    QThread::sleep(secs);
}

////////////////



/** ------------------OpenedFile----------------------
 * @brief Response::FileSender::OpenedFile::OpenedFile
 */
Response::FileSender::OpenedFile::OpenedFile():file_(nullptr){}

Response::FileSender::OpenedFile::OpenedFile(const QString &path, const QString &baseName, const QString &fileExtension, QFile *file, quint64 fileSize, quint64 position)
    :path_(path)
    ,baseName_(baseName)
    ,fileExtension_(fileExtension)
    ,file_(file)
    ,fileSize_(fileSize)
    ,position_(position)
{}

Response::FileSender::OpenedFile &Response::FileSender::OpenedFile::operator =(const Response::FileSender::OpenedFile &rhs){
    this->path_ = rhs.path_;
    this->baseName_ = rhs.baseName_;
    this->fileExtension_ = rhs.fileExtension_;
    this->file_ = rhs.file_;
    this->fileSize_ = rhs.fileSize_;
    this->position_ = rhs.position_;
    return *this;
}

QString Response::FileSender::OpenedFile::path() const{
    return path_;
}

QString Response::FileSender::OpenedFile::baseName() const{
    return baseName_;
}

QString Response::FileSender::OpenedFile::fileExtension() const{
    return fileExtension_;
}

QFile *Response::FileSender::OpenedFile::file(){
    return file_;
}

quint64 Response::FileSender::OpenedFile::fileSize(){
    return fileSize_;
}

quint64 Response::FileSender::OpenedFile::position(){
    return position_;
}

bool Response::FileSender::OpenedFile::isNull(){
    return (file_ == nullptr);
}

void Response::FileSender::OpenedFile::savePosition(quint64 position){
    position_ = position;
}

/** -----------CurrentFile----------
 * @brief Response::FileSender::CurrentFile::path
 * @return
 */
QString Response::FileSender::CurrentFile::path(){
    return path_;
}

QString Response::FileSender::CurrentFile::baseName(){
    return baseName_;
}

QString Response::FileSender::CurrentFile::fileExtension(){
    return fileExtension_;
}

quint64 Response::FileSender::CurrentFile::fileSize(){
    return fileSize_;
}

quint64 Response::FileSender::CurrentFile::position(){
    return position_;
}

QString Response::FileSender::CurrentFile::fileReceiver() const
{
    return fileReceiver_;
}

void Response::FileSender::CurrentFile::setFileReceiver(const QString &fileReceiver)
{
    fileReceiver_ = fileReceiver;
}

QString Response::FileSender::CurrentFile::fileSender() const
{
    return fileSender_;
}

void Response::FileSender::CurrentFile::setFileSender(const QString &fileSender)
{
    fileSender_ = fileSender;
}



/** --------------------------AppUsers------------------------------
 * @brief appUsers::insertNewUser
 * @param firstName
 * @param lastName
 * @param dateOfBirth
 * @param gender
 * @param username
 * @param password
 * @param email
 * @param profilePicturePath
 * @param education
 * @param position
 * @param location
 * @param phoneNumber
 * @param mgbSessionId
 * @return
 */
bool AppUsers::registerNewUser(const QString &firstName, const QString &lastName, const QString &dateOfBirth,const User::Gender gender, const QString &username, const QString &password, const QString &email, const QString &profilePicturePath, const QString &education, const QString &position, const QString &location, const QString &phoneNumber){
    return  userModel_.insertNewUser(firstName,lastName,dateOfBirth,gender,username,password,email,profilePicturePath,education,position,
                                     location,phoneNumber);
}

User AppUsers::getUser(unsigned long userId){
    auto user =  userModel_.getUser(userId);
    //check if has atleast one client and  SetThisClientSocket & SetThisClientMgbSessionId
    if(user.hasAtLeastOneConnectedClient()){
        auto sockets = user.getAllConnectedClientsSocketsForThisUser();
        const bool hasAtLeastOneConnectedClient = (sockets.count()  >= 1);//check again
        if(hasAtLeastOneConnectedClient){
            auto userSocket  = sockets.at(0);
            if(userSocket){
                auto mgbSessionId = ConnectedUserList::instance()->getMgbSessionId(userSocket);
                if(!mgbSessionId.isEmpty()){
                    userSetThisClientSocket(user,userSocket);
                    userSetThisClientMgbSessionId(user,mgbSessionId);
                }
            }
        }else{
            qWarning() << "User AppUsers::getUser(unsigned long userId)-> Critical Error ; logic error user.hasAtLeastOneConnectedClient() returned True but user.getAllConnectedClientsSocketsForThisUser() returned an empty list";
        }
    }
    return user;
}

User AppUsers::getUser(const QString &username){
    auto user = userModel_.getUser(username);
    //check if has atleast one client and  SetThisClientSocket & SetThisClientMgbSessionId
    if(user.hasAtLeastOneConnectedClient()){
        auto sockets = user.getAllConnectedClientsSocketsForThisUser();
        const bool hasAtLeastOneConnectedClient = (sockets.count()  >= 1);//check again
        if(hasAtLeastOneConnectedClient){
            auto userSocket  = sockets.at(0);
            if(userSocket){
                auto mgbSessionId = ConnectedUserList::instance()->getMgbSessionId(userSocket);
                if(!mgbSessionId.isEmpty()){
                    userSetThisClientSocket(user,userSocket);
                    userSetThisClientMgbSessionId(user,mgbSessionId);
                }
            }
        }else{
            qWarning() << "User AppUsers::getUser(const QString &username)-> Critical Error ; logic error user.hasAtLeastOneConnectedClient() returned True but user.getAllConnectedClientsSocketsForThisUser() returned an empty list";
        }
    }
    return user;

}

bool AppUsers::userRegistered(QString username){
    return  userModel_.userExists(username);
}

bool AppUsers::userRegistered(unsigned long userId){
    return userModel_.userExists(userId);
}

bool AppUsers::isValidEmail(const QString &email){
    return  userModel_.isValidEmail(email);
}

bool AppUsers::isEmailTaken(const QString &email)
{
    return  userModel_.isEmailTaken(email);
}

const QString AppUsers::getMgbSessionId(const QSslSocket *clientSocket){
    return  ConnectedUserList::instance()->getMgbSessionId(clientSocket);
}

QString AppUsers::getUserName(unsigned long userId){
    return userModel_.getUserName(userId);
}

unsigned long AppUsers::getUserId(const QString &username){
    return userModel_.getUserId(username);
}

bool AppUsers::isClientAuthenticated(const QSslSocket *clientSocket)
{
    return isClientAuthenticated(getMgbSessionId(clientSocket));
}

bool AppUsers::isClientAuthenticated(const QString &mgbSessionId)
{
    if(mgbSessionId.isEmpty()){
        return false;
    }else{
        return ConnectedUserList::instance()->isAuthenticated(mgbSessionId);
    }
}

bool AppUsers::closeClientConnetion(const QString &mgbSessionId){

    return MSignalsManager::instance()->emitSignal("ControllerIdentifier"
                                                   ,"closeClientSocket"
                                                   ,Q_ARG(QString,mgbSessionId));

}

bool AppUsers::closeClientConnetion(const QSslSocket *clientSocket)
{
    return closeClientConnetion(getMgbSessionId(clientSocket));
}

QString AppUsers::generateFilePath(const QString &folderName, const QString &fileName){
    return fileUtils_.generateFilePath(folderName,fileName);
}

int AppUsers::getTotal(){
    return  userModel_.getTotalNumberOfUsers();
}



/** ----------------------------------------------------------------------
 * @brief ServerFiles::isFileOwner
 * @param userId
 * @param fileNameOrAlias
 * @return
 */
ServerFiles::ServerFiles(){
    MSignalsManager::instance()->registerObject(&serverFilesSignals_);
    //connect newFileUploaded signal to MagabeServerApp
    MSignalsManager::instance()->connectSignals(RegisteredObjects::ServerFilesSignals::className,RegisteredObjects::ServerFilesSignals::Signals::newFileUploaded,RegisteredObjects::MagabeServerManagerSignalReceiver::className,RegisteredObjects::MagabeServerManagerSignalReceiver::Signals::newFileUploaded);
}

ServerFiles::~ServerFiles(){
    MSignalsManager::instance()->unRegisterObject(&serverFilesSignals_);
}
bool ServerFiles::isFileOwner(unsigned long userId, const QString &fileNameOrAlias)
{
    return (fileNameOrAlias.isEmpty() || (userId == 0)) ? false : serverFilesModel_.isFileOwner(userId,fileNameOrAlias);
}

QString ServerFiles::getFilePath(const QString &fileNameOrAlias)
{
    return (fileNameOrAlias.isEmpty()) ? "" : serverFilesModel_.getFilePath(fileNameOrAlias);
}

bool ServerFiles::fileExists(const QString &fileNameOrAlias)
{
    return (fileNameOrAlias.isEmpty()) ? false : serverFilesModel_.fileExists(fileNameOrAlias);
}

bool ServerFiles::deleteFile(unsigned long userId, const QString &fileNameOrAlias)
{
    return (fileNameOrAlias.isEmpty() || (userId == 0)) ? false : serverFilesModel_.deleteFile(userId,fileNameOrAlias);
}

QString ServerFiles::recordFile(unsigned long userId, const QString &filePath)
{
    if((userId == 0) || (filePath.isEmpty())){
        return "";
    }

    if(!fileExistsOnDisk(filePath)) return "";

    const QString fileExtension = getFileExtension(filePath);
    const QString fileName = getFileName(filePath);
    if(fileExtension.isEmpty()){
        qWarning() << "ServerFiles::recordFile :: fileExtension implemention Error";
        return "";
    }

    if(fileName.isEmpty()){
        qWarning() << "ServerFiles::recordFile :: fileName implemention Error";
        return "";
    }

    const QString fAlias = generateAlias(userId,fileExtension);
    if(fAlias.isEmpty()){
        qWarning() << "ServerFiles::recordFile :: fileAlias implemention Error";
        return "";
    }

    if(serverFilesModel_.fileExists(fileName)){
        auto dbPath = serverFilesModel_.getFilePath(fileName);
        const bool probablyTheSameFile = (serverFilesModel_.isFileOwner(userId,fileName)
                                          && (getFileSize(dbPath) == getFileSize(filePath)) );
        if(probablyTheSameFile){
            serverFilesModel_.deleteFile(userId,fileName);
            qWarning() << QString("ServerFiles::recordFile :: User (id = %1), uploaded the same file ,the old one deleted").arg(userId);
        }

    }

    QString newfilePath  = getFileFolder(filePath).append("/").append(fAlias);
    const bool renamed = QFile::rename(filePath,newfilePath);
    if(renamed){
        if(serverFilesModel_.recordFile(userId,newfilePath,fileName,fAlias)){
            emit serverFilesSignals_.newFileUploaded(userId,newfilePath);

            return newfilePath;
        }else{
            qWarning() << "ServerFiles::recordFile :: Recording to db failed";
            return "";
        }
    }else{
        qWarning() << "ServerFiles::recordFile :: File renaming filed";
        return "";
    }
}


bool ServerFiles::fileExistsOnDisk(const QString &filePath){
    return QFileInfo::exists(filePath);
}

qint64 ServerFiles::getFileSize(const QString &filePath){
    QFileInfo file(filePath);
    return file.size();
}

QString ServerFiles::getFileExtension(const QString &filePath){
    if(filePath.isEmpty()) return "";
    QFileInfo file(filePath);
    if(!file.exists()) return "";

    return  file.completeSuffix();
}

QString ServerFiles::getFileName(const QString &filePath){
    if(filePath.isEmpty()) return "";
    QFileInfo file(filePath);
    if(!file.exists()) return "";

    return  file.fileName();
}

bool ServerFiles::copyFile(const QString &currentPath, const QString &newPath)
{
    if(QFile::exists(newPath)){//if the file already exists
        return true;
    }
    return QFile::copy(currentPath,newPath);
}

QString ServerFiles::generateAppRootFilePath(const QString &fileName)
{
    return FileUtils::generateAppRootFilePath(fileName);
}

QString ServerFiles::generateFilePath(const QString &folderName,const QString &fileName)
{
    return FileUtils::generateFilePath(folderName,fileName);
}

QString ServerFiles::getFileFolder(const QString &filePath){
    if(filePath.isEmpty()) return "";
    QFileInfo file(filePath);
    if(!file.exists()) return "";

    return  file.absoluteDir().absolutePath();
}

QString ServerFiles::getAppRootPath(){
    return FileUtils::getAppRootPath();
}

QString ServerFiles::getFileBaseName(const QString &filePath){
    if(filePath.isEmpty()) return "";
    QFileInfo file(filePath);
    if(!file.exists()) return "";

    return  file.baseName();
}

QString ServerFiles::generateAlias(unsigned long userId,const QString &fileExtension){

    QString alias =  QString("%1_%2_").arg(userId).arg(QDateTime::currentMSecsSinceEpoch())
            + serverFilesModel_.generateRandomString(56).append(".").append(fileExtension);
    while(fileExists(alias)){
        alias =  QString("%1_%2_").arg(userId).arg(QDateTime::currentMSecsSinceEpoch())
                + serverFilesModel_.generateRandomString(56).append(".").append(fileExtension);
    }
    return alias;
}
/*------------------------------------------------------*/
