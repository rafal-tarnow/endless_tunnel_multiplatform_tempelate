#pragma once

#ifdef __ANDROID__
#   include <jni.h>
#   include <android_native_app_glue.h>
#endif

void initPurchase(android_app * );

void purchase(const char* message);