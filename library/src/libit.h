#ifndef LIBIT_H
#define LIBIT_H

#include <string>
#include <algorithm>

namespace lbt
{
    void function1(std::string &ptr_str);

    std::string function2(const std::string &ptr_str);

    bool function3(const std::string &ptr_str);
}

#endif