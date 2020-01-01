/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include <QSslSocket>
#include "sslserver.h"

// Intercept new socket connection and enable SSL
void SslServer::incomingConnection(int socketDescriptor)
{
  QSslSocket *serverSocket = new QSslSocket();
  if (serverSocket->setSocketDescriptor(socketDescriptor))
  {
    addPendingConnection (serverSocket);
  }
  else
  {
    delete serverSocket;
  }
}

