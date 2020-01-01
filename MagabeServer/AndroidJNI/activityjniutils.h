/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */


#ifndef N3CTAACTIVITYJNIUTILS_H
#define N3CTAACTIVITYJNIUTILS_H

#include <QObject>
#ifdef Q_OS_ANDROID
#include "activityjniobject.h"

/* ...........use as template..............*/
class ActivityJNIUtils : public ActivityJniObject
{
    Q_OBJECT
public:

    /*  .......must contain......*/
    static ActivityJNIUtils &getInstance(){
        static ActivityJNIUtils instance(nullptr);//will be deleted by the system,when app exit
        return instance;
    }
    /*  ...............................*/
    static int init(JNIEnv *env);

    void showToastMessage(const QString& message);
    void sendSMS(const QString &number,const QString& message);
    bool isPortrait();
    void releaseWakeLock();
    void acquireWakeLock();
    bool hasActiveWakeLock();
    void goNormalScreen();
    void goFullScreen();
signals:
    void orientationLandscape();
    void orientationPortrait();
    void activityPause();
    void volumeDown();
    void volumeUp();
    void volumeMute();
public slots:
    void onOrientationLandscape();
    void onOrientationPortrait();
    void onActivityPause();
    void onVolumeDown();
    void onVolumeUp();
    void onVolumeMute();
    void onAppAboutQt();
private:
    explicit ActivityJNIUtils(QObject *parent = nullptr);/*  .......must contain......*/

};

#else

/*........................................twin dummy class for non android platform.........................*/
//note -to arrage nove the firt method ,position the method, then move others
class ActivityJNIUtils : public QObject
{
    Q_OBJECT

signals:
    void orientationLandscape();
    void orientationPortrait();
    void activityPause();
    void volumeDown();
    void volumeUp();
    void volumeMute();

public:

    static ActivityJNIUtils &getInstance(){
        static ActivityJNIUtils instance(nullptr);//will be deleted by the system,when app exit
        return instance;
    }

    void showToastMessage(QString Message);
    void sendSMS(QString number, QString message);
    bool isPortrait();
    void releaseWakeLock(){}
    void acquireWakeLock(){}
    bool hasActiveWakeLock(){return false;}
    void goNormalScreen() {}
    void goFullScreen(){}
    void onActivityPause(){}
    void onVolumeDown(){}
    void onVolumeUp() {}
    void onVolumeMute(){}
    void onAppAboutQt(){}

signals:

public slots:
private:
    explicit ActivityJNIUtils(QObject *parent = nullptr){Q_UNUSED(parent)}
};

/*..............................................................................................................*/
#endif

#endif // N3CTAACTIVITYJNIUTILS_H
