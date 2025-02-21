#include "client.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <cstring>
#include <queue>

Client::Client(const std::string &ip, const std::string &port) : address{AF_INET,
                                                                         htons(std::stoi(port)),
                                                                         inet_addr(ip.c_str()),
                                                                         {0}} {};
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
        {std::cout << "Connected." << std::endl;
            flag = true;
        }
}

void Client::send_message(const int &message)
{
    offlineQ.push(message);
    // MSG_NOSIGNAL, иначе при потере соединения программа завершиться
    // проблема в том, что при отключении сервера один send() уходит никуда. И только при втором
    // становится понятно, что соединение потеряно
    /*if (send(sockfd, &(offlineQ.front()), sizeof(offlineQ.front()), MSG_NOSIGNAL) == -1)
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
    }*/
    // решение, сделать эхо-сервер

    flag = true; // чтобы не войти в цикл при reconnect()
    int temp = 0;     // для recv

    while (!offlineQ.empty() && flag)
    {
        ssize_t bytes_send = send(sockfd, &(offlineQ.front()), sizeof(offlineQ.front()), MSG_NOSIGNAL);
        ssize_t bytes_recv = recv(sockfd, &(temp), sizeof(temp), 0);

        if (bytes_recv != -1 && bytes_recv == bytes_send)
        {
            offlineQ.pop();
            std::cout << "Message sent." << std::endl;
        }
        else
        {
            flag = false;
            reconnect();
        }
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
}
