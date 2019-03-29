#pragma once

#include "cdelegate.h"

#ifdef QT_CORE_LIB
#include <QSocketNotifier>
#include <QObject>
#else
#include "epoll.hpp"
#endif

#include <sys/types.h>
#include <sys/socket.h>
//#include <arpa/inet.h>
#include <sys/un.h>

#include <string>
#include <vector>
#include <list>

using std::string;

#ifdef QT_CORE_LIB
class AUnixDatagramSocket: public QObject
{
    Q_OBJECT
#else
class AUnixDatagramSocket
{
#endif
    typedef CDelegate<void> ReadDataRecipient_t;
public:
    AUnixDatagramSocket();
    ~AUnixDatagramSocket();

    bool Bind(const char * listenFileName);
    void Close();
    int getFD();

    int64_t readDatagram(std::vector<char> * data);
    int64_t writeDatagram(const char *nazwaPliku, const char *data, int64_t size);

    template <void (*Function)()>
    void connect()
    {
        ReadDataRecipient_t newListenner;
        newListenner.Bind<Function>();
        if (!isSet(newListenner)) listeners.push_back(newListenner);
    }

    template<void (*Function)()>
    void disconnect()
    {
        ReadDataRecipient_t testListener;
        testListener.Bind<Function>();
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

    template<class Typ, void (Typ::*Function)()>
    void connect(Typ *instance)
    {
        ReadDataRecipient_t newListener;
        newListener.Bind<Typ, Function>(instance);
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    template<class Typ, void (Typ::*Function)()>
    void disconnect(Typ *instance)
    {
        ReadDataRecipient_t testListener;
        testListener.Bind<Typ, Function>(instance);
        for (unsigned int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

    void PublicReadyReadSlot(int file_fd);

#ifdef QT_CORE_LIB
    private slots:
    void readyReadSlot(int fd);
#else
private:
    void readyReadSlot(int fd);
#endif

private:
    bool connectFileDescriptorToMainLoop();\
    bool isSet(ReadDataRecipient_t _testListener);
    int ret_val;

    struct sockaddr_un bind_Address;
    struct sockaddr_un send_Address;
    struct sockaddr_un recvfrom_Address;

    socklen_t bind_addr_size;
    socklen_t send_addr_size;
    socklen_t recvfrom_addr_size;

#define BUFLEN 65000
    char first_socket_rx_buffer[BUFLEN];
    int first_socket = -1;

    std::vector<ReadDataRecipient_t> listeners;

#ifdef QT_CORE_LIB
    QSocketNotifier * socketNotifier;
#endif

};

