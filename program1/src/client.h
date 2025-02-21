#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <queue>
#include <arpa/inet.h>

class Client
{

public:
    Client(const std::string &ip, const std::string &port);
    ~Client();
    void init();
    void send_message(const int &message);
    void connect_to_server();
    void reconnect();
    void close_connection();

private:
    int sockfd;
    bool flag;
    const sockaddr_in address;
    std::queue<unsigned int> offlineQ;
};

#endif