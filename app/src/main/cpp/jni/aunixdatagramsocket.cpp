#include "aunixdatagramsocket.h"
#include <string.h>
#include <fcntl.h>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include "epoll.hpp"

using std::cout;
using std::endl;

AUnixDatagramSocket::AUnixDatagramSocket()
{
    //CREATE SOCKET FILE DESCRIPTOR
    if ((first_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
        std::cout << "[Error | CUdpUnixSocket] failed to create udp socket" << std::endl;
    }else{
        std::cout << "[Info | CUdpUnixSocket] sucesfull created socket" << std::endl;
    }

    //REMOTE
    memset((char *) &send_Address, 0, sizeof(struct sockaddr_un));
    send_Address.sun_family = AF_UNIX;
    send_addr_size = sizeof(struct sockaddr_un);

    //BIND
    memset((char *) &bind_Address, 0, sizeof(struct sockaddr_un));
    bind_Address.sun_family = AF_UNIX;
    bind_addr_size = sizeof(struct sockaddr_un);

    //RECVFROM
    memset((char*) &recvfrom_Address, 0, sizeof(struct sockaddr_un));
    recvfrom_addr_size = sizeof(struct sockaddr_un);
}

AUnixDatagramSocket::~AUnixDatagramSocket(){
    this->Close();
    listeners.clear();
}

bool AUnixDatagramSocket::Bind(const char * listenFileName)
{
    bool mIsAbstractSocketNamespace;

    if(listenFileName[0] == '\0')
        mIsAbstractSocketNamespace = true;
    else
        mIsAbstractSocketNamespace = false;


    //DEFINE SEND HOST
    bind_addr_size = sizeof(struct sockaddr_un);

    memset((char *) &bind_Address, 0, sizeof(bind_Address));
    bind_Address.sun_family = AF_UNIX;

    if(!mIsAbstractSocketNamespace)
    {
        strncpy(bind_Address.sun_path, listenFileName, strlen(listenFileName));
    }
    else
    {
        bind_Address.sun_path[0] = '\0';
        strncpy(&(bind_Address.sun_path[1]), &(listenFileName[1]), strlen(&(listenFileName[1])));
    }

    //UNLINK file
    unlink(listenFileName);

    ret_val = bind(first_socket, (struct sockaddr*)&bind_Address, bind_addr_size);

    if(ret_val == 0){
        cout << "[Info | CUdpUnixSocket] sucesfull bind socket" << endl;
        //connectFileDescriptorToMainLoop();
        return true;
    }else if(ret_val == -1){
        cout << "[Error | CUdpUnixSocket] failed to bind udp socket" << endl;
        return false;
    }
    return false;
}

void AUnixDatagramSocket::PublicReadyReadSlot(int file_fd)
{
    readyReadSlot(file_fd);
}

void AUnixDatagramSocket::readyReadSlot(int fd)
{
    if( fd != first_socket)
        return;

    for(ReadDataRecipient_t val : listeners)
    {
        val.Invoke();
    }
}

bool AUnixDatagramSocket::connectFileDescriptorToMainLoop(){
#ifdef QT_CORE_LIB
    socketNotifier = new QSocketNotifier(first_socket, QSocketNotifier::Read);
    if (!socketNotifier)
    {
        return false;
    }
    QObject::connect(socketNotifier, SIGNAL(activated(int)), this, SLOT(readyReadSlot(int)));
    if (!socketNotifier->isEnabled())
    {
        return false;
    }
#else
    if (!Epoll::addClient<AUnixDatagramSocket, &AUnixDatagramSocket::readyReadSlot>(first_socket, this))
    {
        return false;
    }
#endif
    return true;
}

int64_t AUnixDatagramSocket::readDatagram(std::vector<char> *data)
{
    int recv_len = recvfrom(first_socket, first_socket_rx_buffer, BUFLEN, 0, (struct sockaddr *)&recvfrom_Address, &recvfrom_addr_size);
    //cout << clientAddress.sun_path;
    if(recv_len == -1){
        //cout << "Read Error " << endl;
        int errsv = errno;
        if(errsv == EAGAIN){
            //cout << "EAGAIN" << endl;
        }else if(errsv == EWOULDBLOCK){
            //cout << "EWOULDBLOCK" << endl;
        }else{

        }
    }else{
        //correcr read data
        for(int i =0; i < recv_len; i++){
            data->push_back(first_socket_rx_buffer[i]);
        }
    }
    return -1;
}

int64_t AUnixDatagramSocket::writeDatagram(const char * nazwaPliku, const char *data, int64_t size)
{
    bool mIsAbstractSocketNamespace;

    if(nazwaPliku[0] == '\0')
        mIsAbstractSocketNamespace = true;
    else
        mIsAbstractSocketNamespace = false;

    memset(send_Address.sun_path,0,sizeof(send_Address.sun_path));
    if(!mIsAbstractSocketNamespace)
    {
        strncpy(send_Address.sun_path, nazwaPliku, strlen(nazwaPliku));
    }
    else
    {
        send_Address.sun_path[0] = '\0';
        strncpy(&(send_Address.sun_path[1]), &(nazwaPliku[1]) ,strlen(&(nazwaPliku[1])));
    }

    ret_val = sendto(first_socket, data, size, 0, (struct sockaddr*)&send_Address, send_addr_size);

    if(ret_val == -1){
        //cout << "[Error | CUdpSocket] failed sendto data via udp socket" << endl;
        return -1;
    }else{
        //cout << "[Info | CUdpSocket] sucessfull sendto data via udp socket" << endl;
        return ret_val;
    }
    return -1;
}

int AUnixDatagramSocket::getFD()
{
    return first_socket;
}

void AUnixDatagramSocket::Close(){
    Epoll::removeClient(first_socket);
    close(first_socket);
}

bool AUnixDatagramSocket::isSet(ReadDataRecipient_t _testListener)
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

