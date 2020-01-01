/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <QString>

const QString TEMPORARY_FILE_EXTENSION = QStringLiteral(".mgbTempFile");

const int MAX_MAGABEDB_OPEN_ATTEMPTS = 10;

#ifdef MGB_SERVER
const QString APP_NAME = QStringLiteral("MagabeServerApp");
const QString APP_FOLDER = QStringLiteral("magabeServer");
const QString DB_NAME = QStringLiteral("MagabeServer.mgbdb");

#elif defined(MGB_CLIENT)
const QString APP_NAME = QStringLiteral("MagaClientApp");
const QString APP_FOLDER = QStringLiteral("magabeClient");
const QString DB_NAME = QStringLiteral("MagabeClient.mgbdb");

#endif

#endif // CONSTANTS_H
