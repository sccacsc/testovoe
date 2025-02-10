#include "client.h"

Client::Client(const std::string &ip, const std::string& port) : sockfd(0),
                                                                address{AF_INET,
                                                                        htons(std::stoi(port)),
                                                                        inet_addr(ip.c_str()),
                                                                        {0}},
                                                                message(0),
                                                                connection_flag(true) {};
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
    while (connection_flag)
    {
        std::cout << "Trying to connect. . ." << std::endl;
        if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) != -1)
            connection_flag = false;
    }
    std::cout << "Connected." << std::endl;
}

void Client::send_message(int message)
{
    if (send(sockfd, &message, sizeof(message), MSG_NOSIGNAL) == -1)
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
    std::cout << "Message sent" << std::endl;   
}

void Client::reconnect()
{
    std::cout << "Connection lost." << std::endl;
    connection_flag = true;
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
