#include "main_2.h"
#include <sys/epoll.h>

#ifdef __ANDROID__
#   include <android/log.h>
#   include <android/looper.h>
#endif



#define DEBUG_TAG "CapAfriJni"

#ifdef __ANDROID__
#   define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__))
#   define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#   define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, __VA_ARGS__))
#   define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))
#endif

#include "library_api/cunixdatagramsocket.h"
#include "library_api/loop.hpp"




//CUnixDatagramSocket * unixSocket_1 = nullptr;
//CUnixDatagramSocket * unixSocket_2 = nullptr;
//
//void standaloneDataFromUnixSocket_1()
//{
//    static int index = 0;
//    index++;
//    if(index == 10)
//    {
//        Loop::exit(LOOP_EXIT_SUCCESS);
//        index = 0;
//    }
//    LOGD("void standaloneDataFromUnixSocket_1()");
//
//    std::vector<char> buffer;
//    unixSocket_1->readDatagram(&buffer);
//
//    LOGD("    size_1 = %d", int(buffer.size()));
//    for(unsigned int i = 0; i < buffer.size(); i++)
//    {
//        LOGD("    buffer_1[%d] = %d",i, int(buffer[i]));
//    }
//}
//
//void standaloneDataFromUnixSocket_2()
//{
//    LOGD("void standaloneDataFromUnixSocket_2()");
//
//    std::vector<char> buffer;
//    unixSocket_2->readDatagram(&buffer);
//
//    LOGD("    size_2 = %d", int(buffer.size()));
//    for(unsigned int i = 0; i < buffer.size(); i++)
//    {
//        LOGD("    buffer_2[%d] = %d",i, int(buffer[i]));
//    }
//}

void main_3(int x)
{
    LOGD("START THREAD main_3");

//    ALooper * looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
//
//    Loop * loop = new Loop();
//    loop->init(true);
//
//
//    unixSocket_1 = new CUnixDatagramSocket();
//    unixSocket_1->connect<&standaloneDataFromUnixSocket_1>();
//    unixSocket_1->Bind("\0to_NDK_1");
//
//
//    unixSocket_2 = new CUnixDatagramSocket();
//    unixSocket_2->connect<&standaloneDataFromUnixSocket_2>();
//    unixSocket_2->Bind("\0to_NDK_2");
//
//    int retVal, fd, events;
//    while (true)
//    {
//        retVal = ALooper_pollAll(-1, &fd, &events, NULL);
//
//        if (retVal == ALOOPER_POLL_WAKE) {
//            LOGD("LOOP ALOOPER_POLL_WAKE");
//        } else if (retVal == ALOOPER_POLL_CALLBACK) {
//            LOGD("LOOP ALOOPER_POLL_CALLBACK");
//        } else if (retVal == ALOOPER_POLL_TIMEOUT) {
//            LOGD("LOOP ALOOPER_POLL_TIMEOUT");
//        } else if (retVal == ALOOPER_POLL_ERROR) {
//            LOGD("LOOP ALOOPER_POLL_ERROR");
//        }
//        else {
//            loop->processFileDescriptorEvent(fd);
//        }
//    }
//
//    //loop->runApp();
//
//    delete unixSocket_1;
//    delete unixSocket_2;
//    delete loop;
//
//    LOGD("START THREAD main_3 1");

    LOGD("STOP THREAD main_3");
}


