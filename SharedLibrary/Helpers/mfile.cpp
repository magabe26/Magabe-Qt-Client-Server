/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "mfile.h"
#include <QDebug>

MFile::MFile(Extras &extras):
    crypto_(Q_UINT64_C(0xbd4f9fefd46f3a27))
{
    isValid_ = initMagabeData(extras);

}

MFile::MFile(MagabeData &data):
    crypto_(Q_UINT64_C(0xbd4f9fefd46f3a27))
{
    isValid_ = initMagabeData(data);
}

QByteArray MFile::getChunkBytes()
{
    return chunkBytes;
}

quint64 MFile::getChunkSize() const
{
    return chunkSize;
}

quint64 MFile::getFileSize() const
{
    return fileSize;
}

QString MFile::getFileBaseName() const
{
    return fileBaseName;
}

QString MFile::getFileExtension() const
{
    return fileExtension;
}

quint64 MFile::getSenderFpos() const
{
    return senderFpos;
}

quint64 MFile::getReceiverFpos() const
{
    return receiverFpos;
}

QString MFile::getFileLocation() const
{
    return fileLocation;
}

bool MFile::getBytesStillAvailable() const
{
    return bytesStillAvailable;
}

QString MFile::getSenderUsername() const
{
    return senderUsername;
}

QString MFile::getReceiverUsername() const
{
    return receiverUsername;
}

bool MFile::initMagabeData(Extras &extras){
    chunkBytes.clear();
    chunkSize = extras.getULongLongExtras(ExtrasNames::chunkSizeExtras);

    if(chunkSize > 0){
        chunkBytes.append(decryptAndUncompress(extras.getByteArrayExtras(ExtrasNames::chunkBytesExtras)));
    }

    fileSize = extras.getULongLongExtras(ExtrasNames::fileSizeExtras);
    fileBaseName = extras.getStringExtras(ExtrasNames::fileBaseNameExtras);
    fileExtension = extras.getStringExtras(ExtrasNames::fileExtensionExtras);
    senderFpos = extras.getULongLongExtras(ExtrasNames::senderFposExtras);
    receiverFpos = extras.getULongLongExtras(ExtrasNames::receiverFposExtras);
    fileLocation = extras.getStringExtras(ExtrasNames::fileLocationExtras);
    bytesStillAvailable = extras.getBooleanExtras(ExtrasNames::bytesStillAvailableExtras);
    senderUsername = extras.getStringExtras(ExtrasNames::senderUsernameExtras);
    receiverUsername = extras.getStringExtras(ExtrasNames::receiverUsernameExtras);

    if(chunkBytes.isEmpty() && (chunkSize > 0)){
        qDebug() << "MFile::initMagabeData ERROR " <<"extras contains NO BYTES";
        return false;
    }else{
        return true;
    }

}

QByteArray MFile::decryptAndUncompress(const QString data)
{
    //only way it works
    QByteArray decryted = crypto_.decryptToByteArray(data);
    return qUncompress(decryted);
}

QString MFile::compressAndEncrypt(const QByteArray &fileByteArray)
{    //only way it works
    QByteArray compressedData(qCompress(fileByteArray));
    QString BinaryData(crypto_.encryptToString(compressedData));
    return  BinaryData;
}

bool MFile::isValid() const
{
    return isValid_;
}
