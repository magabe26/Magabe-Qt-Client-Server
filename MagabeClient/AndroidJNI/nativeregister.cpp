/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 *

HOW GENERATED JNI HEADER FILES AND FIND FUNCTION SIGNATURE IN ANDROID STUDIO

--1ST------ FIND LOCATION OF android.jar ------

android.jar location for each version

--eg--
C:\androidStudioAndroidsdk\platforms\android-26

WHERE
    -> C:\androidStudioAndroidsdk == where  the sdk is installed
    -> android-26 == version

--2SD------ CREATE HEADER FILES  --------

   -----IN ANDROID STUDIO TERMINAL EXECUTE---
  javah -d GENERATED_JNI_HEADER_FILES -classpath C:\androidStudioAndroidsdk\platforms\android-26\android.jar;app\src\main\java\ com.churamkiasoft.app.MainActivity

  WHERE
       -> app\src\main\java\    == LOCATION WHERE OUR CLASS WITH STATIC FUNCTION ie com.churamkiasoft.app.MainActivity IS LOCATED
       -> GENERATED_JNI_HEADER_FILES  == where  our header file will be created in PROJECT FOLDER
**/

#include "nativeregister.h"

#ifdef Q_OS_ANDROID

/*..............include classes contain native methods here...*/

#include "activityjniutils.h"

/*..............................................................*/

#define LOG_TAG "nativeRegister"

/*
 * Register several native methods for one class.
 */
int registerNativeMethods(JNIEnv* env, const char* className,
                          JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;

    clazz = env->FindClass(className);
    if (clazz == NULL) {
        LOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

// ----------------------------------------------------------------------------


typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

/*
 * This is called by the VM when the shared library is first loaded.
 */
jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    Q_UNUSED(reserved);
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    JNIEnv *env = NULL;

    LOGI("JNI_OnLoad");

    // get the JNIEnv pointer.
    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_6) != JNI_OK) {
        LOGE("ERROR: GetEnv failed");
        return JNI_ERR;
    }

    env = uenv.env;

    /*..................register methods from different classes...................*/
    //register native methods from ActivityJNIUtils.cpp
    if (ActivityJNIUtils::init(env) != JNI_TRUE) {
        LOGE("ERROR: initilization of ActivityJNIUtil failed");
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}

#endif



