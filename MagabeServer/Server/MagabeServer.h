/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CHURASERVER_H
#define CHURASERVER_H

#include <QObject>
#include "sslserver.h"
#include <QSslSocket>
#include <QSslKey>
#include "Helpers/datawritter.h"
#include <QQueue>
#include <QThreadPool>
#include <QThread>
#include "Helpers/datafilter.h"
#include "ControllerIdentifier/controlleridentifier.h"

class DataProcessor;
class IncomingDataManager;


/**
 * @brief The MagabeServer class
 */
class MagabeServer : public QObject
{
    Q_OBJECT
    
public:
    explicit MagabeServer(ControllerIdentifier *controllerIdentifier,QObject *parent);
    ~MagabeServer();
    void setMaxThreadCount(int maxThreadCount);

signals:
    void serverStarted(QHostAddress address, quint16 port);
    void serverStoped();
    
public slots:
    void newConnection();
    void disconnected();
    void aboutToClose();
    void bytesWritten(qint64 bytes);
    void readChannelFinished();
    void startServer(QHostAddress ipAddress, quint16 port);
    void setCertificateAndKeyFile();
    // Slots to receive signals from sockets
    void handshakeComplete();
    void sslErrors(const QList<QSslError> &errors);
    void connectionFailure();
    void stopServer();
    void onAppAboutToQuit();

private slots:
    void onStartAuthenticaion(QString mgbSessionId);
    void onCloseClientSocket(QString mgbSessionId);
private:
    const QSslKey *key_;
    const QSslCertificate *certificate_;
    SslServer *sslServer_;
    const QString invalidFileMsg_;
    DataProcessor *dataProcessor_;
    IncomingDataManager *incomingDataManager_ ;

    void checkFileStatus(QString KeyFile, QString certificateFile);
    void print(QString text);

};



/**---------------DataProcessor----------------------
 * @brief The DataProcessor class
 */
class DataProcessor : public QObject,public QRunnable
{
    Q_OBJECT

public:
    explicit DataProcessor(ControllerIdentifier *controllerIdentifier);
    void run();
    void start();
    void stop();
    DataFilter *dataFilter() const;

signals:
public slots:
    void startDataProcessing(const QByteArray &data, const QString mgbSessionId);
private:
    DataFilter*  dataFilter_;
    bool running_;
    ControllerIdentifier *controllerIdentifier_;
};


/** --------------IncomingDataManager----------------------
 * @brief The IncomingDataManager class
 */
class  IncomingDataManager:public QThread
{
    Q_OBJECT

public:
    struct ReceivedData{
        QByteArray data_;
        QString mgbSessionId_;
    };

    IncomingDataManager(DataProcessor* dataProcessor,QObject *parent);
    void connectSignalToClient(QSslSocket *client );
    // QThread interface
    void stop();
protected:
    void run();
public slots:
    void onMagabeServerReadyRead();
private:
    DataFilter* filiter_;
    QQueue<ReceivedData> receivedData_;
    QMutex mutex_;
    QWaitCondition dataReceiced_;
    DataProcessor* dataProcessor_;
    bool running_;
};



#endif // CHURASERVER_H
