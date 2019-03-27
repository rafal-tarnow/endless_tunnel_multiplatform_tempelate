#include "ctimer.h"
#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

CTimer::CTimer(){
    // CREATE TIMER FILE DESCRIPTOR
    timer_file_descriptor = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    cout << "[INFO][CTimer | CTimer] utworzono timer o fd = " << timer_file_descriptor << endl;
    if(timer_file_descriptor == -1){
        std::cout << "[ERROR][CTimer | ()] NIe udało się utworzyć timera!" << std::endl;
        return;
    }

    memset(&itval, 0, sizeof(struct itimerspec));

#ifdef QT_CORE_LIB
    //CREATE FILE DESCRIPTOR NOTIFIER
    socketNotifier = new QSocketNotifier(timer_file_descriptor, QSocketNotifier::Read);
    QObject::connect(socketNotifier,SIGNAL(activated(int)),this,SLOT(readyRead()));
#else
#   ifdef USE_SELECT
        Select::addClient<CTimer, &CTimer::readyRead>(timer_file_descriptor, this);
#   else
        Epoll::addClient<CTimer, &CTimer::readyRead>(timer_file_descriptor, this);
#   endif
#endif



}

CTimer::~CTimer(){
#ifdef QT_CORE_LIB
    socketNotifier->disconnect();
    delete socketNotifier;
#else
#   ifdef USE_SELECT
        Select::removeClient(timer_file_descriptor);
#   else
        Epoll::removeClient(timer_file_descriptor);
#   endif
#endif
  //  cout << "[INFO][CTimer | ~CTimer] usunieto timer o fd = " << timer_file_descriptor << endl;
    close(timer_file_descriptor);
}

void CTimer::startSingleShot(unsigned int ms)
{

    startSingleShot(ms, SST_NORMAL_RUN);
}

void CTimer::setSingleShot(bool val)
{
    _is_singleShot = val;
    if (_is_singleShot)
    {
        _singleShot = SST_NORMAL_RUN;
    }
}

void CTimer::startSingleShot(unsigned int ms, SingleShotType_t type){
    //SET TIMER AND START TIMER
    if (timer_file_descriptor == -1)
    {
        std::cout << "[ERROR][CTimer | startSingleShot] Błąd inicjalizacji timera!" << std::endl;
        return;
    }
    _singleShot = type;
    tmp_second = ms / 1000;
    tmp_nano_second = (ms - (tmp_second * 1000)) * 1000000;

    itval.it_value.tv_sec = tmp_second;
    itval.it_value.tv_nsec = tmp_nano_second;
    itval.it_interval.tv_sec = 0;
    itval.it_interval.tv_nsec = 0;
    timerfd_settime(timer_file_descriptor, 0, &itval, NULL);
}

void CTimer::start(unsigned int ms){
    if (timer_file_descriptor == -1)
    {
        std::cout << "[ERROR][CTimer | start] Błąd inicjalizacji timera!" << std::endl;
        return;
    }
    //SET TIMER AND START TIMER
    if (_is_singleShot)
    {
        startSingleShot(ms);
        return;
    }
    _singleShot = SST_NONE;
    tmp_second = ms / 1000;
    tmp_nano_second = (ms - (tmp_second * 1000)) * 1000000;

    itval.it_value.tv_sec = tmp_second;
    itval.it_value.tv_nsec = tmp_nano_second;
    itval.it_interval.tv_sec = tmp_second;
    itval.it_interval.tv_nsec = tmp_nano_second;
    timerfd_settime(timer_file_descriptor, 0, &itval, NULL);
}

void CTimer::stop(){
    if (timer_file_descriptor == -1)
    {
        std::cout << "[ERROR][CTimer | stop] Błąd inicjalizacji timera!" << std::endl;
        return;
    }
    itval.it_value.tv_sec = 0;
    itval.it_value.tv_nsec = 0;
    timerfd_settime(timer_file_descriptor, 0, &itval, NULL);
}

CTimer::CTimerState CTimer::isActive(){
    if (timer_file_descriptor == -1)
    {
        return CTimerState_Error;
    }

    struct itimerspec itval;
    if(timerfd_gettime(timer_file_descriptor, &itval) == -1)
        return CTimerState_Error;

    if((!itval.it_value.tv_sec && !itval.it_value.tv_nsec) || (!itval.it_interval.tv_sec && !itval.it_interval.tv_nsec))
        return CTimerState_Stopped;
    else
        return CTimerState_Running;



}

bool CTimer::isSet(TimeoutRecipient_t _testListener)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
    {
        if (listeners.at(i) == _testListener)
        {
            return true;
        }
    }
    return false;
}


#ifdef QT_CORE_LIB
void CTimer::readyRead(){
#else
void CTimer::readyRead(int fd){
    //READ FILE DESCRIPTOR
    if (fd != timer_file_descriptor)
    {
        std::cout << "[ERROR][CTimer] Błąd synchronizacji kolejki!" << std::endl;
        return;
    }
#endif

    int i = read(timer_file_descriptor, &missed, sizeof(missed));
    (void)i;
    for (TimeoutRecipient_t val : listeners)
    {
        val.Invoke(); // wywołanie callbacka
    }

    if (_singleShot == SST_STATIC_RUN)
    {
        delete this;
    }

}

