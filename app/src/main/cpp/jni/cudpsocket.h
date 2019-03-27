#ifndef CUDPSOCKET_H
#define CUDPSOCKET_H

#include "cdelegate.h"


#ifdef QT_CORE_LIB
#include <QSocketNotifier>
#include <QObject>
#else
#include "epoll.hpp"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#include <string>
#include <vector>
#include <list>

using std::string;

/*********************** EXAMPLE **********************

//header
CUdpSocket * udpSocket = nullptr;

void Widget::Widget(){
   udpSocket = new CUdpSocket("192.168.103.97", 7001);
}

void Widget::~Widget(){
    if(udpSocket)
    {
        delete udpSocket;
        udpSocket = nullptr;
    }
}

void Widget::dataFromCUdpSocket(){
    uint32_t senderIP = 0;
    uint16_t senderPort = 0;

    std::vector< char> buffer;
    udpSocket->readDatagram(&buffer,&senderIP,&senderPort);

    cout << "   senderIP = " << CUdpSocket::convert_IP4(senderIP) << endl;
    cout << "   senderPort = " << senderPort << endl;
}

void Widget::on_pushButton_sendData_clicked(){
    char buffer[5] = {1,2,3,4,5};
    uint16_t sendPort = 1234;
    udpSocket->writeDatagram(buffer,5,"127.0.0.1",sendPort);
}

void Widget::on_pushButton_connect_clicked()
{
    uint16_t listen_port = 1235;
    udpSocket->connect<Widget, &Widget::dataFromCUdpSocket>(this);
    udpSocket->Bind(listen_port, CUdpSocket::ShareAddress);
}

void Widget::on_pushButton_disconnect_clicked()
{
    udpSocket->Close();
    udpSocket->disconnect<Widget, &Widget::dataFromCUdpSocket>(this);
}

bool Widget::isOpen(){
    if(CUdpSocket::BoundState == udpSocket->state()){
        return true;
    }else{
        return false;
    }
}


********************************************************/

#ifdef QT_CORE_LIB
class CUdpSocket: public QObject
{
    Q_OBJECT
#else
class CUdpSocket
{
#endif
    typedef CDelegate<void > ReadDataRecipient_t;
public:
    typedef enum {
        DefaultForPlatform = 0x0,
        ShareAddress = 0x1,
        DontShareAddress = 0x2,
        ReuseAddressHint = 0x4
    } BindMode;

    typedef enum {
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ListeningState,
        ClosingState
    }SocketState;


    CUdpSocket(string sendHostAddress, uint16_t sendPort);
    CUdpSocket(const char * sendHostAddress, uint16_t sendPort);

    ~CUdpSocket();


    void setSendHost(std::string sendHostAddress);
    bool Bind(uint16_t listen_port, BindMode);
    void Close();

    int64_t readDatagram(std::vector<char> * data, uint32_t *senderIP_hostByteOrder, uint16_t *senderPort_hostByteOrder);


    int64_t writeDatagram(const char *data, int64_t size);
    int64_t writeDatagram(const char *data, int64_t size, std::string sendIP_hostByteOrder, uint16_t sendPort_hostByteOrder);
    int64_t writeDatagram(const char *data, int64_t size, uint32_t sendIP_hostByteOrder, uint16_t sendPort_hostByteOrder);

    SocketState state();

    template <void (*Function)()>
    void connect()
    {
        ReadDataRecipient_t newListener;
        newListener.Bind<Function>();
        if (!isSet(newListener)) listeners.push_back(newListener);
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

static uint32_t convert_IP4(const char * ip);
static char * convert_IP4(uint32_t);

#ifdef QT_CORE_LIB
private slots:
    void readyReadSlot(int fd);
#else
private:
    void readyReadSlot(int fd);
#endif

private:
    void init();
    bool connectFileDescriptorToMainLoop();
    bool isSet(ReadDataRecipient_t _testListener);
    void printBindError(int error);
    SocketState socketState = UnconnectedState;

    int ret_val;


#define BUFLEN 65000
    char first_socket_rx_buffer[BUFLEN];
    int socket_fd = -1;

    struct sockaddr_in bindSocketInternetAddress, sendSocketInternetAddress, receivedSockerInternetAddress;
    socklen_t lenght_ofAdressStruct;

    std::vector<ReadDataRecipient_t> listeners;

    uint16_t m_sendPort_hostByteOrder = 0;
    uint32_t m_sendIP_hostByteOrder = 0;

#ifdef QT_CORE_LIB
    QSocketNotifier * socketNotifier;
#endif

};

#endif // CUDPSOCKET_H

