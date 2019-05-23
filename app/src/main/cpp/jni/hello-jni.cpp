/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <jni.h>
#include <string.h>

#include <pthread.h>
#include <thread>
#include "library_api/cunixdatagramsocket.h"


/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   hello-jni/app/src/main/java/com/example/hellojni/HelloJni.java
 */


#ifdef __ANDROID__
#   include <android/log.h>
#endif



#define DEBUG_TAG "CapAfriJni"

#ifdef __ANDROID__
#   define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__))
#   define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#   define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, __VA_ARGS__))
#   define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))
#endif


pthread_t thread_main_2;
//void *main_3( void *ptr );
//
//
//
//
//void *main_3( void * message)
//{
//    LOGD("---- NOWY THREAD DZIALA !!!");
////    Epoll epoll(true);
////    Application application;
////    epoll.runApp();
//
//}

void bar(int x)
{
    // do stuff...
}

const char *message = "Main 2";


CUnixDatagramSocket * unixSendSocket = nullptr;


extern "C" {
JNIEXPORT /*jstring*/ int JNICALL Java_com_reyfel_sample_CapAfri_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz);
JNIEXPORT int JNICALL Java_com_reyfel_sample_CapAfri_MainActivity_sendCommand(JNIEnv *env, jobject thiz, jobjectArray pStringArray);
};

JNIEXPORT /*jstring*/ int JNICALL
Java_com_reyfel_sample_CapAfri_MainActivity_stringFromJNI(JNIEnv *env,
                                                          jobject thiz) {

    //START ANOTHER THREAD
//   const char *message = "Main 2";
//    int iret1;
//    iret1 = pthread_create(&thread_main_2, NULL, main_3, (void *) message);
//    if (iret1) {
//        LOGD("Error - pthread_create() return code: %d\n", iret1);
//    }
//
//    pthread_join(thread_main_2, NULL);
//
//
//    //DEBUG
//    for (int i = 0; i < 100; i++) {
//        LOGD("---- JUHU TO DZIALA !!!");
//    }




    //main, foo and bar now execute concurrently

    // synchronize threads:

  //  second.join();               // pauses until second finishes

    unixSendSocket = new CUnixDatagramSocket();

    return 14;
}

std::string jstring2string(JNIEnv *env, jstring jStr) {
    if (!jStr)
        return "";

    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}

JNIEXPORT int JNICALL Java_com_reyfel_sample_CapAfri_MainActivity_sendCommand(JNIEnv *env, jobject thiz, jobjectArray pStringArray)
{
    char buffer[6] = {1,2,3,4,5,6};
    unixSendSocket->writeDatagram("\0to_NDK_2",buffer, 1);
    unixSendSocket->writeDatagram("\0to_NDK_3",buffer, 5);
    unixSendSocket->writeDatagram("\0to_NDK_4",buffer, 2);

    string txt;

        jsize array_length = env->GetArrayLength(pStringArray);

        for (int32_t i = 0; i < array_length; ++i)
        {
            jstring java_string = (jstring) env->GetObjectArrayElement(pStringArray, i);

            txt.append(jstring2string(env, java_string));
            txt.append("|");
        }

    unixSendSocket->writeDatagram("\0to_com_reyfel_system_billing",txt.c_str(),txt.size());

    return 0;
}






