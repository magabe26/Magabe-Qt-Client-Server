/**
 * Copyright 2019 - MagabeLab (Tanzania). All Rights Reserved.
 * Author Edwin Magabe
 */

#ifndef _NATIVE_REGISTER_H
#define _NATIVE_REGISTER_H 1

#include <QObject>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#include <stddef.h>
#include <android/log.h>
#include "jni.h"
#include "pointerutil.h"

#define LOGE(...) do { __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__); } while(0)
#define LOGI(...) do { __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); } while(0)
#define LOGW(...) do { __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); } while(0)

int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods);

#endif

#endif
