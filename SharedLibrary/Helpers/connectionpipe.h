/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef CONNECTIONPIPE_H
#define CONNECTIONPIPE_H
#include <QObject>
#include <QString>

class ConnectionPipe
{
public:
    ConnectionPipe();

    static  void registerObject(const QObject *object);
    static  void unRegisterObject(const QObject *object);

    static  bool isSignalRegistered(const QString &className, const QString &signalName);
    static  bool isSignalRegistered(const QString &signalName);

    static  bool emitSignal(const QString &senderClassName, const QString &signalName,
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

    static  bool connectSlot(const QString &senderClassName, const QString &signalName, const QObject *receiver, const QString &slotName);
    static  bool connectSlot(const QObject *sender, const QString &signalName, const QObject *receiver, const QString &slotName);

    static  QStringList getSignalClassNames(const QString &signalName);

    static  bool connectSignals(const QString &senderClassName, const QString &senderSignalName, const QString &receiverClassName, const QString &receiverSignalName);
    static  bool connectSignals(const QString &senderClassName, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName);
    static  bool connectSignals(const QObject *sender, const QString &senderSignalName, const QObject *receiver, const QString &receiverSignalName);
};

#endif // CONNECTIONPIPE_H
