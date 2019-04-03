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

#include "library_api/aunixdatagramsocket.h"
#include "library_api/loop.hpp"



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
    LOGD("START THREAD main_3");
//   Epoll epoll(true);
//   unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//   unixSocket->connect<&standaloneDataFromUnixSocket>();
//   unixSocket->Bind();
//   epoll.runApp();
//   delete unixSocket;

   //------------------------------


    Loop loop;
    ALooper *looper = loop.init(true);

    unixSocket_1 = new AUnixDatagramSocket();
    unixSocket_1->connect<&standaloneDataFromUnixSocket_1>();
    unixSocket_1->Bind("\0to_NDK_1");


    unixSocket_2 = new AUnixDatagramSocket();
    unixSocket_2->connect<&standaloneDataFromUnixSocket_2>();
    unixSocket_2->Bind("\0to_NDK_2");

    loop.runApp();

    delete unixSocket_1;

}


