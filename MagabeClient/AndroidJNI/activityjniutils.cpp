/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#include "activityjniutils.h"
#include <QDebug>
#include <QCoreApplication>

#ifdef Q_OS_ANDROID

/*.........native functions , class name & method_table  must be here in .cpp not in header,so asto hide them .......*/

/*....................native .methods...............................................................................*/
/*................DO NOT USE , Qt::BlockingQueuedConnection  WILL CAUSE APP TO DEADLOCK...............REMEMBER that black screen*/
/*
 * Method:    onOrientationLandscape
 * Signature: (V)V
 */
void onOrientationLandscape(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onOrientationLandscape");

}

/*
 * Method:    onOrientationPortrait
 * Signature: (V)V
 */
void onOrientationPortrait(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onOrientationPortrait");

}
/*
 * Method:    onActivityPause
 * Signature: (V)V
 */
void onActivityPause(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onActivityPause");

}

void onVolumeDown(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onVolumeDown");

}

void onVolumeUp(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onVolumeUp");

}

void onVolumeMute(JNIEnv *, jclass)
{
    QMetaObject::invokeMethod(&ActivityJNIUtils::getInstance(), "onVolumeMute");

}
/*................................................................................................................*/
//
static JNINativeMethod method_table[] = {

    { "onOrientationLandscape", "()V",(void*) onOrientationLandscape},
    { "onOrientationPortrait", "()V",(void*) onOrientationPortrait},

    { "onActivityPause", "()V",(void*) onActivityPause},

    { "onVolumeDown", "()V",(void*) onVolumeDown},
    { "onVolumeUp", "()V",(void*) onVolumeUp},
    { "onVolumeMute", "()V",(void*) onVolumeMute},
};

/*..................................................................................................................*/


/*...........................the...class that uses the above methoda...............................................*/

/**
 * @brief ActivityJNIUtils::ActivityJNIUtils
 * @param parent
 */
ActivityJNIUtils::ActivityJNIUtils(QObject *parent)
{
    Q_UNUSED(parent)
    connect(QCoreApplication::instance(),&QCoreApplication::aboutToQuit,this,&ActivityJNIUtils::onAppAboutQt);
}

/**
 * @brief N3ctaActivityJNIUtils::init_N3ctaActivityJNIUtils
 * @param env
 * @return
 */
int ActivityJNIUtils::init(JNIEnv *env)
{
    // register our native methods
    if (!registerNativeMethods(env, ActivityJniObject::ACTIVITY_CLASS_NAME/*java class with native methods*/, method_table,
                               sizeof(method_table) / sizeof(method_table[0]))) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/**
 * @brief N3ctaActivityJNIUtils::showToastMessage
 * @param Message
 */
void ActivityJNIUtils::showToastMessage(const QString& message)
{

    QAndroidJniObject toastMessage = QAndroidJniObject::fromString(message);
    //getActivity().callMethod only call method contained in QtActivity class or its Subclass
    getActivity().callMethod<void>("showToast","(Ljava/lang/String;)V",toastMessage.object<jstring>());
}

/**
 * @brief N3ctaActivityJNIUtils::sendSMS
 * @param number
 * @param message
 */
void ActivityJNIUtils::sendSMS(const QString& number, const QString &message)
{

    QAndroidJniObject sms = QAndroidJniObject::fromString(message);
    QAndroidJniObject smsReceiver = QAndroidJniObject::fromString(number);
    //getActivity().callMethod only call method contained in QtActivity class or its Subclass
    getActivity().callMethod<void>("sendSMS", "(Ljava/lang/String;Ljava/lang/String;)V",
                                   smsReceiver.object<jstring>(),sms.object<jstring>());

}

bool ActivityJNIUtils::isPortrait()
{
    bool b = getActivity().callMethod<jboolean>("isPortrait","()Z");
    return b;
}


void ActivityJNIUtils::releaseWakeLock() {
    getActivity().callMethod<void>("releaseWakeLock","()V");
}

void ActivityJNIUtils::acquireWakeLock() {
    getActivity().callMethod<void>("acquireWakeLock","()V");
}

bool ActivityJNIUtils::hasActiveWakeLock()
{
    bool b = getActivity().callMethod<jboolean>("hasActiveWakeLock","()Z");
    return b;
}

void ActivityJNIUtils::goNormalScreen() {
    getActivity().callMethod<void>("goNormalScreen","()V");

}

void ActivityJNIUtils::goFullScreen() {
    getActivity().callMethod<void>("goFullScreen","()V");
}

void ActivityJNIUtils::onOrientationLandscape()
{
    emit orientationLandscape();
}

void ActivityJNIUtils::onOrientationPortrait()
{
    emit orientationPortrait();
}


void ActivityJNIUtils::onActivityPause()
{
    emit activityPause();
}

void ActivityJNIUtils::onVolumeDown()
{
    emit volumeDown();
}

void ActivityJNIUtils::onVolumeUp()
{
    emit volumeUp();
}

void ActivityJNIUtils::onVolumeMute()
{
    emit volumeMute();
}

void ActivityJNIUtils::onAppAboutQt(){
    releaseWakeLock();//release any locks
}
/*...................................................................................................................*/

/* exaple call for startic java class
callStaticMethod<void>(classPathName// java class name
       , "sendSMS" // method name
       , "(Ljava/lang/String;Ljava/lang/String;)V" // signature  (arg-types)return -type //use java command --> javap -s -p classname.java to find signature
       ,Number.object<jstring>(),Message.object<jstring>());  //fuction param, gettiing the message from the object

*/
#else
/*........................................twin dummy class for non android platform.........................*/


void ActivityJNIUtils::showToastMessage(QString Message){Q_UNUSED(Message);}

void ActivityJNIUtils::sendSMS(QString number, QString message){Q_UNUSED(message);Q_UNUSED(number);}

bool ActivityJNIUtils::isPortrait(){
    return false;
}



/*..............................................................................................................*/

#endif
