#ifndef CTIMER_HPP
#define CTIMER_HPP

#include <stdint.h>
#include <vector>
#include <iostream>
#include "cdelegate.h"


#ifdef QT_CORE_LIB
#   include <QSocketNotifier>
#   include <QObject>
#else
#   ifdef USE_SELECT
#       include "select.hpp"
#   else
#       include "epoll.hpp"
#   endif
#endif


//EXAMPLE OF USE
//CTimer * timer;
//timer = new CTimer();
//timer->connect<Application, &Application::onTimer_1s>(this);
//timer->start(1000);
//delte timer;


#ifdef QT_CORE_LIB
class CTimer : public QObject{
    Q_OBJECT
#else
class CTimer{
#endif
    typedef CDelegate<void> TimeoutRecipient_t;
    typedef enum
    {
        SST_STATIC_RUN, /**< Timer singleShot uruchamiany przy pomocy funckji statycznej */
        SST_NORMAL_RUN, /**< Timer singleShot ustawiony na obiekcie przez użytkownika */
        SST_NONE        /**< Timer w trybie normal bez signleShot */
    }SingleShotType_t;

public:

    typedef enum{
        CTimerState_Error,
        CTimerState_Running,
        CTimerState_Stopped,
    }CTimerState;


    explicit CTimer();
    ~CTimer();
    void startSingleShot(unsigned int ms);
    void setSingleShot(bool val);
    void start(unsigned int ms);
    void stop();
    CTimerState isActive();

    template<class C, void (C::*Function)()>
    static CTimer * singleShot(int msec, C *instance)
    {
        CTimer *tmp = new CTimer();
        tmp->connect<C, Function>(instance);
        // tmp->_singleShot = SST_STATIC_RUN;
        tmp->startSingleShot(msec, SST_STATIC_RUN);
        return tmp;
    }

    template<void (*Function)()>
    static CTimer * singleShot(int msec)
    {
        CTimer *tmp = new CTimer();
        tmp->connect<Function>();
        //tmp->_singleShot = SST_STATIC_RUN;
        tmp->startSingleShot(msec, SST_STATIC_RUN);
        return tmp;
    }

    template <void (*Function)()>
    void connect()
    {
        TimeoutRecipient_t newListener;
        newListener.Bind<Function>();
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    template<void (*Function)()>
    void disconnect()
    {
        TimeoutRecipient_t testListener;
        testListener.Bind<Function>();
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

    template<class C, void (C::*Function)()>
    void connect(C *instance)
    {
        TimeoutRecipient_t newListener;
        newListener.Bind<C, Function>(instance);
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    template<class C, void (C::*Function)()>
    void disconnect(C *instance)
    {
        TimeoutRecipient_t testListener;
        testListener.Bind<C, Function>(instance);
        for (unsigned int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

#ifdef QT_CORE_LIB
private slots:
    void readyRead();
#else
private:
    void readyRead(int fd);
#endif



private:
    std::vector<TimeoutRecipient_t> listeners;/**< wektor odbiorców timeoutu */
    unsigned int tmp_second = 0;
    unsigned int tmp_nano_second = 0;
    SingleShotType_t _singleShot = SST_NONE;
    bool _is_singleShot = false;
    uint64_t missed;
    struct itimerspec itval;
    int timer_file_descriptor;
    //bool _is_timerActive;


    bool isSet(TimeoutRecipient_t _testListener);
    void startSingleShot(unsigned int ms, SingleShotType_t type);

#ifdef QT_CORE_LIB
    QSocketNotifier * socketNotifier;
#endif


};


#endif // CTIMER_HPP
