/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CHURACLIENT_H
#define CHURACLIENT_H

#include <QSslSocket>
#include <QTimer>
#include <sstream>
#include "Helpers/magabedata.h"
#include <QThreadPool>
#include <QThread>
#include <QQueue>
#include "Helpers/datafilter.h"
#include "ControllerIdentifier/controlleridentifier.h"

class DataProcessor;
class  IncomingDataManager;

class MagabeClient : public QObject
{
    Q_OBJECT

public:
    explicit MagabeClient(ControllerIdentifier *controllerIdentifier,QObject *parent);
    ~MagabeClient();
    void setMaxThreadCount(int maxThreadCount);
    void connectToMagabeServer(QString ipAddress, quint16 port, int msecsTimeout);
signals:
    void sendToDataProcessor(QString receivedData);
    void connectedToMagabeserver();
    void disconnectedFromMagabeServer();
    void connectTimeout();

public slots:
    void onConnectToServer(QString ip,QString port,int msecsTimeout);
    void disconnect();
    void onAppAboutToQuit();

private slots:
    void print(QString text);
    void connected();
    void disconnected();
    void hostFound();
    void bytesWritten(qint64 bytes);
    // void readyRead();
    void showError(QAbstractSocket::SocketError e);
    void sslErrors(const QList<QSslError> &errors);
    bool isValidCertificate();
private:
    QSslSocket *socket_;
    DataProcessor *dataProcessor_;
    IncomingDataManager *incomingDataManager_ ;

};

/**
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
    void startDataProcessing(const QByteArray &data);
private:
    DataFilter*  dataFilter_;
    bool running_;
    ControllerIdentifier *controllerIdentifier_;
};


/**
 * @brief The IncomingDataManager class
 */
class  IncomingDataManager:public QThread
{
    Q_OBJECT

public:
    IncomingDataManager(DataProcessor* dataProcessor,QObject *parent);
    void connectSignalToSocket(QSslSocket *socket );
    void stop();

    // QThread interface
protected:
    void run();

public slots:
    void onMagabeClientReadyRead();
private:
    DataFilter* filiter_;
    QQueue<QByteArray> receivedData_;
    QMutex mutex_;
    QWaitCondition dataReceiced_;
    DataProcessor* dataProcessor_;
    bool running_;
};


#endif // CHURACLIEBTDIALOG_H
