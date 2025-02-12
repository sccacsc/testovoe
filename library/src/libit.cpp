#include "libit.h"

#include <algorithm>

// Сортирует элементы входной строки по убыванию и все четные элементы заменяет на
// латинские буквы «КВ». Функция не возвращает никакого значения.
void lbt::function1(std::string &ptr_str)
{
    std::sort(ptr_str.begin(), ptr_str.end(),
              [](int a, int b){ return a > b; });
    for (auto it = ptr_str.begin(); it != ptr_str.end();)
    {
        if (*it % 2 == 0)
        {
            it = ptr_str.erase(it);
            it = ptr_str.insert(it, 'K');
            ++it;
            it = ptr_str.insert(it, 'B');
            ++it;
        }
        else
            ++it;
    }
}
// Рассчитывает и возвращает общую сумму всех элементов входной строки, которые
// являются численными значениями
int lbt::function2(const std::string &ptr_str)
{
    int sum = 0;

    for (const auto ch : ptr_str)
    {
        if (std::isdigit(ch))
            sum += ch - '0';
    }

    return sum;
}
// Анализирует, из скольки символов состоит входная строка. Если оно больше 2-ух
// символов и, если оно кратно 32, то функция возвращает «истина». В противном случае функция
// возвращает «ложь»
bool lbt::function3(const int &sum)
{
    return ((sum > 9) && (sum % 32 == 0)) ? true : false;
}