#ifndef PROG1_H
#define PROG1_H

#include "client.h"

//#include <mutex>
#include <condition_variable>
#include <queue>

class Program1
{
public:
    Program1(std::unique_ptr<Client> ptr);
    ~Program1();
    void producer();
    void consumer();

private:
    std::mutex mt;
    std::condition_variable cv;
    //std::condition_variable cv1;
    std::queue<std::string> buffer;
    std::unique_ptr<Client> client;
    bool ready;
};

#endif