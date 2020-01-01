/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QMetaEnum>
#include "Helpers/magabedata.h"
#include <QSslSocket>
#include "Helpers/mfile.h"
#include <QSemaphore>
#include <QFileInfo>
#include <QFile>

//ONLY USE THIS MACRO TO TRIGGER OTHER ACTION , DO NOT CALL THE SLOT DIRECTLY
#define triggerAnotherAction(actionName,extras)\
    currentExecutingAction_ = #actionName;/*so that ControllerIdentifier add this as MESAE controller,and not the calling controller */\
    on##actionName##Triggered(extras);


#define REGISTER_ACTIONS\
    private slots:\
    void registerActions(){\
    QMetaObject metaobj = this->staticMetaObject; \
    QMetaEnum actionsMetaEnum_ = metaobj.enumerator(metaobj.indexOfEnumerator("Actions")); \
    for(int index = 0;index<actionsMetaEnum_.keyCount();index++){\
    QString action = actionsMetaEnum_.key(index);\
    int enumValue = actionsMetaEnum_.value(index);\
    registerAction(action,enumValue);\
    }\
    }\
    public:

//////////// MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
#define MESAE_BEGIN_LOOP(WAIT_XSECS_BEFORE_START_MESAE,MESAE_TAG,MESAE_ONVALIDATE_TAG_CALLBACK)\
    qint64 MESAE_waitXSecsBeforeStartMESAE = WAIT_XSECS_BEFORE_START_MESAE;\
    int MESAE_giveUpExecuteAfterXSecs = 15;\
    bool MESAE_restartAfterTimeOut = false;\
    MESAE_EnableFeature(this,&extras,MESAE_TAG,MESAE_ONVALIDATE_TAG_CALLBACK);\
    MESAE_PauseThread(MESAE_waitXSecsBeforeStartMESAE);\
    while (user().isConnected() && MESAE_isEnabled()) {\
    MESAE_Semaphore_acquire(this);\
    if(user().isConnected() && MESAE_isEnabled()){\
    if(extras.isEmpty() && MESAE_hasTimeout()){\
    if(MESAE_restartAfterTimeOut){\
    if(MESAE_getOnRestartExecutionCallback()){\
    MESAE_getOnRestartExecutionCallback()();\
    }\
    goto MESAE_waitForInput_;\
    }else{\
    goto MESAE_exitExecution_;\
    }\
    }\
    if(0 == extras.getExtrasCount()){\
    if(MESAE_restartAfterTimeOut){\
    if(MESAE_getOnRestartExecutionCallback()){\
    MESAE_getOnRestartExecutionCallback()();\
    }\
    goto MESAE_waitForInput_;\
    }else{\
    goto MESAE_exitExecution_;\
    }\
    }\
    }else{\
    goto MESAE_exitExecution_;\
    }

#define MESAE_END_LOOP(GIVE_UP_WAITING_FOR_INPUT_AFTER_XSECS)\
    MESAE_giveUpExecuteAfterXSecs = GIVE_UP_WAITING_FOR_INPUT_AFTER_XSECS;\
    MESAE_waitForInput_: \
    int  MESAE_timeoutAfter = 1000*((MESAE_giveUpExecuteAfterXSecs < 0) ?\
    60 :  MESAE_giveUpExecuteAfterXSecs);\
    MESAE_WaitForInput(this,MESAE_timeoutAfter);\
    }\
    MESAE_exitExecution_:\
    MESAE_DisableFeature(this);

#define  MESAE_exitExecution() goto MESAE_exitExecution_;
#define  MESAE_restartExecution() goto MESAE_waitForInput_;
#define  MESAE_waitForInput() goto MESAE_waitForInput_;
///////////////////


class Controller;
struct User;
class ControllerIdentifier;

struct Response{
    /**
     * @brief The ResponseError struct
     */
    struct ResponseError
    {
        enum ErrorType {
            NoError = 0,
            ControllerNotSetError,
            ActionNotSetError,
            UserDisconnectedError,
            DataNotSetError
        };

        QString toString();
        ResponseError::ErrorType type();
        bool operator ==(ResponseError::ErrorType type);
        bool operator ==(ResponseError error);

    private:
        friend struct Response;
        ErrorType errorType_ = NoError;
        unsigned long userId = 0;
    };

    class FileSender{

    public:
        struct ExtrasNames{
            NEW_EXTRAS_NAME(callbackController)
            NEW_EXTRAS_NAME(callbackAction)
            NEW_EXTRAS_NAME(filePosition)
            NEW_EXTRAS_NAME(FileSenderError)
            NEW_EXTRAS_NAME(fileLocation)
            NEW_EXTRAS_NAME(fileReceived)
        };

        struct FileSenderError
        {
            enum ErrorType {
                NoError = 0,
                FileDoesNotExistsError,
                CantOpenFileError,
                InvalidFilePositionError,
                InvalidFilePathError,
                EmptyFileError,
                UnknownError,
                CausedByAResponseError
            };

            FileSenderError();
            FileSenderError(FileSenderError::ErrorType type);
            QString toString();
            FileSenderError::ErrorType type();
            bool operator ==(FileSenderError::ErrorType type);
            bool operator ==(FileSenderError error);
            bool operator !=(FileSenderError::ErrorType type);
            bool operator !=(Response::FileSender::FileSenderError error);
        private:
            friend struct FileSender;
            ErrorType errorType_;
            unsigned long userId;
        };

        /**
         * @brief The CurrentFile struct
         */
        struct CurrentFile{
            QString path();
            QString baseName();
            QString fileExtension();
            quint64 fileSize();
            quint64 position();
            QString fileReceiver() const;
            void setFileReceiver(const QString &fileReceiver);
            QString fileSender() const;
            void setFileSender(const QString &fileSender);
        private:
            friend class FileSender;
            QString path_;
            QString baseName_;
            QString fileExtension_;
            quint64 fileSize_=0;
            quint64 position_=0;
            QString fileSender_;
            QString fileReceiver_;

        };

        /**
         * @brief The OpenedFile class
         */
        class OpenedFile {
            QString path_;
            QString baseName_;
            QString fileExtension_;
            QFile *file_;//just own a pointer, do not use it to delete
            quint64 fileSize_;
            quint64 position_;

        public:
            OpenedFile();
            OpenedFile(const QString &path,const QString &baseName
                       ,const QString &fileExtension
                       ,QFile *file
                       ,quint64 fileSize,quint64 position);
            OpenedFile& operator = (const OpenedFile &rhs);
            QString path() const;
            QString baseName() const;
            QString fileExtension() const;
            QFile *file();
            quint64 fileSize();
            quint64 position();
            bool isNull();
            void savePosition( quint64 position);
        };//OpenedFile class

        FileSender();
        //destroctor close opened files,when the The FileSender object is destroyed
        ~FileSender();
        FileSenderError &lastError();
        QString compressAndEncrypt(const QByteArray &fileByteArray);
        bool sendFile(const QString &path,quint64 startPosition);
        bool sendFile();
        bool sendFile(const QString &path);
        void setTransferRate(quint64 bytes);
        void setStartPosition(quint64 startPosition);
        bool setFilePath(const QString &path);
        QString getFilePath();
        OpenedFile  openFile(const QString& path,bool *opened,FileSenderError::ErrorType *errorType);
        void closeFile(const QString& path);
        CurrentFile& currentFile();
        void setFileTag(const QString &fileTag);

    private:
        friend struct Response;
        //24kb //DO NOT WRITE MANY BYTES,
        //THE ROOPING WIIL B BIG, AND COS MORE CPU,small is fast
        quint64 transferRate_ = 1024*24;
        FileSenderError fileSenderError_;
        quint64 position_ = 0;
        QString fpath_;
        Response *response_;
        SimpleCrypt crypto_;
        QString fileTag_;
        QMap<QString,OpenedFile> openedFileMap_;

        void addOpenedFile(const QString& path,OpenedFile& openedFile);
        void removeAndCloseOpenedFile(const QString& path);
        //may return a null OpenedFile if is not open
        OpenedFile getOpened(const QString& path);
        bool isFileOpen(const QString& path);
        CurrentFile currentFile_;

    };//FileSender

    Response(User *user);
    Response &addExtras(const QString &name, int value);
    Response &addExtras(const QString &name, bool value);
    Response &addExtras(const QString &name, float value);
    Response &addExtras(const QString &name, double value);
    Response &addExtras(const QString &name, long value);
    Response &addExtras(const QString &name,unsigned long value);
    Response &addExtras(const QString &name, quint64 value);
    Response &addExtras(const QString &name,const QString &value);
    Response &addExtras(const QString &name, const char *value);
    Response &addExtras(const QString &name, const QJsonObject &object);
    Response &addExtras(const QString &name, const QJsonArray &array);
    Response &addExtras(const QString &name, const QStringList &list);

    Response &addExtras(const char *name, const char *value);
    Response &addExtras(const char *name, const QByteArray &value);
    Response &addExtras(const char *name, const QString &value);
    Response &addExtras(const char *name, int value);
    Response &addExtras(const char *name, bool value);
    Response &addExtras(const char *name, float value);
    Response &addExtras(const char *name, double value);
    Response &addExtras(const char *name, long value);
    Response &addExtras(const char *name, qint64 value);
    Response &addExtras(const char *name, unsigned long long value);
    Response &addExtras(const char *name, const QJsonObject &object);
    Response &addExtras(const char *name, const QJsonArray &array);
    Response &addExtras(const char *name, const QStringList &list);

    Response &removeExtras(const QString &name);
    Response &removeExtras(const char* name);

    bool containsExtras(const QString& name);
    bool containsExtras(const char* name);

    int getExtrasCount();
    void clear();
    QString controller();
    Response &setController(const QString &controller);
    QString action();
    Response &setAction(const QString &action);
    QJsonObject getJsonObjectExtras(const QString &name);
    QJsonArray getJsonArrayExtras(const QString &name);
    QJsonObject getJsonObjectExtras(const char *name);
    QJsonArray getJsonArrayExtras(const char *name);
    QVariantList getVariantListExtras(const QString &name);
    bool send();
    bool send(MagabeData &data);
    ResponseError &lastError();
    FileSender &fileSender();
    bool queueBypassSend();

private:
    friend class FileSender;
    MagabeData data_;
    ResponseError responseError_;
    FileSender fileSender_;
    User *user_;//not owner do not delete
};


typedef Response::ResponseError ResponseError;
typedef Response::FileSender::FileSenderError FileSenderError;

class User : public QObject{
    Q_OBJECT

public:
    explicit  User(Controller *controller);
    ~User();
    bool isConnected();
    bool send(MagabeData &data);
    unsigned long getId() const;
    Response& response() const;
signals:
    void sendData(QString data);
private slots:
    void onDisconnected();

private:
    friend struct Response;
    friend class Controller;
    unsigned long id_ =0;
    Response *response_;
    QSslSocket *socket_ ;
    Controller *controller_;
};


/**
 //MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
 * @brief The MESAESignalDispatcher class
 */
class MESAESignalDispatcher : public QObject
{
    Q_OBJECT

public:
    explicit MESAESignalDispatcher (QObject *parent = nullptr)
        : QObject(parent){}
signals:
    void MESAEEnabledChanged(Controller *controller,bool enabled);
    void MESAE_WaitForInput(Controller *controller,int msecs);

};

class ConnectionPipe;
class AppFiles{

};

/** -------------------------Controller----------------------------
 * @brief The Controller class
 */
class Controller : public QObject
{
    Q_OBJECT

public:

    explicit Controller(QObject *parent = nullptr);
    ~Controller();
    /*the newly created object must have a null parent*/
    virtual Controller* createNew()=0;
    void sendDataToMagabeServer(MagabeData &data);
    void print(QString status);
    QString name() const;
    void setName(const QString &name);
    bool enabled() const;
    void setEnabled(bool enabled);
    static void performAction(Controller *controller, MagabeData data);
    User &user()const;
    void wait(unsigned long secs);

    ///MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE

    /* dont forget the boolea return type when setting this callback using a lamba
    example;-  notice the  -> bool
            [=](const QString& MESAE_tag,Extras& receivedData) ->bool {
                    qDebug() << "messe tag" << MESAE_tag <<"received data" << receivedData.toByteArray();
                    return true;
                }
    */
    typedef bool(*MESAE_OnValidateTagCallback)(const QString& MESAE_tag,Extras& receivedData);

    bool MESAE_isEnabled() const;
    void MESAE_PauseThread(unsigned long secs);
    bool MESAE_hasTimeout() const;
    /*if MESAE_restartAfterTimeOut is set to true,
    MESAE_onRestartExecutionCallback_ must be set to to provide a way to
     terminate the execution*/
    void MESAE_setOnRestartExecutionCallback(std::function<void(void)> callback);
    std::function<void ()> MESAE_getOnRestartExecutionCallback() const;
    ///

    QString getCurrentExecutingAction() const;
    static void initFileDownloading(const QString& controller,const QString& action
                                    ,const QString& callbackController,const QString& callbackAction
                                    ,const QString& fileLocation/*path/uri*/);

    static bool isSignalRegistered(const QString &className, const QString &signalName);
    static bool isSignalRegistered(const QString &signalName);
    static bool emitSignal(const QString &senderClassName, const QString &signalName, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9);
    static bool connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName);
    //return names of classes thet contain that signal
    static QStringList getSignalClassNames(const QString&  signalName);
    static bool connectSignals(const QString&  senderClassName,const QString&  senderSignalName,
                               const QString&  receiverClassName,const QString&  receiverSignalName);
    int getIndexOfControllerMethod(const QString &methodName);
private slots:
    virtual void triggerAction(MagabeData data);
    virtual void registerActions();
    void onPerformActionFinished();

protected:
    void triggerActionDoContinue(MagabeData& data);
    void printActionTriggered(const QString &action);
    bool isRegisteredAction(const QString& action);
    void registerAction(const QString &action, const int enumValue);

    QString currentExecutingAction_;

private:
    friend class ControllerIdentifier;
    friend class  User;
    friend class MESAE_Controllers;
    friend class MESAE_Controller;

    void setSocket(QSslSocket *socket);
    User *user_;
    QString name_;
    bool enabled_;
    QMap<QString, int>  actions_; //actios and their values
    void printActionRegistrationError();
    QMap<QString,int> methods_;



    //// MAGABE EDWIN  SYNCHRONIZED ACTION EXECUTION - MESAE
    friend bool MESAE_EnableFeature(Controller * controller,Extras *inputExtras
                                    ,const QString& MESAE_tag
                                    ,MESAE_OnValidateTagCallback MESAE_onValidateTagCallback);
    friend bool MESAE_DisableFeature(Controller * controller);
    friend void MESAE_WaitForInput(Controller * controller,int msecs);//NEW
    friend void MESAE_Semaphore_acquire(Controller * controller);
    void onMESAE_timeout(int msecs);
    void MESAE_UpdateInputExtras(Extras &extras);
    bool MESAE_timeout_;
    bool MESAE_Enabled_;
    Extras* MESAE_InputExtras_;
    QSemaphore MESAE_semaphore_;
    MESAESignalDispatcher MESAESignalDispatcher_;
    bool MESAE_hasActiveTimer_;
    std::function<void(void)> MESAE_onRestartExecutionCallback_;
    QString MESAE_tag_;
    MESAE_OnValidateTagCallback MESAE_onValidateTagCallback_;

    ///

    friend class ConnectionPipe;
    static void registerObject(const QObject *object);
    static void unRegisterObject(const QObject *object);
    void registerControllerMethods(Controller *controller);
};

#define CONTROLLER_SET_NAME(clazz) \
    setName(#clazz);\
    setObjectName(#clazz);


#define FILE_DOWNLOAD_SETUP_ACTION(FILESENDER_CONTROLLER,FILESENDER_ACTION \
    ,CALLBACK_CONTROLLER,CALLBACK_ACTION,SAVE_FOLDER \
    ,DOWNLOAD_PROGRESS_CALLBACK,ON_ERROR_CALLBACK,ON_FILE_ALREADY_DOWNLOADED_CALLBACK) \
    QString fileSenderController = FILESENDER_CONTROLLER; \
    QString fileSenderAction = FILESENDER_ACTION; \
    QString saveFolder = SAVE_FOLDER; \
    QString callbackController = CALLBACK_CONTROLLER; \
    QString callbackAction = CALLBACK_ACTION; \
    QString fileTag;\
    std::function<void(QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  downloaded,quint64 fileSize,QString fileTag)> downloadProgressCallback = DOWNLOAD_PROGRESS_CALLBACK; \
    std::function<void(int errorNo,QString errorString,QString fileTag)> onErrorCallback = ON_ERROR_CALLBACK; \
    /*(in onFileAlreadyDowbloadedCallback) return true to reDownload or false to exit execution*/\
    std::function<bool(QString filePath,QString fileTag)> onFileAlreadyDowbloadedCallback = ON_FILE_ALREADY_DOWNLOADED_CALLBACK; \
    QString errorString = "No Error";\
    int errorNo = 0;\
    if(!downloadProgressCallback){\
    downloadProgressCallback = [=]\
    (QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  downloaded,quint64 fileSize, QString fileTag){\
    Q_UNUSED(downloaded)\
    Q_UNUSED(fileBaseName)\
    Q_UNUSED(fileExtension)\
    Q_UNUSED(fileLocation)\
    auto status = QStringLiteral("downloading... ")+QString("%1.%2 [%3% of %4 MB]")\
    .arg(fileBaseName).arg(fileExtension).arg(percentageCompletion).arg((float)fileSize/(1024*1024));\
    qDebug() << status;\
    };\
    }\
    if(!onErrorCallback){\
    onErrorCallback = [=](int errorNo,QString errorString,QString fileTag){\
    qWarning() << "errorNo#"<< errorNo <<" | "<< errorString;\
    };\
    }\
    if(callbackController.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> callbackController is not set !";\
    errorNo = 1;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    if(callbackAction.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> callbackAction is not set !";\
    errorNo = 2;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    if(fileSenderController.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> fileSenderController is not set !";\
    errorNo = 3;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    \
    if(fileSenderAction.isEmpty()){\
    errorString = getCurrentExecutingAction() +"::Error -> fileSenderAction is not set !";\
    errorNo = 4;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    if(saveFolder.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> saveFolder is not set !";\
    errorNo = 5;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    bool downloadCompleted = false;\
    quint64  chunkSize;\
    quint64 fileSize;\
    QString  fileBaseName;\
    QString  fileExtension;\
    quint64 senderFpos;\
    quint64  receiverFpos;\
    bool bytesStillAvailable;\
    QString fileSender;\
    QString fileReceiver;\
    fileBaseName =  extras.getStringExtras(MFile::ExtrasNames::fileBaseNameExtras);\
    fileExtension =  extras.getStringExtras(MFile::ExtrasNames::fileExtensionExtras);\
    QString fileLocation = extras.getStringExtras(MFile::ExtrasNames::fileLocationExtras);\
    const QString& MESAE_Tag = fileLocation;/*using file location as a MESAE_TAG*/\
    auto MESAE_onValidateTagCallback = [](const QString& MESAE_tag,Extras& receivedData) ->bool {\
    QString tagFromReceivedData = receivedData.getStringExtras(MFile::ExtrasNames::fileLocationExtras);\
    return (MESAE_tag == tagFromReceivedData);\
    };\
    QDir appDir_;\
    QString fileFolder = saveFolder;\
    QString  fDir = appDir_.homePath();\
    fDir.append(QString("/.%1/%2").arg(APP_FOLDER).arg(fileFolder));\
    appDir_.setPath(fDir);\
    if(!appDir_.exists()){\
    if(!appDir_.mkpath(fDir)){\
    errorString = getCurrentExecutingAction() + "::Error ->  Failed to create folder ;- " + fDir;\
    errorNo = 6;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    return;\
    }\
    }\
    QString fName = fileBaseName+"."+fileExtension;\
    QString tmpfName = fileBaseName+TEMPORARY_FILE_EXTENSION;\
    QString tmpfPath,filePath;\
    tmpfPath = filePath = fDir;\
    tmpfPath.append("/").append(tmpfName);\
    filePath.append("/").append(fName);\
    QFile file(tmpfPath);\
    quint64 position = 0;\
    if(file.exists()){\
    position = file.size();\
    }\
    QByteArray receivedBytes;\
    QFile fullFile(filePath);\
    checkIfFullFileExists:\
    if(fullFile.exists()){\
    if(onFileAlreadyDowbloadedCallback){\
    if(onFileAlreadyDowbloadedCallback(filePath,fileTag)){\
    fullFile.remove();\
    goto checkIfFullFileExists;\
    }else{\
    downloadCompleted = true;\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,extras.getStringExtras(MFile::ExtrasNames::fileLocationExtras))\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,fullFile.size())\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,true)\
    .send();\
    return;\
    }\
    }else{\
    downloadCompleted = true;\
    errorString = getCurrentExecutingAction() +     QString("::Error ->  File already downloded to path ;- %1 and ON_FILE_ALREADY_DOWNLOADED_CALLBACK is not set").arg(filePath);\
    errorNo = 10;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,extras.getStringExtras(MFile::ExtrasNames::fileLocationExtras))\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,fullFile.size())\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,true)\
    .send();\
    return;\
    }\
    }else{\
    MESAE_BEGIN_LOOP(0.1,MESAE_Tag,MESAE_onValidateTagCallback)\
{\
    if(fileTag.isEmpty()){\
    auto tmp = extras.getStringExtras(MFile::ExtrasNames::fileTagExtras);\
    if(!tmp.isEmpty()) fileTag = tmp;\
    }\
    MESAE_restartAfterTimeOut = true;\
    MESAE_setOnRestartExecutionCallback([=](){\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,fileLocation)\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,position)\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,false)\
    .send();\
    });\
    MFile  chunkfile(extras);\
    if(!chunkfile.isValid()){\
    MESAE_exitExecution();\
    }\
    Response::FileSender::FileSenderError senderError((Response::FileSender::FileSenderError::ErrorType)extras.getIntExtras(Response::FileSender::ExtrasNames::FileSenderErrorExtras));\
    if(senderError != Response::FileSender::FileSenderError::ErrorType::NoError){\
    errorString = getCurrentExecutingAction() + "::FileSenderError -> "  + senderError.toString();\
    errorNo = 9;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    MESAE_exitExecution();\
    }\
    chunkSize =  chunkfile.getChunkSize();\
    fileSize =  chunkfile.getFileSize();\
    senderFpos =  chunkfile.getSenderFpos();\
    receiverFpos =  chunkfile.getReceiverFpos();\
    fileLocation =  chunkfile.getFileLocation();\
    bytesStillAvailable =  chunkfile.getBytesStillAvailable();\
    fileSender =  chunkfile.getSenderUsername();\
    fileReceiver =  chunkfile.getReceiverUsername();\
    if(position != receiverFpos){\
    if((!receivedBytes.isEmpty()) && file.open(QFile::Append)){\
    file.write(receivedBytes,receivedBytes.size());\
    file.close();\
    }\
    receivedBytes.clear();\
    position = file.size();\
    goto continueDownloading;\
    }\
    if(chunkSize > 0){\
    receivedBytes.append(chunkfile.getChunkBytes());\
    position = senderFpos;\
    float percentagecompletion = ((float)position/fileSize)*100;\
    if(downloadProgressCallback){\
    downloadProgressCallback(fileBaseName,fileExtension,fileLocation,percentagecompletion,position,fileSize,fileTag);\
    }\
    if(!bytesStillAvailable){\
    goto saveFile;\
    }\
    }\
    if(receivedBytes.size() >= (1024*1024)){\
{saveFile:\
    if(!file.open(QFile::Append)){\
    errorString =  getCurrentExecutingAction() + "::Error -> Failed to open :: " + tmpfPath + " for serving";\
    errorNo = 7;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    MESAE_exitExecution();\
    }\
    qint64 byteWrittern = file.write(receivedBytes,receivedBytes.size());\
    if( byteWrittern != -1){ \
    receivedBytes.clear();\
    file.close();\
    position = file.size();\
    if(position == fileSize){\
    QFile::rename(tmpfPath.toLatin1(),filePath.toLatin1()); \
    MESAE_PauseThread(0.5);/*Wait for 0.5sec For fileSystem to update,prevent notsent errors*/\
    downloadCompleted = true;\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,fileLocation)\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,position)\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,downloadCompleted)\
    .send();\
    MESAE_exitExecution();\
    }else{\
    goto continueDownloading;\
    }\
    }else{\
    file.close();\
    position = senderFpos;\
    errorString =  getCurrentExecutingAction() + "::Error -> Failed to write to file";\
    errorNo = 8;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString,fileTag);\
    }\
    }\
    }\
{continueDownloading:\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,fileLocation)\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,position)\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,false)\
    .send();\
    }\
    } MESAE_END_LOOP(45)\
    }




#define FILE_SENDER_SETUP_ACTION(FILE_TAG,FILE_SENDING_PROGRESS_CALLBACK,ON_ERROR_CALLBACK)\
    std::function<void(QString fileTag,QString fileLocation,float percentageCompletion,quint64 fileSize)> fileSendingProgressCallback = FILE_SENDING_PROGRESS_CALLBACK; \
    std::function<void(QString fileTag,int fileSenderError)> onErrorCallback = ON_ERROR_CALLBACK; \
    if(!fileSendingProgressCallback){\
    fileSendingProgressCallback = [=](QString fileTag,QString fileLocation,float percentageCompletion,quint64 fileSize){\
    Q_UNUSED(fileTag)\
    auto status = QStringLiteral("sending file... ")+QString("%1 [%2% of %3 MB]")\
    .arg(fileLocation).arg(percentageCompletion).arg((float)fileSize/(1024*1024));\
    qDebug() << status;\
    };\
    }\
    QString callbackAction;\
    QString callbackController;\
    quint64 filePosition;\
    auto fileSender = user().response().fileSender();\
    bool fileSendingFinished = false;\
    bool errorOccured = false;\
    Response::FileSender::FileSenderError error;\
    QString fileTag = extras.getStringExtras(MFile::ExtrasNames::fileTagExtras);\
    if(fileTag.isEmpty()){\
    fileTag = FILE_TAG;\
    }\
    fileSender.setFileTag(fileTag);\
    QString fileLocation = extras.getStringExtras(Response::FileSender::ExtrasNames::fileLocationExtras);\
    const QString& MESAE_Tag = fileLocation;/*using file location as a MESAE_TAG*/\
    auto MESAE_onValidateTagCallback = [](const QString& MESAE_tag,Extras& receivedData) ->bool {\
    QString tagFromReceivedData = receivedData.getStringExtras(Response::FileSender::ExtrasNames::fileLocationExtras);\
    return (MESAE_tag == tagFromReceivedData);\
    };\
    MESAE_BEGIN_LOOP(0.3,MESAE_Tag,MESAE_onValidateTagCallback)\
{\
    fileSendingFinished = extras.getBooleanExtras(Response::FileSender::ExtrasNames::fileReceivedExtras);\
    if(fileSendingFinished){\
    MESAE_exitExecution();\
    }\
    callbackAction = extras.getStringExtras(Response::FileSender::ExtrasNames::callbackActionExtras);\
    callbackController = extras.getStringExtras(Response::FileSender::ExtrasNames::callbackControllerExtras);\
    filePosition = extras.getULongLongExtras(Response::FileSender::ExtrasNames::filePositionExtras);\
    user().response()\
    .setController(callbackController)\
    .setAction(callbackAction);\
    bool sent =  fileSender.sendFile(fileLocation,filePosition);\
    if(!sent){\
    error  = fileSender.lastError();\
    errorOccured = (error.type() != Response::FileSender::FileSenderError::ErrorType::NoError);\
    user().response().addExtras(Response::FileSender::ExtrasNames::FileSenderErrorExtras,(int)error.type()).send();\
    if(onErrorCallback){\
    onErrorCallback(fileTag,(int)error.type());\
    }\
    MESAE_exitExecution();\
    }else{\
    auto currentFile = fileSender.currentFile();\
    quint64 fileSize  = currentFile.fileSize();\
    quint64 position = currentFile.position();\
    float percentagecompletion = ((float)position/fileSize)*100;\
    if(fileSendingProgressCallback){\
    fileSendingProgressCallback(fileTag,fileLocation,percentagecompletion,fileSize);\
    }\
    }\
    } MESAE_END_LOOP(60)\


#define TO_CSTR(name) #name
#define NEW_CONTROLLER_NAME(name) constexpr static const char* name##ControllerName = TO_CSTR(name##Controller);

#endif // CONTROLLER_H
