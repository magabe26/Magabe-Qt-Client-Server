/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef SSLSERVER_H
#define SSLSERVER_H

#include <QSslError>
#include <QTcpServer>

// QTcpServer extension to intercept new socket connection and enable SSL
class SslServer : public QTcpServer
{
  Q_OBJECT

public:
  virtual void incomingConnection(int socketDescriptor);
};

#endif // SSLSERVER_H

