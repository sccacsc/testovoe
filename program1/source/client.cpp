#include <client.h>

Client::Client(const std::string &ip, const std::string port) : sockfd(0),
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

void Client::send_message(std::string message)
{
    if (send(sockfd, &message, sizeof(message), MSG_NOSIGNAL) == -1)
    {
        if (errno == EPIPE || errno == ECONNRESET)
        {
            // if need u can save value in buffer here
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

/*int main(int argc, char *argv[])
{
    try
    {
        std::unique_ptr<Client> client;
        if (argc != 3)
        {
            client = std::make_unique<Client>("127.0.0.1", "8888");
        }
        else
        {
            client = std::make_unique<Client>(argv[1], argv[2]);
        }
        client->init();
        client->connect_to_server();
    }

    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return -1;
    }

    std::cout << 123;

    return 0;
}*/