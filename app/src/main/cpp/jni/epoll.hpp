#pragma once

#include <iostream>
#include <map>
#include <sys/syscall.h>
#include <unistd.h>

#include "cdelegate.h"

#define EPOLL_EXIT_SUCCESS 0
#define EPOLL_EXIT_FAILURE -1

class EpollClient;

class Epoll
{
    typedef CDelegate<void, int> EpollRecipient_t;
    typedef std::map<int, EpollRecipient_t> EpollRecipientsMap_t;

public:
    Epoll(bool _verbose);
    ~Epoll();
    template <class C, void (C::*Function)(int)> static bool addClient(int fileDesc, C *instance);

    template <void (*Function)(int)> static bool addClient(int fileDesc);
    static void removeClient(int fileDesc);

    int runApp();
    static void exit(int code = EPOLL_EXIT_SUCCESS);

private:
    bool verbose = true;
    int _epoll_fd = -1;
    int exit_code = EPOLL_EXIT_SUCCESS;
    bool run = true;
    EpollRecipientsMap_t recipientsMap;
    pid_t thread_id;

    static std::map<pid_t, Epoll *> epoll_map;
    static pthread_mutex_t mutex_threadSave;

    void init(bool _verbose = false);

    static bool addEpollClient(int fileDesc);
};

template <class C, void (C::*Function)(int)> bool Epoll::addClient(int fileDesc, C *instance)
{
    pthread_mutex_lock( &mutex_threadSave);
    Epoll *epoll_object = epoll_map[syscall(SYS_gettid)];
    pthread_mutex_unlock( &mutex_threadSave);

    EpollRecipient_t newRecipient;
    if (epoll_object->recipientsMap.find(fileDesc) != epoll_object->recipientsMap.end())
    {
        if (epoll_object->verbose)
            std::cout << "[WARNING][EPOLL | addClient] Klient fd = " << fileDesc << " jest już dodany!" << std::endl;
        return false;
    }

    if (addEpollClient(fileDesc))
    {
        newRecipient.Bind<C, Function>(instance);
        epoll_object->recipientsMap[fileDesc] = newRecipient;
        if (epoll_object->verbose)
            std::cout << "[INFO][EPOLL  | addCLient] Klient fd = " << fileDesc << " został poprawnie dodany do kolejki Epoll." << std::endl;
        return true;
    }
    return false;
}

template <void (*Function)(int)> bool Epoll::addClient(int fileDesc)
{
    pthread_mutex_lock( &mutex_threadSave);
    Epoll *epoll_object = epoll_map[syscall(SYS_gettid)];
    pthread_mutex_unlock( &mutex_threadSave);

    EpollRecipient_t newRecipient;
    if (epoll_object->recipientsMap.find(fileDesc) != epoll_object->recipientsMap.end())
    {
        if (epoll_object->verbose)
            std::cout << "[WARNING][EPOLL | addClient] Klient fd = " << fileDesc << " jest już dodany!" << std::endl;
        return false;
    }

    if (addEpollClient(fileDesc))
    {
        newRecipient.Bind<Function>();
        epoll_object->recipientsMap[fileDesc] = newRecipient;
        if (epoll_object->verbose)
            std::cout << "[INFO][EPOLL  | addCLient] Klient fd = " << fileDesc << " został poprawnie dodany do kolejki Epoll." << std::endl;
        return true;
    }
    return false;
}

