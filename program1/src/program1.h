#ifndef PROG1_H
#define PROG1_H

#include "client.h"
#include "libit.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <string>
#include <algorithm>

class Program1 {
public:
    explicit Program1(std::unique_ptr<Client> ptr);
    void producer();
    void consumer();
private:
    std::mutex mt;
    std::condition_variable cv;
    std::queue<std::string> v;
    std::unique_ptr<Client> client;
};

#endif