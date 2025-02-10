#include "program2.h"

Server::Server(const std::string port) : server_sockfd(-1),
                                         client_sockfd(-1),
                                         epoll_fd(-1),
                                         client_len(sizeof(client_address)),
                                         server_address{AF_INET, htons(std::stoi(port)), {INADDR_ANY}, {0}},
                                         client_address(),
                                         number_of_events(0),
                                         message("empty") {

                                         };
Server::~Server()
{
    close_connection();
}

void Server::init()
{

    server_sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); // fcntl NON_BLOCK
    if (server_sockfd == -1)
    {
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
    }

    int opt = 1;
    setsockopt(server_sockfd, SOL_SOCKET,
               SO_REUSEADDR | SO_REUSEPORT, &opt,
               sizeof(opt));
    if (setsockopt(server_sockfd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)) == -1)
    {
        throw std::runtime_error("Set socket option failed: " + std::string(strerror(errno)));
    }

    if (bind(server_sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
    }

    if (listen(server_sockfd, 128) == -1)
    {
        throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
    }
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        throw std::runtime_error("Epoll create failed: " + std::string(strerror(errno)));
    }

    struct epoll_event event{};
    event.data.fd = server_sockfd;
    event.events = EPOLLIN;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sockfd, &event))
    {
        throw std::runtime_error("Epoll adding failed: " + std::string(strerror(errno)));
    }

    std::cout << "Server is waiting. . ." << std::endl;

    struct epoll_event events[MAX_EVENTS];

    while (true)
    {
        number_of_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (number_of_events == -1)
        {
            throw std::runtime_error("Epoll wait failed: " + std::string(strerror(errno)));
        }
        for (int i = 0; i < number_of_events; ++i)
        {
            if (events[i].data.fd == server_sockfd)
            {
                client_sockfd = accept4(server_sockfd,
                                        (struct sockaddr *)&client_address,
                                        &client_len,
                                        SOCK_NONBLOCK);
                if (client_sockfd == -1)
                {
                    std::cout << "Accept failed, continue waiting. . ." << std::endl;
                    continue;
                }
                // inet_ntop
                std::cout << "New coonection with: " << client_sockfd << std::endl;

                event.data.fd = client_sockfd;
                event.events = EPOLLIN | EPOLLET;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sockfd, &event))
                {
                    throw std::runtime_error("Epoll adding failed: " + std::string(strerror(errno)));
                }
            }
            else if (events[i].events & EPOLLIN)
            {
                ssize_t bytes_read = recv(events[i].data.fd, &message, sizeof(message), 0);
                if (bytes_read <= 0)
                {
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                    std::cout << "Client disconnected: " << client_sockfd << std::endl;
                }
                else
                {
                    std::cout << "Message: " << message << std::endl;
                    if(lbt::function3(message)) std::cout << "Client send: " << message << std::endl;
                    else std::cout << "Data from clinet if invalid" << std::endl;;
                }
            }
        }
    }
}

void Server::close_connection()
{

    close(server_sockfd);
    close(client_sockfd);
    std::cout << "Server closed connection" << std::endl;
}

int main(int argc, char *argv[])
{   
    try
    {
        std::unique_ptr<Server> server;
        if (argc != 2)
        {
            server = std::make_unique<Server>("8888");
        }
        else
        {
            server = std::make_unique<Server>(argv[1]);
        }
        server->init();
    }

    catch (const std::exception &e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}