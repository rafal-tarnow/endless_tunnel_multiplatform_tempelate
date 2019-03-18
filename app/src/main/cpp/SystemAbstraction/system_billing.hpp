#pragma once

#include <string>

using namespace std;

#ifdef __ANDROID__
#   include <jni.h>
#   include <android_native_app_glue.h>
#endif

#ifdef __ANDROID__
void initPurchase(android_app * );
#endif

void printToast(const char *message);
int callJava();
string print_dpi();
