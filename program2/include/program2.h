#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
// #include <fcntl.h>

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

#define MAX_EVENTS 1024

class Server
{

public:
    Server(const std::string port);
    ~Server();
    void close_connection();
    void init();

private:
    int server_sockfd, client_sockfd, epoll_fd;
    socklen_t client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int number_of_events;
    int message;
    std::vector<int> clientList;
};