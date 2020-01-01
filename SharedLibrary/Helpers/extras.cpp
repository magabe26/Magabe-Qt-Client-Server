/**
 * Copyright 2017-2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */



#include "extras.h"
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QDateTime>

//the class works dont change anything

const QString Extras::CONTROLLER_SIGNATURE = QStringLiteral("Controller");
const QString Extras::ACTION_SIGNATURE = QStringLiteral("Action");
const QString Extras::NON = QStringLiteral("NON");
const QString Extras::EXTRAS_SIGNATURE = QStringLiteral("Extras");
const QString Extras::EXTRAS_COUNT_SIGNATURE = QStringLiteral("ExtrasCount");
const QString Extras::ID_SIGNATURE = QStringLiteral("ExtrasId");
const QString Extras::MGB_SESSION_ID_SIGNATURE = QStringLiteral("MgbSessionId");


Extras::Extras(){init();}

Extras::Extras(const QByteArray &extras){
    setData(extras);
}

Extras &Extras::addExtras(const QString &name, const QString &value){
    return addExtras(name.toStdString().c_str(),value.toStdString().c_str());
}

Extras &Extras::addExtras(const QString &name, int value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, bool value){
    addExtras(name, (value ? 1 : 0));
    return *this;
}

Extras &Extras::addExtras(const QString &name, float value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, double value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, long value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, qint64 value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, const char *value){
    addExtras(name.toStdString().c_str(),value);
    return *this;
}

Extras &Extras::addExtras(const QString &name, unsigned long long value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, unsigned long value){
    addExtras(name,QString::number(value));
    return *this;
}

Extras &Extras::addExtras(const QString &name, const QByteArray &value){
    addExtras(name.toStdString().c_str(),value);
    return *this;
}

Extras &Extras::addExtras(const QString &name, const QJsonObject &object)
{
    return addExtras(name.toStdString().c_str(),object);
}

Extras &Extras::addExtras(const QString &name, const QJsonArray &array)
{
    return addExtras(name.toStdString().c_str(),array);
}

Extras &Extras::addExtras(const QString &name, const QStringList &list)
{
    if(!list.isEmpty()){
        addExtras(name,QJsonArray::fromStringList(list));
    }
    return *this;
}

Extras &Extras::addExtras(const char *name, const char *value){
    if(strlen(name) > 0){
        removeExtras(name);//remove if exists
        data_.append(QString("%1|E%2|:|").arg(name).arg(value));
        incrementExtrasCount();
    }
    return *this;
}

Extras &Extras::addExtras(const char *name, const QByteArray &value){
    addExtras(name,value.data());
    return *this;
}

Extras &Extras::addExtras(const char *name, const QString &value){
    addExtras(name,value.toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, int value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, bool value){
    addExtras(name,(value)? 1 : 0);
    return *this;
}

Extras &Extras::addExtras(const char *name, float value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, double value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, long value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, qint64 value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, unsigned long long value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, unsigned long value){
    addExtras(name,QString::number(value).toStdString().c_str());
    return *this;
}

Extras &Extras::addExtras(const char *name, const QJsonObject &object)
{
    QJsonDocument doc(object);
    if((!doc.isEmpty()) && (!doc.isNull())){
        addExtras(name,doc.toJson().data());
    }
    return *this;
}

Extras &Extras::addExtras(const char *name, const QJsonArray &array)
{
    QJsonDocument doc(array);
    if((!doc.isEmpty()) && (!doc.isNull())){
        addExtras(name,doc.toJson().data());
    }
    return *this;
}

Extras &Extras::addExtras(const char *name, const QStringList &list)
{
    addExtras(QString(name),list);
    return *this;
}

unsigned long long Extras::getULongLongExtras(const QString &name){
    return getStringExtras(name).toULongLong();
}

unsigned long Extras::getULongExtras(const QString &name){
    return getStringExtras(name).toULong();
}

QString Extras::getStringExtras(const QString &name){
    return getStringExtras(name.toStdString().c_str());
}

int Extras::getIntExtras(const QString &name){
    return getStringExtras(name).toInt();
}

bool Extras::getBooleanExtras(const QString &name){
    return getBooleanExtras(name.toStdString().c_str());
}

float Extras::getFloatExtras(const QString &name){
    return getFloatExtras(name.toStdString().c_str());
}

double Extras::getDoubleExtras(const QString &name){
    return getDoubleExtras(name.toStdString().c_str());
}

long Extras::getLongExtras(const QString &name){
    return getLongExtras(name.toStdString().c_str());
}

QJsonObject Extras::getJsonObjectExtras(const QString &name)
{
    QByteArray json = getByteArrayExtras(name.toStdString().c_str());
    auto emptyJsonObject = QJsonDocument::fromJson("{}").object();
    if(json.isEmpty()){
        return emptyJsonObject;
    }
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if((!doc.isEmpty()) && (!doc.isNull())){
        if(doc.isObject()){
            return doc.object();
        }else{
            return emptyJsonObject;
        }
    }else{
        return emptyJsonObject;
    }
}

QJsonArray Extras::getJsonArrayExtras(const QString &name)
{
    QByteArray json = getByteArrayExtras(name.toStdString().c_str());
    auto emptyJsonArray = QJsonDocument::fromJson("[]").array();
    if(json.isEmpty()){
        return emptyJsonArray;
    }
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if((!doc.isEmpty()) && (!doc.isNull())){
        if(doc.isArray()){
            return doc.array();
        }else{
            return emptyJsonArray;
        }
    }else{
        return emptyJsonArray;
    }
}

Extras &Extras::removeExtras(const QString &name){
    return removeExtras(name.toStdString().c_str());
}

unsigned long long Extras::getULongLongExtras(const char *name){
    return getStringExtras(name).toULongLong();
}

unsigned long Extras::getULongExtras(const char *name){
    return getStringExtras(name).toULong();
}

QByteArray Extras::getByteArrayExtras(const char *name){

    int value_i = -1;
    int value_len = -1;
    if(!containsExtras(name,&value_i,&value_len,0,0)){
        return QByteArray("");
    }
    QByteArray byteArray = data_.mid(value_i,value_len);
    if(byteArray.isNull()){
        return QByteArray("");
    }else{
        return byteArray;
    }

}

QString Extras::getStringExtras(const char *name){    
    return getByteArrayExtras(name).data();
}

int Extras::getIntExtras(const char *name){
    return getStringExtras(name).toInt();
}

bool Extras::getBooleanExtras(const char *name){
    return (getIntExtras(name) == 1) ? true : false;
}

float Extras::getFloatExtras(const char *name){
    return getStringExtras(name).toFloat();
}

double Extras::getDoubleExtras(const char *name){
    return getStringExtras(name).toDouble();
}

long Extras::getLongExtras(const char *name){
    return getStringExtras(name).toLong();
}

Extras &Extras::removeExtras(const char *name){
    int startIndex = -1;
    int endIndex = -1;
    if(containsExtras(name,0,0,&startIndex,&endIndex)){
        int len = (endIndex - startIndex) + 1;
        data_.remove(startIndex,len);
        decrementExtrasCount();
    }
    return *this;
}

QJsonObject Extras::getJsonObjectExtras(const char *name)
{
    return getJsonObjectExtras(QString(name));
}

QJsonArray Extras::getJsonArrayExtras(const char *name)
{
    return getJsonArrayExtras(QString(name));
}

int Extras::getExtrasCount(){
    return getIntExtras(EXTRAS_COUNT_SIGNATURE);
}

bool Extras::containsExtras(const QString &name){
    return containsExtras(name.toStdString().c_str());

}

bool Extras::containsExtras(const char *name){
    return containsExtras(name,0,0,0,0);
}

QString Extras::getDataId(){
    return getStringExtras(ID_SIGNATURE);
}

bool Extras::operator ==(const Extras &rhs){
    return (this->data_ == rhs.data_);
}

bool Extras::operator !=(const Extras &rhs){
    return (this->data_ != rhs.data_);
}

void Extras::operator =(const Extras &rhs){
    data_.clear();
    data_.append(rhs.data_);
}

Extras &Extras::setMgbSessionId(const QString& mgbSessionId){
    removeExtras(MGB_SESSION_ID_SIGNATURE);
    data_.append(QString("%1|E%2|:|").arg(MGB_SESSION_ID_SIGNATURE).arg(mgbSessionId));
    return *this;
}

QString Extras::getMgbSessionId(){
    return getStringExtras(MGB_SESSION_ID_SIGNATURE).trimmed();
}

bool Extras::isValid(){
    bool containController = containsElement(CONTROLLER_SIGNATURE.toStdString().c_str(),'C',0,0,0,0);
    bool containAction = containsElement(ACTION_SIGNATURE.toStdString().c_str(),'A',0,0,0,0);
    bool containId = containsElement(ID_SIGNATURE.toStdString().c_str(),'E',0,0,0,0);
    return (containController && containAction && containId);
}

bool Extras::isEmpty(){
    const int  count  = getExtrasCount();
    return (count == 0);
}

void Extras::clear(){
    QString id = getDataId();
    data_.clear();
    data_.append(QString("%1|C%2|:|").arg(CONTROLLER_SIGNATURE).arg(NON));
    data_.append(QString("%1|A%2|:|").arg(ACTION_SIGNATURE).arg(NON));
    data_.append(QString("%1|E%2|:|").arg(EXTRAS_COUNT_SIGNATURE).arg(0));
    data_.append(QString("%1|E%2|:|").arg(ID_SIGNATURE).arg(id));

}

QByteArray Extras::toByteArray(){
    return data_;
}

bool Extras::containsExtras(const char *name, int *valueIndex, int *valueLength,
                            int* extrasStartIndex,int* extrasEndIndex){
    return containsElement(name,'E',valueIndex,valueLength,extrasStartIndex,extrasEndIndex);
}

bool Extras::containsElement(const char *name, const QChar identifier, int *valueIndex, int *valueLength,
                             int* extrasStartIndex, int* extrasEndIndex){
    if(valueIndex)
        *valueIndex = -1;

    if(valueLength)
        *valueLength = -1;

    if(extrasStartIndex)
        *extrasStartIndex = -1;

    if(extrasEndIndex)
        *extrasEndIndex = -1;

    int nameStart_i = data_.indexOf(QString("%1|%2").arg(name).arg(identifier));
    if(nameStart_i == -1){
        return false;
    }

    int nameEnd0_i = data_.indexOf('|',nameStart_i);
    if(nameEnd0_i == -1 || (data_.at(nameEnd0_i) != '|')){
        return false;
    }


    int nameEnd1_i = data_.indexOf(identifier,nameEnd0_i);
    if(nameEnd1_i == -1 || (data_.at(nameEnd1_i) != identifier)){
        return false;
    }

    int delimiterStart_i = -1;
    int delimiterStart_i_startFrom = nameEnd1_i;

recalc_delimiterStart_i:
    delimiterStart_i = data_.indexOf('|',delimiterStart_i_startFrom);
    if(delimiterStart_i == -1 || (data_.at(delimiterStart_i) != '|')){
        return false;
    }


    int delimiterMid_i = -1;
    int delimiterMid_i_startFrom = delimiterStart_i;

recalc_delimiterMid_i:
    delimiterMid_i = data_.indexOf(':',delimiterMid_i_startFrom);
    if(delimiterMid_i == -1 || (data_.at(delimiterMid_i) != ':')){
        return false;
    }else{
        if(delimiterStart_i != (delimiterMid_i - 1)){//this means the | we found is not it,so find the next one
            delimiterStart_i_startFrom  = (delimiterStart_i+1);
            goto recalc_delimiterStart_i;
        }
    }

    int delimiterEnd_i = data_.indexOf('|',delimiterMid_i);
    if(delimiterEnd_i == -1 || (data_.at(delimiterEnd_i) != '|')){
        return false;
    }else{
        if(delimiterMid_i != (delimiterEnd_i - 1)){//this means the : we found is not it,so find the next one
            delimiterMid_i_startFrom  = (delimiterMid_i+1);
            goto recalc_delimiterMid_i;
        }
    }

    if(valueIndex)
        *valueIndex = (nameEnd1_i+1);

    if(valueLength)
        *valueLength = (delimiterStart_i - (*valueIndex));

    if(extrasStartIndex)
        *extrasStartIndex = nameStart_i;

    if(extrasEndIndex)
        *extrasEndIndex = delimiterEnd_i;

    return true;
}

QString Extras::getElement(const QString &name,const QChar identifier){

    int value_i = -1;
    int value_len = -1;
    if(!containsElement(name.toStdString().c_str(),identifier,&value_i,&value_len,0,0)){
        return "";
    }
    return data_.mid(value_i,value_len).data();

}

void Extras::setData(const QByteArray &data)
{
    //first
    data_.clear();
    data_.append(data);
    //then
    if(!isValid()){
        qWarning() << "Extras::Extras(const QByteArray &extrasByteArray) :: Error "<< "Can not initialize from a given QByteArray";
        init();
    }
}

QByteArray Extras::getJsonExtras(){
    QString d(this->toByteArray());
    auto array =  d.split("|:|");

    QJsonObject jobj;
    foreach (QString extra, array) {
        const int i = extra.indexOf("|");
        const int e_i = (i +1);
        if(((i != -1) && (e_i < extra.length()) && (extra.at(e_i) == 'E'))){
            QStringList extr = extra.split("|E");
            if(extr.length() >= 2){
                if((extr.at(0) != Extras::ID_SIGNATURE)
                        && (extr.at(0) != Extras::EXTRAS_COUNT_SIGNATURE)){
                    QJsonDocument json = QJsonDocument::fromJson(extr.at(1).toLatin1());
                    if((!json.isEmpty()) && (!json.isNull())){
                        if(json.isObject()){
                            jobj.insert(extr.at(0),json.object());
                        }else if(json.isArray()){
                            jobj.insert(extr.at(0),json.array());
                        }else{
                            jobj.insert(extr.at(0),extr.at(1));
                        }
                    }else{
                        jobj.insert(extr.at(0),extr.at(1));
                    }
                }
            }
        }
    }

    QJsonDocument doc(jobj);
    return doc.toJson(QJsonDocument::Compact);
}

QVariantList Extras::getVariantListExtras(const QString &name){
    return  getJsonArrayExtras(name).toVariantList();
}

void Extras::addElement(const QString &name, const QChar identifier, const QString &value){
    int startIndex = -1;
    int endIndex = -1;
    if(containsElement(name.toStdString().c_str(),identifier,0,0,&startIndex,&endIndex)){
        int len = (endIndex - startIndex) + 1;
        data_.remove(startIndex,len);
    }
    data_.append(QString("%1|%2%3|:|").arg(name).arg(identifier).arg(value));

}

void Extras::init(){
    data_.clear();
    data_.append(QString("%1|C%2|:|").arg(CONTROLLER_SIGNATURE).arg(NON));
    data_.append(QString("%1|A%2|:|").arg(ACTION_SIGNATURE).arg(NON));
    data_.append(QString("%1|E%2|:|").arg(EXTRAS_COUNT_SIGNATURE).arg(0));
    data_.append(QString("%1|E%2|:|").arg(ID_SIGNATURE).arg(generateId()));
}

QString Extras::generateId(){
    QString str(QStringLiteral("789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopgrst0123456"));
    QString generatedStr;
    srand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    for(int x = 1; x <= 142;x++){
        int i = rand()%str.size();
        generatedStr += str.at(i) ;
    }
    QString objAddress(QString::number((unsigned long long)this));
    return generatedStr.append(objAddress).append(QString::number(QDateTime::currentMSecsSinceEpoch()));
}

void Extras::incrementExtrasCount(){
    int count = getExtrasCount();
    count++;

    int startIndex = -1;
    int endIndex = -1;
    if(containsExtras(EXTRAS_COUNT_SIGNATURE.toStdString().c_str(),0,0,&startIndex,&endIndex)){
        int len = (endIndex - startIndex) + 1;
        data_.remove(startIndex,len);
    }
    data_.append(QString("%1|E%2|:|").arg(EXTRAS_COUNT_SIGNATURE).arg(count));
}

void Extras::decrementExtrasCount(){
    int count = getExtrasCount();
    if(count > 0){
        count--;
    }else{
        count = 0;
    }

    int startIndex = -1;
    int endIndex = -1;
    if(containsExtras(EXTRAS_COUNT_SIGNATURE.toStdString().c_str(),0,0,&startIndex,&endIndex)){
        int len = (endIndex - startIndex) + 1;
        data_.remove(startIndex,len);
    }
    data_.append(QString("%1|E%2|:|").arg(EXTRAS_COUNT_SIGNATURE).arg(count));

}
