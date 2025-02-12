#ifndef LIBIT_H
#define LIBIT_H

#include <string>

namespace lbt
{
    void function1(std::string &ptr_str);

    int function2(const std::string &ptr_str);

    bool function3(const int &sum);
}

#endif