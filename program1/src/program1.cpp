#include "program1.h"

Program1::Program1(std::unique_ptr<Client> ptr) : client(std::move(ptr)) {};

inline bool is_valid_string(const std::string &str)
{
    return !str.empty() &&
           str.size() <= 64 &&
           std::all_of(str.begin(), str.end(), ::isdigit);
}

void Program1::producer()
{
            std::string message;
    while (true)
    {
        std::cout << std::endl;
        std::cout << "Enter string: ";
        std::cin >> message;
        std::cout << std::endl;

        if (is_valid_string(message))
        {
            std::cout << "String is valid." << std::endl;

            lbt::function1(message);

            std::unique_lock<std::mutex> lck(mt);

            v.push(message);

            cv.notify_all();

            cv1.wait(lck, [this]
                     { return v.empty(); });
        }
        else
            std::cout << "String is invalid: size <= 64 or string is empty." << std::endl;
    }
}

void Program1::consumer()
{
    while (true)
    {
        std::string consumed_value;

        std::unique_lock<std::mutex> lck(mt);
        cv.wait(lck, [this]
                { return !v.empty(); });

        consumed_value = v.front();
        v.pop();

        std::cout << "Thread get: " << consumed_value << std::endl;

        client->send_message(lbt::function2(consumed_value));

        cv1.notify_all();
    }
}

int main(int argc, char *argv[])
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

        Program1 progObj(std::move(client));
        std::thread t1(&Program1::producer, &progObj);
        std::thread t2(&Program1::consumer, &progObj);

        t1.join();
        t2.join();
    }

    catch (const std::exception &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}