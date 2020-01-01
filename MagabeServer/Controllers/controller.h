/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include <QMetaEnum>
#include "Helpers/magabedata.h"
#include "Helpers/user.h"
#include "Models/usermodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QFile>
#include "Helpers/mfile.h"
#include <QDateTime>
#include <QSemaphore>
#include "Helpers/simplecrypt.h"
#include "Models/serverfilesmodel.h"
#include "Helpers/fileutils.h"
#include "Helpers/errors.h"


class UserObjectEditor;
struct ConnectedUser;
class ControllerIdentifier;

//ONLY USE THIS MACRO TO TRIGGER OTHER ACTION , DO NOT CALL THE SLOT DIRECTLY
#define triggerAnotherAction(actionName,extras)\
    currentExecutingAction_ = #actionName;/*so that ControllerIdentifier add this as MESAE controller,and not the calling controller */\
    on##actionName##Triggered(extras);


//using this macro trick because te base class hhas no enum info
// about its its derived class at object
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
    while (user().thisClientConnectionIsActive() && MESAE_isEnabled()) {\
    MESAE_Semaphore_acquire(this);\
    if(user().thisClientConnectionIsActive() && MESAE_isEnabled()){\
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


#define CONTROLLER_SET_NAME(clazz) \
    setName(#clazz);\
    setObjectName(#clazz);


/**
 * @brief The Response struct
 */
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
        bool operator ==(ErrorType type);
        bool operator ==(ResponseError error);
    private:
        friend struct Response;
        ErrorType errorType_ = NoError;
        unsigned long userId = 0;
    };


    /**
     * @brief The FileSender class
     */
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
            bool operator ==(FileSender::FileSenderError error);
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
        FileSender::FileSenderError &lastError();
        QString compressAndEncrypt(const QByteArray &fileByteArray);
        bool sendFile(const QString& path, quint64 startPosition);
        bool sendFile();
        bool sendFile(const QString& path);
        void closeOpenFiles();
        /*default is 24kb*/
        void setTransferRate(quint64 bytes);
        void setStartPosition(quint64 startPosition);
        bool setFilePath(const QString& path);
        QString getFilePath();
        quint64 getPosition() const;
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
        Response* response_;//do not delete, its not the owner
        QString fpath_;
        quint64 position_;
        SimpleCrypt crypto_;
        QString fileTag_;

        QMap<QString,OpenedFile> openedFileMap_;

        void addOpenedFile(const QString& path,OpenedFile& openedFile);
        void removeAndCloseOpenedFile(const QString& path);
        //may return a null OpenedFile if is not open
        OpenedFile getOpened(const QString& path);
        bool isFileOpen(const QString& path);
        CurrentFile currentFile_;

    };//FileSender class

    /**
     * @brief Response
     * @param cuser
     */
    Response(ConnectedUser *cuser);
    Response &addExtras(const QString &name, int value);
    Response &addExtras(const QString &name, bool value);
    Response &addExtras(const QString &name, float value);
    Response &addExtras(const QString &name, double value);
    Response &addExtras(const QString &name, long value);
    Response &addExtras(const QString &name, unsigned long value);
    Response &addExtras(const QString &name, qint64 value);
    Response &addExtras(const QString &name, quint64 value); //unsigned long long
    Response &addExtras(const QString &name, const QString &value);
    Response &addExtras(const QString &name, const QByteArray &value);
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
    Response &addExtras(const char *name, unsigned long value);
    Response &addExtras(const char *name, qint64 value);
    Response &addExtras(const char *name, quint64 value);//unsigned long long
    Response &addExtras(const char* name, const QJsonObject &object);
    Response &addExtras(const char* name, const QJsonArray &array);
    Response &addExtras(const char* name, const QStringList &list);

    int getExtrasCount();
    Response &removeExtras(const QString &name);
    Response &removeExtras(const char* name);
    bool containsExtras(const QString& name);
    bool containsExtras(const char* name);
    void clear();
    QString controller();
    Response& setController(const QString &controller);
    QString action();
    Response& setAction(const QString &action);
    QJsonObject getJsonObjectExtras(const QString &name);
    QJsonArray getJsonArrayExtras(const QString &name);
    QJsonObject getJsonObjectExtras(const char* name);
    QJsonArray getJsonArrayExtras(const char* name);
    QVariantList getVariantListExtras(const QString& name);

    /*return true if the data is sent and set the last error to ResponseError::NoError
    else return false, and set the last error,this function also clear extras
    and action but retain controller*/
    bool send();
    /*return true if the data is sent and set the last error to ResponseError::NoError
    else return false, and set the last error,unlike send(),this function also clear all data
    when the data is sent*/
    bool send(MagabeData &data);
    //return the last error
    ResponseError& lastError();
    FileSender&  fileSender();

private:
    friend class FileSender;
    ConnectedUser *cuser_;
    MagabeData data_;
    ResponseError responseError_;
    FileSender fileSender_;
    bool queueBypassSend();


};

typedef Response::ResponseError ResponseError;

typedef Response::FileSender::FileSenderError FileSenderError;

class AppUsers;
/**  ---------------------------
 * @brief The ConnectedUser struct
 */
struct ConnectedUser{

    ConnectedUser(const User& user);
    ~ConnectedUser();
    bool isRegistered();
    bool closeConnection();
    QString firstName() const;
    QString lastName() const;
    QString dateOfBirth() const;
    User::Gender gender() const;
    QString username() const;
    QString authKey() const;
    QString password() const;
    QString email() const;
    bool isBlackListed() const;
    qint64 createdAt() const;
    qint64 updatedAt() const;
    QString profilePicturePath() const;
    QString education() const;
    QString position() const;
    QString location() const;
    QString phoneNumber() const;
    QString getThisClientMgbSessionId() const;
    unsigned long id() const;
    bool thisClientConnectionIsActive();
    bool updatePhoneNumber(const QString &phoneNumber);
    bool updateLocation(const QString &location);
    bool updatePosition(const QString &position);
    bool updateEducation(const QString &education);
    bool updateProfilePicturePath(const QString &profilePicturePath);
    bool updateIsBlackListed(bool isBlackListed);
    bool updateEmail(const QString &email);
    bool updatePassword(const QString &password);
    bool updateUsername(const QString &username);
    bool updateGender(const User::Gender &gender);
    bool updateDateOfBirth(const QString &dateOfBirth);
    bool updateFirstName(const QString &firstName);
    bool updateLastName(const QString &lastName);
    bool updateAuthKey(const QString &authKey);
    QString generateAuthKey();
    bool isClientAuthenticated();
    bool hasDataToSend();
    QString getJsonUserInfo();

    User::ConnectionState getConnectionState();
    QString getConnectionStateHuman();
    bool logOut();
    //return true if invocked sucessfull
    bool logIn(const QString &username, const QString &password, Errors::AuthenticationError * const authenticatinError);
    bool authenticate(const QString& username, const QString& authKey, const QString& password
                      , Errors::AuthenticationError * const authenticatinError, QString * const newAuthKey
                      , bool checkAuthKey);
    static  QString authenticationErrorToString(Errors::AuthenticationError authenticationError);
    void setAppUsers(AppUsers *appUsers);

    Response& response();

private:
    friend struct Response;
    friend class FileSender;
    UserObjectEditor *editor_;
    User user_;
    Response *response_;
    AppUsers *appUsers_;
    //please pass a valid poiter,does not check if the passed one its null
    friend UserObjectEditor& getUserObjectEditor(ConnectedUser *cuser);
    bool updateIsActive(bool isActive);
    void send(MagabeData &data);
    //send the data immediatry and bypass the queue
    //used for files
    void queueBypassSend(MagabeData &data);
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
class ServerFiles;
/**   --------------------------Controller--------------------
 * @brief The Controller class
 */
class Controller : public QObject
{
    Q_OBJECT

public:

    explicit Controller(QObject *parent);
    virtual ~Controller();
    virtual Controller* createNew()=0;
    void print(QString status);
    QString name() const;
    void setName(const QString &name);//the name must be the same as the class name
    bool enabled() const;
    void setEnabled(bool enabled);
    QString getTimestamp();
    void printStatus();
    //only called by controller indentifier , do not call it
    static void performAction(Controller* controller,MagabeData data);//data must be a copy
    void wait(unsigned long secs);
    QString getCurrentExecutingAction() const;

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
    void initFileUploading(const QString &controller, const QString &action, const QString &callbackController, const QString &callbackAction, const QString &fileLocation, const QString &mgbSessionId);

    static bool isSignalRegistered(const QString&  className,const QString&  signalName);
    static bool isSignalRegistered(const QString&  signalName);

    static bool emitSignal(const QString&  senderClassName,const QString&  signalName,
                           QGenericArgument val0 = QGenericArgument(Q_NULLPTR),
                           QGenericArgument val1 = QGenericArgument(),
                           QGenericArgument val2 = QGenericArgument(),
                           QGenericArgument val3 = QGenericArgument(),
                           QGenericArgument val4 = QGenericArgument(),
                           QGenericArgument val5 = QGenericArgument(),
                           QGenericArgument val6 = QGenericArgument(),
                           QGenericArgument val7 = QGenericArgument(),
                           QGenericArgument val8 = QGenericArgument(),
                           QGenericArgument val9 = QGenericArgument());

    static bool connectSlot(const QString&  senderClassName,const QString&  signalName,
                            const QObject *receiver,const QString&  slotName);

    static bool connectSignals(const QString&  senderClassName,const QString&  senderSignalName,
                               const QString&  receiverClassName,const QString&  receiverSignalName);

    //return names of classes thet contain that signal
    static QStringList getSignalClassNames(const QString &signalName);
signals:
    void closeClientSocket(QString mgbSessionId);

private slots:
    virtual void triggerAction(MagabeData data);
    void onPerformActionFinished();
    //For Actions registrations please call macro
    virtual void registerActions();

protected:
    void triggerActionDoContinue(MagabeData& data);
    void printActionTriggered(const QString &action);
    bool isRegisteredAction(const QString& action);
    void registerAction(const QString &action,const int enumValue);
    int getIndexOfControllerMethod(const QString& methodName);
    ConnectedUser& user()const;
    AppUsers& appUsers();
    ServerFiles& serverFiles();
    QString currentExecutingAction_;

private:
    friend class ControllerIdentifier;
    friend class User;
    friend class MESAE_Controllers;
    friend class MESAE_Controller;

    QString name_;
    bool enabled_;
    //do not change names,it is a design choice
    ConnectedUser *user_;
    AppUsers *appUsers_;
    ServerFiles *serverFiles_;
    QMap<QString, int>  actions_; //actios and their values
    QMap<QString,int> methods_;

    void printActionRegistrationError();

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
    void registerControllerMethods(Controller* controller);
};


/**
 * @brief The AppUsers struct
 */
struct AppUsers{
    bool registerNewUser(const QString &firstName, const QString &lastName, const QString &dateOfBirth,
                         const User::Gender gender, const QString &username, const QString &password, const QString &email,
                         const QString &profilePicturePath, const QString &education, const QString & position,
                         const QString &location, const QString &phoneNumber);

    User getUser(unsigned long userId);
    User getUser(const QString &username);
    bool userRegistered(QString username);
    bool userRegistered(unsigned long userId);
    bool isValidEmail(const QString & email);
    bool isEmailTaken(const QString &email);
    bool isClientAuthenticated(const QSslSocket* clientSocket);
    bool isClientAuthenticated(const QString &mgbSessionId);
    bool closeClientConnetion(const QString &mgbSessionId);
    bool closeClientConnetion(const QSslSocket* clientSocket);
    QString generateFilePath(const QString &folderName,const QString &fileName);
    int getTotal();
    const QString getMgbSessionId(const QSslSocket *clientSocket);
    QString getUserName(unsigned long userId);
    unsigned long getUserId(const QString &username);

private:
    UserModel userModel_;
    FileUtils fileUtils_;
};


/*--------------------------------*/
/**
 * @brief The ServerFilesSignals class
 */
class ServerFilesSignals: public QObject
{
    Q_OBJECT

signals:
    void newFileUploaded(unsigned long userId,QString filePath);
public:
    ServerFilesSignals(QObject *parent = nullptr)
        : QObject(parent){}//do not move to cpp file;cause moc problems
};

/**
 * @brief The ServerFiles class
 */
class ServerFiles
{
public:
    ServerFiles();
    ~ServerFiles();

    bool isFileOwner(unsigned long userId, const QString &fileNameOrAlias);
    QString getFilePath(const QString &fileNameOrAlias);
    bool fileExists(const QString &fileNameOrAlias);
    bool deleteFile(unsigned long userId, const QString &fileNameOrAlias);
    //record newFilePath if succesfull or an empty string
    QString recordFile(unsigned long userId, const QString &filePath);
    QString getFileExtension(const QString &filePath);
    QString getFileBaseName(const QString &filePath);
    QString getFileFolder(const QString &filePath);
    QString getAppRootPath();
    bool fileExistsOnDisk(const QString &filePath);
    qint64 getFileSize(const QString &filePath);
    QString getFileName(const QString &filePath);
    bool copyFile(const QString &currentPath,const QString &newPath);
    QString generateAppRootFilePath(const QString &fileName);
    QString generateFilePath(const QString &folderName, const QString &fileName);
private:
    QString generateAlias(unsigned long userId,const QString &fileExtension);
    ServerFilesModel serverFilesModel_;
    ServerFilesSignals serverFilesSignals_;
};



/*--------------------------------------------------------------------*/


#define FILE_UPLOAD_SETUP_ACTION(FILESENDER_CONTROLLER,FILESENDER_ACTION \
    ,CALLBACK_CONTROLLER,CALLBACK_ACTION,SAVE_FOLDER \
    ,UPLOAD_PROGRESS_CALLBACK,ON_ERROR_CALLBACK,ON_FILE_ALREADY_UPLOADED_CALLBACK) \
    QString fileSenderController = FILESENDER_CONTROLLER; \
    QString fileSenderAction = FILESENDER_ACTION; \
    QString saveFolder = SAVE_FOLDER; \
    QString callbackController = CALLBACK_CONTROLLER; \
    QString callbackAction = CALLBACK_ACTION; \
    std::function<void(QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  uploaded,quint64 fileSize)> uploadProgressCallback = UPLOAD_PROGRESS_CALLBACK; \
    std::function<void(int errorNo,QString errorString)> onErrorCallback = ON_ERROR_CALLBACK; \
    /*(in onFileAlreadyUploadedCallback) return true to reupload or false to exit execution*/\
    std::function<bool(QString filePath,QString fileTag)> onFileAlreadyUploadedCallback = ON_FILE_ALREADY_UPLOADED_CALLBACK; \
    QString errorString = "No Error";\
    int errorNo = 0;\
    if(!uploadProgressCallback){\
    uploadProgressCallback = [=]\
    (QString fileBaseName,QString fileExtension,QString fileLocation,float percentageCompletion,quint64  uploaded,quint64 fileSize){\
    Q_UNUSED(uploaded)\
    Q_UNUSED(fileBaseName)\
    Q_UNUSED(fileExtension)\
    Q_UNUSED(fileLocation)\
    auto status = QStringLiteral("uploading... ")+QString("%1.%2 [%3% of %4 MB]")\
    .arg(fileBaseName).arg(fileExtension).arg(percentageCompletion).arg((float)fileSize/(1024*1024));\
    qDebug() << status;\
    };\
    }\
    if(!onErrorCallback){\
    onErrorCallback = [=](int errorNo,QString errorString){\
    qWarning() << "errorNo#"<< errorNo <<" | "<< errorString;\
    };\
    }\
    if(callbackController.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> callbackController is not set !";\
    errorNo = 1;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    return;\
    }\
    if(callbackAction.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> callbackAction is not set !";\
    errorNo = 2;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    return;\
    }\
    if(fileSenderController.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> fileSenderController is not set !";\
    errorNo = 3;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    return;\
    }\
    \
    if(fileSenderAction.isEmpty()){\
    errorString = getCurrentExecutingAction() +"::Error -> fileSenderAction is not set !";\
    errorNo = 4;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    return;\
    }\
    if(saveFolder.isEmpty()){\
    errorString = getCurrentExecutingAction() + "::Error -> saveFolder is not set !";\
    errorNo = 5;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    return;\
    }\
    bool uploadCompleted = false;\
    QString fileTag;\
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
    QString  fDir = appDir_.homePath();/*i use home path bcoz its a server*/\
    fDir.append(QString("/.%1/%2").arg(APP_FOLDER).arg(fileFolder));\
    appDir_.setPath(fDir);\
    if(!appDir_.exists()){\
    if(!appDir_.mkpath(fDir)){\
    errorString = getCurrentExecutingAction() + "::Error ->  Failed to create folder ;- " + fDir;\
    errorNo = 6;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
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
    if(onFileAlreadyUploadedCallback){\
    if(onFileAlreadyUploadedCallback(filePath,fileTag)){\
    fullFile.remove();\
    goto checkIfFullFileExists;\
    }else{\
    uploadCompleted = true;\
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
    uploadCompleted = true;\
    errorString = getCurrentExecutingAction() +     QString("::Error ->  File already uploaded to path ;- %1 and ON_FILE_ALREADY_UPLOADED_CALLBACK is not set").arg(filePath);\
    errorNo = 10;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
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
    chunkSize =  chunkfile.getChunkSize();\
    fileSize =  chunkfile.getFileSize();\
    senderFpos =  chunkfile.getSenderFpos();\
    receiverFpos =  chunkfile.getReceiverFpos();\
    fileLocation =  chunkfile.getFileLocation();\
    bytesStillAvailable =  chunkfile.getBytesStillAvailable();\
    fileSender =  chunkfile.getSenderUsername();\
    fileReceiver =  chunkfile.getReceiverUsername();\
    if(!chunkfile.isValid()){\
    MESAE_exitExecution();\
    }\
    Response::FileSender::FileSenderError senderError((Response::FileSender::FileSenderError::ErrorType)extras.getIntExtras(Response::FileSender::ExtrasNames::FileSenderErrorExtras));\
    if(senderError != Response::FileSender::FileSenderError::ErrorType::NoError){\
    errorString = getCurrentExecutingAction() + "::FileSenderError -> "  + senderError.toString();\
    errorNo = 9;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    MESAE_exitExecution();\
    }\
    if(position != receiverFpos){\
    if((!receivedBytes.isEmpty()) && file.open(QFile::Append)){\
    file.write(receivedBytes,receivedBytes.size());\
    file.close();\
    }\
    receivedBytes.clear();\
    position = file.size();\
    goto continueUploading;\
    }\
    if(chunkSize > 0){\
    receivedBytes.append(chunkfile.getChunkBytes());\
    position = senderFpos;\
    float percentagecompletion = ((float)position/fileSize)*100;\
    if(uploadProgressCallback){\
    uploadProgressCallback(fileBaseName,fileExtension,fileLocation,percentagecompletion,position,fileSize);\
    }\
    if(((quint64)receivedBytes.size() == fileSize) || (!bytesStillAvailable)){\
    goto saveFile;\
    }\
    }\
    if(receivedBytes.size() >= (1024*1024)){\
{saveFile:\
    if(!file.open(QFile::Append)){\
    errorString =  getCurrentExecutingAction() + "::Error -> Failed to open :: " + tmpfPath + " for serving";\
    errorNo = 7;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    MESAE_exitExecution();\
    }\
    qint64 byteWrittern = file.write(receivedBytes,receivedBytes.size());\
    if( byteWrittern != -1){ \
    receivedBytes.clear();\
    file.close();\
    position = file.size();\
    if(position == fileSize){\
    QFile::rename(tmpfPath.toLatin1(),filePath.toLatin1()); \
    MESAE_PauseThread(0.5);/*Wait for 0.5sec For fileSystem to update prevent not sent errors*/\
    uploadCompleted = true;\
    user().response().setController(fileSenderController)\
    .setAction(fileSenderAction)\
    .addExtras(MFile::ExtrasNames::fileTagExtras,fileTag)\
    .addExtras(Response::FileSender::ExtrasNames::callbackControllerExtras,callbackController)\
    .addExtras(Response::FileSender::ExtrasNames::callbackActionExtras,callbackAction)\
    .addExtras(Response::FileSender::ExtrasNames::fileLocationExtras,fileLocation)\
    .addExtras(Response::FileSender::ExtrasNames::filePositionExtras,position)\
    .addExtras(Response::FileSender::ExtrasNames::fileReceivedExtras,uploadCompleted)\
    .send();\
    MESAE_exitExecution();\
    }else{\
    goto continueUploading;\
    }\
    }else{\
    file.close();\
    position = senderFpos;\
    errorString =  getCurrentExecutingAction() + "::Error -> Failed to write to file";\
    errorNo = 8;\
    if(onErrorCallback)onErrorCallback(errorNo,errorString);\
    }\
    }\
    }\
{continueUploading:\
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
