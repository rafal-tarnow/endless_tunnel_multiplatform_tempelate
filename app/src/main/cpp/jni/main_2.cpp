#include "main_2.h"
#include <sys/epoll.h>

#ifdef __ANDROID__
#   include <android/log.h>
#include <android/looper.h>

#endif



#define DEBUG_TAG "CapAfriJni"

#ifdef __ANDROID__
#   define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, __VA_ARGS__))
#   define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, __VA_ARGS__))
#   define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, __VA_ARGS__))
#   define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, __VA_ARGS__))
#endif

#include "aunixdatagramsocket.h"



AUnixDatagramSocket * unixSocket = nullptr;

void standaloneDataFromUnixSocket()
{
    LOGD("void standaloneDataFromUnixSocket()");

    std::vector<char> buffer;
    unixSocket->readDatagram(&buffer);

    LOGD("    size = %d", buffer.size());
    for(unsigned int i = 0; i < buffer.size(); i++)
    {
        LOGD("    buffer[%d] = %d",i, int(buffer[i]));
    }
}


int _epoll_fd = -1;

void main_3(int x)
{
   LOGD("---- NOWY THREAD DZIALA !!!");
    Epoll epoll(true);
//
//    unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//    unixSocket->connect<&standaloneDataFromUnixSocket>();
//    unixSocket->Bind();
//
    epoll.runApp();
//
//
//    delete unixSocket;

    //----------------------------

//    _epoll_fd = epoll_create(100);
//    if (_epoll_fd == -1)
//    {
//        LOGD("[ERROR][EPOLL | init] Nie udało się otworzyć epoll'a");
//        return;
//    }
//
//    unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
//    unixSocket->connect<&standaloneDataFromUnixSocket>();
//    unixSocket->Bind();
//
//
//    //AD FD
//    struct epoll_event event_ctl;
//
//    event_ctl.data.fd = unixSocket->getFD();
//    event_ctl.events = EPOLLIN;
//
//    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, unixSocket->getFD(), &event_ctl) == -1)
//    {
//            LOGD("[ERROR][EPOLL | addClient] Nie można dodać klienta do Epoll'a fd = ");
//            goto exit1;
//    }
//
//    //RUN
//    {
//#define EPOLL_MAX_EVENTS 64
//        int fileDesc_nums = -1;
//        struct epoll_event epoll_events[EPOLL_MAX_EVENTS];
//        bool run = true;
//
//        while(run) {
//            fileDesc_nums = epoll_wait(_epoll_fd, epoll_events, EPOLL_MAX_EVENTS, -1);
//
//            for (int i = 0; i < fileDesc_nums; i++)
//            {
//                if (epoll_events[i].data.fd == unixSocket->getFD())
//                {
//                    LOGD("[EPOLL | eventFrom] unixSocket ");
//                    unixSocket->PublicReadyReadSlot(epoll_events[i].data.fd);
//                    //recipientsMap[epoll_events[i].data.fd](epoll_events[i].data.fd);
//                }
//
//            }
//        }
//
//    }
//
//
//
//    //DELETE FD
//    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, unixSocket->getFD(), NULL);
//    close(_epoll_fd);
//    _epoll_fd = -1;
//
//    exit1:
//    delete unixSocket;
   //------------------------------

    unixSocket = new AUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
    unixSocket->connect<&standaloneDataFromUnixSocket>();
    unixSocket->Bind();

    int fd = -1;
    int events, retVal;

    ALooper* looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);

    ALooper_addFd(looper, unixSocket->getFD(), unixSocket->getFD(), ALOOPER_EVENT_INPUT, NULL, NULL);


    while (1) {
        retVal = ALooper_pollAll(-1, NULL, &events, NULL);

        if(retVal == ALOOPER_POLL_WAKE)
        {
            LOGD("ALOOPER_POLL_WAKE");
        }
        else if(retVal == ALOOPER_POLL_CALLBACK)
        {
            LOGD("ALOOPER_POLL_CALLBACK");
        }
        else if(retVal == ALOOPER_POLL_TIMEOUT)
        {
            LOGD("ALOOPER_POLL_TIMEOUT");
        }
        else if(retVal == ALOOPER_POLL_ERROR)
        {
            LOGD("ALOOPER_POLL_ERROR");
        }
        else
        {
            LOGD("retVal == %d",retVal);
            if(retVal == unixSocket->getFD())
            {
                unixSocket->PublicReadyReadSlot(retVal);
            }
        }


    }

delete unixSocket;

}


