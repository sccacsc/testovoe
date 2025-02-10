#ifndef LIBIT_H
#define LIBIT_H

#include <string>
#include <algorithm>

namespace lbt
{
    inline void function1(std::string& ptr_str);

    inline std::string function2(const std::string& ptr_str);

    inline bool function3(const std::string& ptr_str);
}

#endif