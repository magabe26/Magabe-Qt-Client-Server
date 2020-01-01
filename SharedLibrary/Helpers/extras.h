/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef EXTRAS_H
#define EXTRAS_H
#include <QByteArray>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>

class Extras
{
public:
    Extras();
    explicit  Extras(const QByteArray& extras);
    ~Extras(){}

    Extras& addExtras(const QString &name, const QString &value);
    Extras& addExtras(const QString &name, int value);
    Extras& addExtras(const QString &name, bool value);
    Extras& addExtras(const QString &name, float value);
    Extras& addExtras(const QString &name, double value);
    Extras& addExtras(const QString &name, long value);
    Extras& addExtras(const QString &name, qint64 value);
    Extras& addExtras(const QString &name,const char *value);
    Extras& addExtras(const QString &name, unsigned long long value);
    Extras& addExtras(const QString &name, unsigned long value);
    Extras& addExtras(const QString &name, const QByteArray& value);
    Extras& addExtras(const QString &name, const QJsonObject &object);
    Extras& addExtras(const QString &name, const QJsonArray &array);
    Extras& addExtras(const QString &name, const QStringList &list);

    /* const char* versions */
    Extras& addExtras(const char* name, const char* value);
    Extras& addExtras(const char* name, const QByteArray& value);
    Extras& addExtras(const char* name, const QString &value);
    Extras& addExtras(const char* name, int value);
    Extras& addExtras(const char* name, bool value);
    Extras& addExtras(const char* name, float value);
    Extras& addExtras(const char* name, double value);
    Extras& addExtras(const char* name, long value);
    Extras& addExtras(const char* name, qint64 value);
    Extras& addExtras(const char* name, unsigned long long value);
    Extras& addExtras(const char* name, unsigned long value);
    Extras& addExtras(const char* name, const QJsonObject &object);
    Extras& addExtras(const char* name, const QJsonArray &array);
    Extras& addExtras(const char* name, const QStringList &list);

    //quint64
    unsigned long long getULongLongExtras(const QString &name);
    //ulong
    unsigned long getULongExtras(const QString &name);
    QString getStringExtras(const QString &name);
    int getIntExtras(const QString &name);
    bool getBooleanExtras(const QString &name);
    float getFloatExtras(const QString &name);
    double getDoubleExtras(const QString &name);
    long getLongExtras(const QString &name);
    /*returns an empty QJsonObject if the name does not exists*/
    QJsonObject getJsonObjectExtras(const QString &name);
    /*returns an empty QJsonArray if the name does not exists*/
    QJsonArray getJsonArrayExtras(const QString &name);

    Extras& removeExtras(const QString &name);

    /*char * versions*/
    unsigned long long getULongLongExtras(const char* name);
    unsigned long getULongExtras(const char* name);
    QByteArray getByteArrayExtras(const char *name);
    QString getStringExtras(const char* name);
    int getIntExtras(const char* name);
    bool getBooleanExtras(const char* name);
    float getFloatExtras(const char* name);
    double getDoubleExtras(const char* name);
    long getLongExtras(const char* name);
    Extras &removeExtras(const char* name);
    /*returns an empty QJsonObject if the name does not exists*/
    QJsonObject getJsonObjectExtras(const char* name);
    /*returns an empty QJsonArray if the name does not exists*/
    QJsonArray getJsonArrayExtras(const char* name);

    int getExtrasCount();

    bool containsExtras(const QString& name);
    bool containsExtras(const char* name);

    QString getDataId();

    bool operator ==(const Extras &rhs);
    bool operator !=(const Extras &rhs);

    void operator =(const Extras &rhs);

    Extras &setMgbSessionId(const QString &mgbSessionId);
    QString getMgbSessionId();
    bool isValid();
    bool isEmpty();
    void clear();
    QByteArray toByteArray();
    void setData(const QByteArray &data);
    QByteArray getJsonExtras();
    QVariantList getVariantListExtras(const QString& name);

protected:
    const static QString CONTROLLER_SIGNATURE;
    const static QString ACTION_SIGNATURE;
    const static QString NON;
    QString getElement(const QString& name,const QChar identifier);
    void addElement(const QString& name,const QChar identifier,const QString& value);

private:
    const static QString EXTRAS_SIGNATURE;
    const static QString EXTRAS_COUNT_SIGNATURE;
    const static QString ID_SIGNATURE;
    const static QString MGB_SESSION_ID_SIGNATURE;

    QByteArray data_;
    bool containsExtras(const char *name, int *valueIndex, int *valueLength, int *extrasStartIndex, int *extrasEndIndex);
    bool containsElement(const char* name,const QChar identifier, int* valueIndex, int* valueLength,
                         int* extrasStartIndex, int* extrasEndIndex);
    void init();
    QString generateId();
    void incrementExtrasCount();
    void decrementExtrasCount();

};

//for making extras keys
#define NEW_EXTRAS_NAME(name) constexpr static const char* name##Extras = #name;
//only use this when passing empty strings to addExtras, do not pass empty literal strings
//passing empty literal string set the value to "1" not ""
#define EMPTY_STRING_EXTRAS QString()

#ifndef TO_CSTR
#define TO_CSTR(name) #name
#endif
#define NEW_ACTION_NAME(name) constexpr static const char* name##ActionName = TO_CSTR(Action##name);


#endif // EXTRAS_H
