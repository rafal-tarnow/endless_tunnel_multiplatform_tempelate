#include "main_2.h"

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

#include "ctimer.h"
#include "cunixdatagramsocket.h"

class Application_2{
public:
    Application_2();
    ~Application_2();

private:
    void onTimer_1s();
    void onTimer_250ms();
    void onTimer_500ms();
    CTimer * timer_1s;
    CTimer * timer_250ms;
    CTimer * timer_500ms;
    CUnixDatagramSocket * unixSocket = nullptr;;
};

class Application_3{
public:
    Application_3();
    ~Application_3();

private:
    void dataFromUnixSocket();
    void onTimer_1s();
    void onTimer_250ms();
    void onTimer_500ms();
    CTimer * timer_1s;
    CTimer * timer_250ms;
    CTimer * timer_500ms;
    CUnixDatagramSocket * unixSocket = nullptr;;
};


void main_2(int x)
{
//    LOGD("---- NOWY THREAD DZIALA !!!");
    Epoll epoll(true);
    Application_2 application;
    epoll.runApp();

}

void main_3(int x)
{
    LOGD("---- NOWY THREAD DZIALA !!!");
    Epoll epoll(true);
    Application_3 application;
    epoll.runApp();

}


#include <iostream>
using namespace std;

Application_2::Application_2()
{
    timer_1s = new CTimer();
    timer_1s->connect<Application_2, &Application_2::onTimer_1s>(this);
    timer_1s->start(1000);

    timer_250ms = new CTimer();
    timer_250ms->connect<Application_2, &Application_2::onTimer_250ms>(this);
    timer_250ms->start(250);

    timer_500ms = new CTimer();
    timer_500ms->connect<Application_2, &Application_2::onTimer_500ms>(this);
    timer_500ms->start(500);

    unixSocket = new CUnixDatagramSocket("com_reyfel_sample_CapAfri_to_Java", true);
}

Application_2::~Application_2()
{
    delete timer_1s;
    delete timer_250ms;
    delete timer_500ms;

    if(unixSocket)
    {
        delete unixSocket;
        unixSocket = nullptr;
    }
}

static int time_1s = 0;

void Application_2::onTimer_1s()
{
    time_1s++;
    LOGD("Application::onTimer_1s()) = ");// << time_1s*1000 << endl;

    char buffer[5] = {1,2,3,4,5};
    unixSocket->writeDatagram("com_reyfel_sample_CapAfri_to_NDK",buffer, 5);
}

static int time_250ms = 0;

void Application_2::onTimer_250ms()
{
    time_250ms += 250;
    LOGD("Application::onTimer_250ms() = ");// << time_250ms << endl;
}

static int time_500ms = 0;

void Application_2::onTimer_500ms()
{
    time_500ms += 500;
    LOGD("Application::onTimer_500ms() = ");// << time_500ms << endl;
}


Application_3::Application_3()
{
    timer_1s = new CTimer();
    timer_1s->connect<Application_3, &Application_3::onTimer_1s>(this);
    timer_1s->start(1000);

    timer_250ms = new CTimer();
    timer_250ms->connect<Application_3, &Application_3::onTimer_250ms>(this);
    timer_250ms->start(250);

    timer_500ms = new CTimer();
    timer_500ms->connect<Application_3, &Application_3::onTimer_500ms>(this);
    timer_500ms->start(500);

    unixSocket = new CUnixDatagramSocket("com_reyfel_sample_CapAfri_to_NDK", true);
    unixSocket->connect<Application_3, &Application_3::dataFromUnixSocket>(this);
    unixSocket->Bind();
}

void Application_3::dataFromUnixSocket(){
    LOGD("Application_3::dataFromUnixSocket()");

    std::vector<char> buffer;
    unixSocket->readDatagram(&buffer);
}


Application_3::~Application_3()
{
    delete timer_1s;
    delete timer_250ms;
    delete timer_500ms;

    if(unixSocket)
    {
        delete unixSocket;
        unixSocket = nullptr;
    }
}

static int time_3_1s = 0;

void Application_3::onTimer_1s()
{
    time_3_1s++;
    LOGD("Application::onTimer_1s()) = ");// << time_1s*1000 << endl;

//    char buffer[5] = {1,2,3,4,5};
//    uint16_t sendPort = 7001;
//    udpSocket->writeDatagram(buffer,5,"127.0.0.1",sendPort);
}

static int time_3_250ms = 0;

void Application_3::onTimer_250ms()
{
    time_3_250ms += 250;
    LOGD("Application::onTimer_250ms() = ");// << time_250ms << endl;
}

static int time_3_500ms = 0;

void Application_3::onTimer_500ms()
{
    time_3_500ms += 500;
    LOGD("Application::onTimer_500ms() = ");// << time_500ms << endl;
}