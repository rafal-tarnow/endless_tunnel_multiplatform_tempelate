#ifndef CUNIXDATAGRAMSOCKET_H
#define CUNIXDATAGRAMSOCKET_H

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
class CUnixDatagramSocket: public QObject
{
    Q_OBJECT
#else
class CUnixDatagramSocket
{
#endif
    typedef CDelegate<void> ReadDataRecipient_t;
public:
    CUnixDatagramSocket(const char *listenFileName, bool isAbstract = false);
    ~CUnixDatagramSocket();

    bool Bind();
    void Close();

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

    struct sockaddr_un serverAddress;
    struct sockaddr_un clientAddress;

#define BUFLEN 65000
    char first_socket_rx_buffer[BUFLEN];
    int first_socket = -1;

    bool mIsAbstractSocketNamespace = false;
    socklen_t lenght_ofAdressStruct;

    std::vector<ReadDataRecipient_t> listeners;


#ifdef QT_CORE_LIB
    QSocketNotifier * socketNotifier;
#endif

};

#endif // CUNIXDATAGRAMSOCKET_H
