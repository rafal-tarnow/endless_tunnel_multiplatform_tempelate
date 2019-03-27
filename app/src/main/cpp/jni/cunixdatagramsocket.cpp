#include "cunixdatagramsocket.h"
#include <string.h>
#include <fcntl.h>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include "epoll.hpp"

using std::cout;
using std::endl;

CUnixDatagramSocket::CUnixDatagramSocket(char * listenFileName)
{
    //UNLINK file
    unlink(listenFileName);

    //CREATE SOCKET FILE DESCRIPTOR
    if ((first_socket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1){
        std::cout << "[Error | CUdpUnixSocket] failed to create udp socket" << std::endl;
    }else{
        std::cout << "[Info | CUdpUnixSocket] sucesfull created socket" << std::endl;
    }

    //DEFINE SEND HOST
    lenght_ofAdressStruct = sizeof(serverAddress);

    memset((char *) &serverAddress, 0, sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strcpy(serverAddress.sun_path, listenFileName);

    memset((char *) &clientAddress, 0, sizeof(clientAddress));
    clientAddress.sun_family = AF_UNIX;
}

CUnixDatagramSocket::~CUnixDatagramSocket(){
    this->Close();
    listeners.clear();
}

bool CUnixDatagramSocket::Bind()
{
    ret_val = bind(first_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    if(ret_val == 0){
        cout << "[Info | CUdpUnixSocket] sucesfull bind socket" << endl;
        connectFileDescriptorToMainLoop();
        return true;
    }else if(ret_val == -1){
        cout << "[Error | CUdpUnixSocket] failed to bind udp socket" << endl;
        return false;
    }
    return false;
}

void CUnixDatagramSocket::readyReadSlot(int fd)
{
    if( fd != first_socket)
        return;

    for(ReadDataRecipient_t val : listeners)
    {
        val.Invoke();
    }
}

bool CUnixDatagramSocket::connectFileDescriptorToMainLoop(){
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
    if (!Epoll::addClient<CUnixDatagramSocket, &CUnixDatagramSocket::readyReadSlot>(first_socket, this))
    {
        return false;
    }
#endif
    return true;
}

int64_t CUnixDatagramSocket::readDatagram(std::vector<char> *data)
{
    int recv_len = recvfrom(first_socket, first_socket_rx_buffer, BUFLEN, 0, (struct sockaddr *)&clientAddress, &lenght_ofAdressStruct);
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

int64_t CUnixDatagramSocket::writeDatagram(char * nazwaPliku, const char *data, int64_t size)
{
    strcpy(clientAddress.sun_path, nazwaPliku);
    ret_val = sendto(first_socket, data, size, 0, (struct sockaddr*)&clientAddress, lenght_ofAdressStruct);

    if(ret_val == -1){
        //cout << "[Error | CUdpSocket] failed sendto data via udp socket" << endl;
        return -1;
    }else{
        //cout << "[Info | CUdpSocket] sucessfull sendto data via udp socket" << endl;
        return ret_val;
    }
    return -1;
}


void CUnixDatagramSocket::Close(){
    close(first_socket);
    Epoll::removeClient(first_socket);
}

bool CUnixDatagramSocket::isSet(ReadDataRecipient_t _testListener)
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

