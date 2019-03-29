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

#include "aunixdatagramsocket.h"



AUnixDatagramSocket * unixSocket_1 = nullptr;
AUnixDatagramSocket * unixSocket_2 = nullptr;

void standaloneDataFromUnixSocket_1()
{
    LOGD("void standaloneDataFromUnixSocket_1()");

    std::vector<char> buffer;
    unixSocket_1->readDatagram(&buffer);

    LOGD("    size_1 = %d", buffer.size());
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer_1[%d] = %d",i, int(buffer[i]));
    }
}

void standaloneDataFromUnixSocket_2()
{
    LOGD("void standaloneDataFromUnixSocket_2()");

    std::vector<char> buffer;
    unixSocket_2->readDatagram(&buffer);

    LOGD("    size_2 = %d", buffer.size());
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer_2[%d] = %d",i, int(buffer[i]));
    }
}




int callbackFunc(int fd, int events, void* data)
{
    LOGD("callbackFunc");
    if(events == ALOOPER_EVENT_INPUT)
    {
        LOGD("ALOOPER_EVENT_INPUT");
        if(fd == unixSocket_1->getFD())
        {
            unixSocket_1->PublicReadyReadSlot(fd);
            return 1;
        }
        else if(fd == unixSocket_2->getFD())
        {
            unixSocket_2->PublicReadyReadSlot(fd);
            return 1;
        }
    }
    return 1;
}

void main_3(int x)
{
   LOGD("---- NOWY THREAD DZIALA !!!");
//   Epoll epoll(true);
//   unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//   unixSocket->connect<&standaloneDataFromUnixSocket>();
//   unixSocket->Bind();
//   epoll.runApp();
//   delete unixSocket;

   //------------------------------

    ALooper * looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

    unixSocket_1 = new AUnixDatagramSocket();
    unixSocket_1->connect<&standaloneDataFromUnixSocket_1>();
    unixSocket_1->Bind("\0to_NDK_1");
    ALooper_addFd(looper, unixSocket_1->getFD(), ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, callbackFunc, NULL);

    unixSocket_2 = new AUnixDatagramSocket();
    unixSocket_2->connect<&standaloneDataFromUnixSocket_2>();
    unixSocket_2->Bind("\0to_NDK_2");
    ALooper_addFd(looper, unixSocket_2->getFD(), ALOOPER_POLL_CALLBACK, ALOOPER_EVENT_INPUT, callbackFunc, NULL);

    int events;
    ALooper_pollAll(-1, NULL, &events, NULL);

    delete unixSocket_1;

}


