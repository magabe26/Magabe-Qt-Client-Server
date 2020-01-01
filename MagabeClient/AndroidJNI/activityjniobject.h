/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef ACTIVITYJNIOBJECT_H
#define ACTIVITYJNIOBJECT_H
#include <QObject>

#ifdef Q_OS_ANDROID
#include "nativeregister.h"
#include <QAndroidJniObject>


class  ActivityJniObject : public QObject
{
    Q_OBJECT
public:
    explicit ActivityJniObject(QObject *parent = nullptr){ Q_UNUSED(parent);}

    constexpr static const char *ACTIVITY_CLASS_NAME = "com/magabelab/magabeclient/MagabeClientActivity";

    QAndroidJniObject getActivity() {
        if(!activity_.isValid()){
            activity_ = QAndroidJniObject::callStaticObjectMethod(
                        ACTIVITY_CLASS_NAME, // java class name
                        "getInstance", // method name
                        QString("()L%1;").arg(ACTIVITY_CLASS_NAME).toStdString().c_str());//our Activity contructor signature
        }
        return activity_;
    }

private:
    QAndroidJniObject activity_;
};


#endif
#endif // ACTIVITYJNIOBJECT_H



