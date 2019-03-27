#include "cudpsocket.h"
#include<string.h>
#include <fcntl.h>
#include <iostream>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include "epoll.hpp"

using std::cout;
using std::endl;


static int make_socket_non_blocking (int sfd)
{
    int flags, s;

    flags = fcntl (sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror ("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror ("fcntl");
        return -1;
    }

    return 0;
}

CUdpSocket::CUdpSocket(string sendHostAddress, uint16_t sendPort)
{
    m_sendPort_hostByteOrder = sendPort;
    m_sendIP_hostByteOrder = CUdpSocket::convert_IP4(sendHostAddress.c_str());
    init();
}


CUdpSocket::CUdpSocket(const char * sendHostAddress, uint16_t sendPort)
{
    m_sendPort_hostByteOrder = sendPort;
    m_sendIP_hostByteOrder = CUdpSocket::convert_IP4(sendHostAddress);
    init();
}

void CUdpSocket::init()
{
    //CREATE SOCKET FILE DESCRIPTOR
    if ((socket_fd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        std::cout << "[Error | CUdpSocket] failed to create udp socket" << std::endl;
    }else{
        std::cout << "[Info | CUdpSocket] sucesfull created socket with fd = " << socket_fd << std::endl;
    }

    //MAKE SOCKET NONBLOCKING
    ret_val = make_socket_non_blocking (socket_fd);

    if(ret_val == -1){
        std::cout << "[Error | CUdpSocket] failed to make nonblocking socket" << std::endl;
    }else if(ret_val == 0){
        std::cout << "[Info | CUdpSocket] sucesfull make nonblocking socket" << std::endl;
    }


    //DEFINE SEND HOST
    lenght_ofAdressStruct = sizeof(sendSocketInternetAddress);

    memset((char *) &sendSocketInternetAddress, 0, sizeof(sendSocketInternetAddress));
    sendSocketInternetAddress.sin_family = AF_INET;
    sendSocketInternetAddress.sin_port = htons(m_sendPort_hostByteOrder);
    sendSocketInternetAddress.sin_addr.s_addr = htonl(m_sendIP_hostByteOrder);

    //CLEAR RECEIVED SOCKET ADDRESS
    memset((char *) &receivedSockerInternetAddress, 0, sizeof(receivedSockerInternetAddress));
}

CUdpSocket::~CUdpSocket(){
    this->Close();
    listeners.clear();
}

void CUdpSocket::setSendHost(std::string sendHostAddress)
{
    m_sendIP_hostByteOrder = CUdpSocket::convert_IP4(sendHostAddress.c_str());
    sendSocketInternetAddress.sin_addr.s_addr = htonl(m_sendIP_hostByteOrder);
}

bool CUdpSocket::Bind(uint16_t listen_port, BindMode){

    if(socket_fd == -1)
        init();

    if(socketState != UnconnectedState){
        cout << "[Warning | CUdpSocket] socket arleady binded" ;
    }

    memset((char *) &bindSocketInternetAddress, 0, sizeof(bindSocketInternetAddress));
    bindSocketInternetAddress.sin_family = AF_INET;
    bindSocketInternetAddress.sin_port = htons(listen_port);
    bindSocketInternetAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    ret_val = bind(socket_fd , (struct sockaddr*)&bindSocketInternetAddress, sizeof(bindSocketInternetAddress));
    int error = errno;
    printBindError(error);
    if(ret_val  == 0){
        cout << "[Info | CUdpSocket] sucesfull bind socket on port " << listen_port << endl;
        socketState = BoundState;
        connectFileDescriptorToMainLoop();
        return true;
    }else if(ret_val == -1){
        cout << "[Error | CUdpSocket] failed to bind udp socket on port " << listen_port  << endl;
        socketState = UnconnectedState;
        return false;
    }

    return false; //qt default error value
}

void CUdpSocket::printBindError(int error)
{
    switch(error)
    {
    case EACCES:
        cout << "EACCES: The address is protected, and the user is not the superuser." << endl << endl;

        cout << "EACCES: Search permission is denied on a component of the path prefix. \
                (See also path_resolution(7).)" << endl;
                break;
    case EADDRINUSE:
        cout << "EADDRINUSE: The given address is already in use." << endl << endl;

        cout << "EADDRINUSE: (Internet domain sockets) The port number was specified as \
                zero in the socket address structure, but, upon attempting to \
                bind to an ephemeral port, it was determined that all port \
                numbers in the ephemeral port range are currently in use.  See \
                the discussion of /proc/sys/net/ipv4/ip_local_port_range \
                ip(7)." << endl;
                break;
    case EBADF:
        cout << "EBADF: sockfd is not a valid file descriptor." << endl;
        break;
    case EINVAL:
        cout << "EINVAL: The socket is already bound to an address." << endl << endl;

        cout << "addrlen is wrong, or addr is not a valid address for this \
                socket's domain." << endl;
                break;
    case ENOTSOCK:
        cout << "ENOTSOCK: The file descriptor sockfd does not refer to a socket." << endl;
        break;

    case EADDRNOTAVAIL:

        cout << "EADDRNOTAVAIL: A nonexistent interface was requested or the requested address \
                was not local." << endl;
                break;
    case EFAULT:
        cout << "EFAULT: addr points outside the user's accessible address space." << endl;
        break;
    case ELOOP:
        cout << "ELOOP: Too many symbolic links were encountered in resolving addr." << endl;
        break;
    case ENAMETOOLONG:
        cout << "ENAMETOOLONG: addr is too long." << endl;
        break;
    case ENOENT:
        cout << "ENOENT: A component in the directory prefix of the socket pathname \
                does not exist." << endl;
                break;
    case ENOMEM:
        cout << "ENOMEM: Insufficient kernel memory was available." << endl;
        break;
    case ENOTDIR:

        cout << "ENOTDIR: A component of the path prefix is not a directory." << endl;
        break;
    case EROFS:
        cout << "EROFS: The socket inode would reside on a read-only filesystem." << endl;
        break;
    }
}

void CUdpSocket::readyReadSlot(int fd)
{
    //static int l = 0;
    //cout << "CUdpSocket::readyReadSlot " << l++ << endl;

    if (fd != socket_fd) return;


    for (ReadDataRecipient_t val : listeners)
    {
        val.Invoke();
    }

}

bool CUdpSocket::connectFileDescriptorToMainLoop(){
#ifdef QT_CORE_LIB
    socketNotifier = new QSocketNotifier(socket_fd, QSocketNotifier::Read);
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
#   ifdef USE_SELECT
    if (!Select::addClient<CUdpSocket, &CUdpSocket::readyReadSlot>(socket_fd, this))
    {
        return false;
    }
#   else
    if (!Epoll::addClient<CUdpSocket, &CUdpSocket::readyReadSlot>(socket_fd, this))
    {
        return false;
    }
#   endif
#endif
    return true;
}

int64_t CUdpSocket::readDatagram(std::vector<char> * data, uint32_t *senderIP, uint16_t *senderPort){

    int first_recv_len = recvfrom(socket_fd, first_socket_rx_buffer, BUFLEN, 0, (struct sockaddr *) &receivedSockerInternetAddress, &lenght_ofAdressStruct);
    *senderPort = ntohs(receivedSockerInternetAddress.sin_port);
    *senderIP = ntohl(receivedSockerInternetAddress.sin_addr.s_addr);
    if(first_recv_len == -1){
        //cout << "Read Error " << endl;
        int errsv = errno;
        if(errsv == EAGAIN){
            //cout << "EAGAIN" << endl;
        }else if(errsv == EWOULDBLOCK){
            //cout << "EWOULDBLOCK" << endl;
        }else{

        }
    }else{
        //odczytano poprawnie dane
        for(int i =0; i < first_recv_len; i++){
            data->push_back(first_socket_rx_buffer[i]);
        }
    }

    return -1; //qt default error value
}

int64_t CUdpSocket::writeDatagram(const char *data, int64_t size){

    ret_val = sendto(socket_fd, data, size, 0, (struct sockaddr*) &sendSocketInternetAddress, lenght_ofAdressStruct);

    if(ret_val == -1){
        //cout << "[Error | CUdpSocket] failed sendto data via udp socket" << endl;
        return -1;
    }else{
        //cout << "[Info | CUdpSocket] sucessfull sendto data via udp socket" << endl;
        return ret_val;
    }

    return -1; // qt default error value
}

int64_t CUdpSocket::writeDatagram(const char *data, int64_t size, std::string sendIP_hostByteOrder, uint16_t sendPort_hostByteOrder)
{
    return writeDatagram(data, size, convert_IP4(sendIP_hostByteOrder.c_str()), sendPort_hostByteOrder);
}


int64_t CUdpSocket::writeDatagram(const char *data, int64_t size, uint32_t sendIP_hostByteOrder, uint16_t sendPort_hostByteOrder){

    if((sendIP_hostByteOrder != m_sendIP_hostByteOrder) || (sendPort_hostByteOrder != m_sendPort_hostByteOrder)){
        m_sendIP_hostByteOrder = sendIP_hostByteOrder;
        m_sendPort_hostByteOrder = sendPort_hostByteOrder;

        sendSocketInternetAddress.sin_port = htons(m_sendPort_hostByteOrder);
        sendSocketInternetAddress.sin_addr.s_addr = htonl(m_sendIP_hostByteOrder);
    }

    ret_val = sendto(socket_fd, data, size, 0, (struct sockaddr*) &sendSocketInternetAddress, lenght_ofAdressStruct);

    if(ret_val == -1){
        //cout << "[Error | CUdpSocket] failed sendto data via udp socket" << endl;
        return -1;
    }else{
        //cout << "[Info | CUdpSocket] sucessfull sendto data via udp socket" << endl;
        return ret_val;
    }

    return -1; // qt default error value
}

void CUdpSocket::Close(){
    if(socket_fd != -1)
    {
        close(socket_fd);
#ifdef QT_CORE_LIB
        socketNotifier->disconnect();
        delete socketNotifier;
#else
#   ifdef USE_SELECT
        Select::removeClient(socket_fd);
#   else
        Epoll::removeClient(socket_fd);
#   endif
#endif
        socket_fd = -1;
        //shutdown(first_socket,);
        socketState = UnconnectedState;
    }
}


CUdpSocket::SocketState CUdpSocket::state(){
    return socketState;
}

uint32_t CUdpSocket::convert_IP4(const char * ip)
{
    struct in_addr ip_addr;
    if (inet_aton(ip, &ip_addr) == 0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }
    return ntohl(ip_addr.s_addr);
}

char * CUdpSocket::convert_IP4(uint32_t ip)
{
    struct in_addr ip_addr;
    ip_addr.s_addr = htonl(ip);
    return inet_ntoa(ip_addr);
}

bool CUdpSocket::isSet(ReadDataRecipient_t _testListener)
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
