#include "client.h"

Client::Client(const std::string &ip, const std::string &port) : sockfd(0),
                                                                 address{AF_INET,
                                                                         htons(std::stoi(port)),
                                                                         inet_addr(ip.c_str()),
                                                                         {0}},
                                                                 message(0) {};
void Client::init()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
    }
}
Client::~Client()
{
    close_connection();
}

void Client::connect_to_server()
{
    std::cout << "Trying to connect. . ." << std::endl;
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
    {
        std::cout << "Fail. Continue working offline. . ." << std::endl;
    }
    else
        std::cout << "Connected." << std::endl;
}

void Client::send_message(int message)
{
    offlineQ.push(message);

    /*send(sockfd, &(offlineQ.front()), sizeof(offlineQ.front()), MSG_NOSIGNAL);
    if (recv(sockfd, &(message), sizeof(message), MSG_NOSIGNAL) == -1)
    {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN)
        {
            reconnect();
        }
        else
        {
            throw std::runtime_error("Send failed: " + std::string(strerror(errno)));
        }
    }
    else
    {
        std::cout << "Message sent" << std::endl;
        offlineQ.pop();
    }*/
    // MSG_NOSIGNAL, иначе при потере соединения программа завершиться
    // 
    if (send(sockfd, &(offlineQ.front()), sizeof(offlineQ.front()), MSG_NOSIGNAL) == -1)
    {
        if (errno == EPIPE || errno == ECONNRESET)
        {
            reconnect();
        }
        else
        {
            throw std::runtime_error("Send failed: " + std::string(strerror(errno)));
        }
    }
    else
    {
        std::cout << "Message sent" << std::endl;
        offlineQ.pop();
    }
}

void Client::reconnect()
{
    std::cout << "Connection lost." << std::endl;
    close_connection();
    init();
    connect_to_server();
}

void Client::close_connection()
{
    if (close(sockfd) == -1)
    {
        throw std::runtime_error("Socket closing failed: " + std::string(strerror(errno)));
    };
    sockfd = -1;
}
