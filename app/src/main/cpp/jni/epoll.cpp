#include "epoll.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

#define EPOLL_MAX_EVENTS 64
void print_epoll_ctl_error(int errorCode);

pthread_mutex_t mutex_map = PTHREAD_MUTEX_INITIALIZER;

std::map<pid_t, Epoll *> Epoll::epoll_map;


Epoll::Epoll(bool _verbose)
{
    thread_id = syscall(SYS_gettid);
    pthread_mutex_lock( &mutex_map);
    {
        epoll_map[thread_id] = this;
    }
    pthread_mutex_unlock( &mutex_map);

    init(_verbose);
}

Epoll::~Epoll()
{
    pthread_mutex_lock( &mutex_map);
    auto it = epoll_map.find(syscall(SYS_gettid));
    if (it != epoll_map.end())
    {
        epoll_map.erase(it);
    }
    pthread_mutex_unlock( &mutex_map);

}



void Epoll::init(bool _verbose)
{
    verbose = _verbose;
    _epoll_fd = epoll_create(100);
    if (_epoll_fd == -1)
    {
        if (verbose) std::cout << "[ERROR][EPOLL | init] Nie udało się otworzyć epoll'a" << std::endl;
        exit(EXIT_FAILURE);
    }
}

static struct epoll_event event_ctl;

bool Epoll::addEpollClient(int fileDesc)
{
    pthread_mutex_lock( &mutex_map);
    Epoll * epoll_object = epoll_map[syscall(SYS_gettid)];
    pthread_mutex_unlock( &mutex_map);


    if (epoll_object->_epoll_fd == -1)
    {
        if (epoll_object->verbose) std::cout << "[WARNING][EPOLL | addClient] Nie zainicjalizowano Epoll'a" << std::endl;
        return false;
    }

    event_ctl.data.fd = fileDesc;
    event_ctl.events = EPOLLIN;

    if (epoll_ctl(epoll_object->_epoll_fd, EPOLL_CTL_ADD, fileDesc, &event_ctl) == -1)
    {
        int error_code_for_epoll = errno;
        if (epoll_object->verbose) std::cout << "[ERROR][EPOLL | addClient] Nie można dodać klienta do Epoll'a fd = " << fileDesc << " fd = " << epoll_object->_epoll_fd << std::endl;
        print_epoll_ctl_error(error_code_for_epoll);

        //exit(EXIT_FAILURE);
        return false;
    }

    return true;
}



void Epoll::removeClient(int fileDesc)
{
    pthread_mutex_lock( &mutex_map);
    Epoll * epoll_object = epoll_map[syscall(SYS_gettid)];
    pthread_mutex_unlock( &mutex_map);

    EpollRecipientsMap_t::iterator it;
    it = epoll_object->recipientsMap.find(fileDesc);
    if (it != epoll_object->recipientsMap.end())
    {
        epoll_ctl(epoll_object->_epoll_fd, EPOLL_CTL_DEL, fileDesc, NULL);
        epoll_object->recipientsMap.erase(it);
        if (epoll_object->verbose) std::cout << "[INFO][EPOLL | runApp] Usunięto odbiorcę fd = " << fileDesc << " z kolejki epoll!" << std::endl;
    }
}

int Epoll::runApp()
{

    int fileDesc_nums = -1;
    struct epoll_event epoll_events[EPOLL_MAX_EVENTS];

    if (_epoll_fd == -1)
    {
        if (verbose) std::cout << "[ERROR][EPOLL | runApp] Nie zainicjalizowano kolejki!" << std::endl;
        run = false;
        return -1;
    }

    if (recipientsMap.size() == 0)
    {
        if (verbose) std::cout << "[INFO][EPOLL | runApp] Nie zarejestrowano odbiorców!" << std::endl;
        run = false;
        return -1;
    }

    run = true;

    while(run) {
        fileDesc_nums = epoll_wait(_epoll_fd, epoll_events, EPOLL_MAX_EVENTS, -1);

        //        if(verbose)
        //        {
        //            static unsigned long epoll_wait_iteration = 0;
        //            //if((epoll_wait_iteration % 100) == 0){
        //                cout << "epoll_wait_iteration = " << epoll_wait_iteration++ << endl;
        //            //}
        //        }

        //cout << "Epoll descriptor count = " << fileDesc_nums << endl;


        for (int i = 0; i < fileDesc_nums; i++)
        {
            if ((epoll_events[i].events & EPOLLERR) || (epoll_events[i].events & EPOLLHUP) || (!(epoll_events[i].events & EPOLLIN)))
            {
                cout << "[ERROR][EPOLL | runApp] EPOLLERR EPOLLHUP EPOLLIN on fd = " << epoll_events[i].data.fd << endl;
                cout.flush();
                exit(EXIT_FAILURE);
                continue;
            }

            if (recipientsMap.find(epoll_events[i].data.fd) != recipientsMap.end())
            {
                if (verbose) std::cout << "[EPOLL | eventFrom] fd = " << epoll_events[i].data.fd << std::endl;
                recipientsMap[epoll_events[i].data.fd](epoll_events[i].data.fd);
            }
            else
            {
                if (verbose) std::cout << "[WARNING][EPOLL | runApp] Nie odnaleziono odbiorcy dla zdarzenia fd = " << epoll_events[i].data.fd << std::endl;
            }
        }
    }

    if (verbose) std::cout << "[INFO][EPOLL | runApp] Kończenie działania kolejki epoll!" << std::endl;

    for (EpollRecipientsMap_t::iterator it = recipientsMap.begin(); it != recipientsMap.end(); it++)
    {
        epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
        if (verbose) std::cout << "[INFO][EPOLL | runApp] Usunięto odbiorcę fd = " << it->first << " z kolejki epoll!" << std::endl;
        recipientsMap.erase(it);
    }

    close(_epoll_fd);
    _epoll_fd = -1;
    return exit_code;
}

void Epoll::exit(int code)
{
    pthread_mutex_lock( &mutex_map);
    Epoll * epoll_object = epoll_map[syscall(SYS_gettid)];
    pthread_mutex_unlock( &mutex_map);

    epoll_object->exit_code = code;
    epoll_object->run = false;
}

void print_epoll_ctl_error(int errorCode)
{
    switch(errorCode){
    case EBADF:
        cout << "   EBADF   epfd or fd is not a valid file descriptor." << endl;
        break;

    case EEXIST:
        cout << "   EEXIST  op was EPOLL_CTL_ADD, and the supplied file descriptor fd is " << endl;
        cout << "           already registered with this epoll instance." << endl;
        break;

    case EINVAL:
        cout << "   EINVAL  epfd is not an epoll file descriptor, or fd is the same as" << endl;
        cout << "           epfd, or the requested operation op is not supported by this" << endl;
        cout << "           interface." << endl;
        cout << endl;

        cout << "   EINVAL  An invalid event type was specified along with EPOLLEXCLUSIVE" << endl;
        cout << "           in events." << endl;
        cout << endl;

        cout  << "  EINVAL  op was EPOLL_CTL_MOD and events included EPOLLEXCLUSIVE." << endl;
        cout << endl;

        cout << "   EINVAL  op was EPOLL_CTL_MOD and the EPOLLEXCLUSIVE flag has" << endl;
        cout << "           previously been applied to this epfd, fd pair." << endl;
        cout << endl;

        cout << "   EINVAL  EPOLLEXCLUSIVE was specified in event and fd refers to an" << endl;
        cout << "           epoll instance." << endl;
        cout << endl;

        break;

    case ELOOP:
        cout << "   ELOOP   fd refers to an epoll instance and this EPOLL_CTL_ADD" << endl;
        cout << "           operation would result in a circular loop of epoll instances" << endl;
        cout << "           monitoring one another." << endl;
        break;

    case ENOENT:
        cout << "   ENOENT  op was EPOLL_CTL_MOD or EPOLL_CTL_DEL, and fd is not" << endl;
        cout << "           registered with this epoll instance." << endl;
        break;

    case ENOMEM:
        cout << "   ENOMEM  There was insufficient memory to handle the requested op" << endl;
        cout << "           control operation." << endl;

        break;

    case ENOSPC:
        cout << "   ENOSPC  The limit imposed by /proc/sys/fs/epoll/max_user_watches was" << endl;
        cout << "           encountered while trying to register (EPOLL_CTL_ADD) a new" << endl;
        cout << "           file descriptor on an epoll instance.  See epoll(7) for" << endl;
        cout << "           further details." << endl;

        break;

    case  EPERM:
        cout << "   EPERM   The target file fd does not support epoll.  This error can" << endl;
        cout << "           occur if fd refers to, for example, a regular file or a" << endl;
        cout << "           directory." << endl;
        break;
    }
}


