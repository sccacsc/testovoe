#ifndef PROG2_H
#define PROG2_H

#include <arpa/inet.h>
#include <string>
#include <vector>

#define MAX_EVENTS 1024

class Server
{

public:
    Server(const std::string port);
    ~Server();
    void close_connection();
    void init();

private:
    sockaddr_in server_address;
    sockaddr_in client_address;
    socklen_t client_len;
    std::vector<int> clientList;
    int server_sockfd, client_sockfd, 
    epoll_fd, number_of_events,
    message;
};

#endif