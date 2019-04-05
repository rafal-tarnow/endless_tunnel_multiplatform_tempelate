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



CUnixDatagramSocket * unixSocket_1 = nullptr;
CUnixDatagramSocket * unixSocket_2 = nullptr;

CUnixDatagramSocket * unixSocket_3 = nullptr;
CUnixDatagramSocket * unixSocket_4 = nullptr;

void standaloneDataFromUnixSocket_1()
{
    static int index = 0;
    index++;
    if(index == 10)
    {
        Loop::exit(LOOP_EXIT_SUCCESS);
        index = 0;
    }
    LOGD("void standaloneDataFromUnixSocket_1()");

    std::vector<char> buffer;
    unixSocket_1->readDatagram(&buffer);

    LOGD("    size_1 = %d", int(buffer.size()));
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

    LOGD("    size_2 = %d", int(buffer.size()));
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer_2[%d] = %d",i, int(buffer[i]));
    }
}


void standaloneDataFromUnixSocket_3()
{
    LOGD("void standaloneDataFromUnixSocket_3()");

    std::vector<char> buffer;
    unixSocket_3->readDatagram(&buffer);

    LOGD("    size_3 = %d", buffer.size());
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer_3[%d] = %d",i, int(buffer[i]));
    }
}

void standaloneDataFromUnixSocket_4()
{
    LOGD("void standaloneDataFromUnixSocket_4()");

    std::vector<char> buffer;
    unixSocket_4->readDatagram(&buffer);

    LOGD("    size_4 = %d", buffer.size());
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer_4[%d] = %d",i, int(buffer[i]));
    }
}

void main_3(int x)
{
    LOGD("START THREAD main_3 0");
//   Epoll epoll(true);
//   unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//   unixSocket->connect<&standaloneDataFromUnixSocket>();
//   unixSocket->Bind();
//   epoll.runApp();
//   delete unixSocket;

   //------------------------------


   Loop * loop = new Loop();
    loop->init(true);

    unixSocket_1 = new CUnixDatagramSocket();
    unixSocket_1->connect<&standaloneDataFromUnixSocket_1>();
    unixSocket_1->Bind("\0to_NDK_1");


    unixSocket_2 = new CUnixDatagramSocket();
    unixSocket_2->connect<&standaloneDataFromUnixSocket_2>();
    unixSocket_2->Bind("\0to_NDK_2");

    loop->runApp();

    delete unixSocket_1;
    delete unixSocket_2;

    delete loop;



    LOGD("START THREAD main_3 1");

    loop = new Loop();
    loop->init(true);

    unixSocket_1 = new CUnixDatagramSocket();
    unixSocket_1->connect<&standaloneDataFromUnixSocket_1>();
    unixSocket_1->Bind("\0to_NDK_1");


    unixSocket_2 = new CUnixDatagramSocket();
    unixSocket_2->connect<&standaloneDataFromUnixSocket_2>();
    unixSocket_2->Bind("\0to_NDK_2");

    loop->runApp();

    delete unixSocket_1;
    delete unixSocket_2;

    delete loop;

    LOGD("STOP THREAD main_3");
}


void main_4(int x)
{
    LOGD("START THREAD main_4");
//   Epoll epoll(true);
//   unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//   unixSocket->connect<&standaloneDataFromUnixSocket>();
//   unixSocket->Bind();
//   epoll.runApp();
//   delete unixSocket;

    //------------------------------


//    Loop loop;
//    loop.init(true);
//
//    unixSocket_3 = new CUnixDatagramSocket();
//    unixSocket_3->connect<&standaloneDataFromUnixSocket_3>();
//    unixSocket_3->Bind("\0to_NDK_3");
//
//
//    unixSocket_4 = new CUnixDatagramSocket();
//    unixSocket_4->connect<&standaloneDataFromUnixSocket_4>();
//    unixSocket_4->Bind("\0to_NDK_4");
//
//    loop.runApp();
//
//    delete unixSocket_3;
//    delete unixSocket_4;

}

