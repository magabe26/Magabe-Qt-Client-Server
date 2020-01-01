/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef MFILE_H
#define MFILE_H
#include "magabedata.h"
#include "Helpers/simplecrypt.h"

struct MFile{
    struct ExtrasNames{
        NEW_EXTRAS_NAME(chunkBytes)
        NEW_EXTRAS_NAME(chunkSize)
        NEW_EXTRAS_NAME(fileSize)
        NEW_EXTRAS_NAME(fileBaseName)
        NEW_EXTRAS_NAME(fileExtension)
        NEW_EXTRAS_NAME(senderFpos)
        NEW_EXTRAS_NAME(receiverFpos)
        NEW_EXTRAS_NAME(fileLocation)
        NEW_EXTRAS_NAME(bytesStillAvailable)
        NEW_EXTRAS_NAME(senderUsername)
        NEW_EXTRAS_NAME(receiverUsername)
        NEW_EXTRAS_NAME(fileTag)
    };
    MFile(Extras& extras);
    MFile(MagabeData& data);
public:
    QByteArray getChunkBytes();
    quint64 getChunkSize() const;
    quint64 getFileSize() const;
    QString getFileBaseName() const;
    QString getFileExtension() const;
    quint64 getSenderFpos() const;
    quint64 getReceiverFpos() const;
    QString getFileLocation() const;
    bool getBytesStillAvailable() const;
    QString getSenderUsername() const;
    QString getReceiverUsername() const;
    bool isValid() const;

private:
    QByteArray chunkBytes;
    quint64  chunkSize = 0;
    quint64 fileSize = 0;
    QString  fileBaseName="";
    QString  fileExtension ="";
    quint64 senderFpos;
    quint64  receiverFpos;
    QString fileLocation;
    bool bytesStillAvailable = false;
    QString senderUsername;
    QString receiverUsername;
    bool initMagabeData(Extras& extras);
    bool isValid_ = false;
    SimpleCrypt crypto_;
    QByteArray decryptAndUncompress(const QString data);
    QString compressAndEncrypt(const QByteArray &fileByteArray);
};


#endif // MFILE_H
