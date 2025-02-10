#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

class Client
{

public:
    Client(const std::string& ip, const std::string& port);
    ~Client();
    void init();
    void send_message(std::string message);
    void connect_to_server();
    void reconnect();
    void close_connection();

private:
    int sockfd;
    struct sockaddr_in address;
    int message;
    bool connection_flag;
};

#endif