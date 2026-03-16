/**
 * @file main.cpp
 * @brief Демонстрация работы библиотеки print_ip
 *
 * Содержит примеры использования всех перегрузок функции print_ip.
 */

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>

#include "print_ip.hpp"

int main() {
    
    ip_printer::print_ip(int8_t{ -1 }); // 255
    ip_printer::print_ip(int16_t{ 0 }); // 0.0
    ip_printer::print_ip(int32_t{ 2130706433 }); // 127.0.0.1
    ip_printer::print_ip(int64_t{ 8875824491850138409 }); // 123.45.67.89.101.112.131.41
    ip_printer::print_ip(std::string{ "Hello, World!" }); // Hello, World!
    ip_printer::print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    ip_printer::print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
    ip_printer::print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0
    //ip_printer::print_ip(std::make_tuple(123, "456", 789.0)); Ошибка - разные типы в tuple

    return 0;
}