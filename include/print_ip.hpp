/**
 * @file print_ip.hpp
 * @brief Функция print_ip для печати "условного IP-адреса"
 *
 * Поддерживаются:
 * - Целочисленные типы (int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t)
 * - std::string
 * - std::vector и std::list (любые типы элементов)
 * - std::tuple (только с элементами одного типа)
 *
 * @author Student
 * @version 1.0
 */

#ifndef PRINT_IP
#define PRINT_IP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <cstdint>

namespace ip_printer {

    // Вспомогательные структуры для проверки типов
    template<typename...>
    struct is_tuple : std::false_type {};

    template<typename... Args>
    struct is_tuple<std::tuple<Args...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_tuple_v = is_tuple<T>::value;

    // Проверка, что все типы в tupele одинаковые
    template<typename Tuple, typename = void>
    struct all_types_same : std::false_type {};

    template<typename... Args>
    struct all_types_same<std::tuple<Args...>,
        std::enable_if_t<sizeof...(Args) >= 1>> {
        static constexpr bool value = (std::is_same_v<Args, std::tuple_element_t<0, std::tuple<Args...>>> && ...);
    };

    template<typename T>
    inline constexpr bool all_types_same_v = all_types_same<T>::value;

    // Проверка что std::vector
    template<typename T>
    struct is_vector : std::false_type {};

    template<typename... Args>
    struct is_vector<std::vector<Args...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_vector_v = is_vector<T>::value;

    // Проверка что std::list
    template<typename T>
    struct is_list : std::false_type {};

    template<typename... Args>
    struct is_list<std::list<Args...>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_list_v = is_list<T>::value;

    // Проверка что std::vector или std::list
    template<typename T>
    inline constexpr bool is_vector_or_list_v = is_vector_v<T> || is_list_v<T>;

    // Вспомогательные печати
    namespace detail {
        template<typename Tuple, std::size_t... Is>
        void print_tuple_impl(const Tuple& t, std::index_sequence<Is...>) {
            ((std::cout << (Is == 0 ? "" : ".") << std::get<Is>(t)), ...);
            std::cout << std::endl;
        }

        template<typename T>
        void print_integer(T value) {
            constexpr size_t bytes = sizeof(T);
            const uint8_t* data = reinterpret_cast<const uint8_t*>(&value);

            for (size_t i = bytes; i > 0; --i) {
                std::cout << static_cast<int>(data[i - 1]);
                if (i > 1) {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
    }

    /**
     * @brief Печатает целое число как IP-адрес.
     *
     * Число интерпретируется как последовательность байт в порядке
     * от старшего к младшему. Поддерживаются целые числа любого размера.
     *
     * @param value Целое число для печати
     *
     * @code
     * print_ip(int8_t{-1});           // 255
     * print_ip(int16_t{0});            // 0.0
     * print_ip(int32_t{2130706433});   // 127.0.0.1
     * print_ip(int64_t{8875824491850138409}); // 123.45.67.89.101.112.131.41
     * @endcode
     */
    template<typename T>
    typename std::enable_if_t<std::is_integral_v<T>>
        print_ip(T value) {
        detail::print_integer(value);
    }

    /**
     * @brief Печатает строку без изменений.
     *
     * @code
     * print_ip(std::string{"Hello, World!"}); // Hello, World!
     * print_ip(std::string{"192.168.1.1"});   // 192.168.1.1
     * @endcode
     */
    template<typename T>
    typename std::enable_if_t<std::is_same_v<T, std::string>>
        print_ip(const T& value) {
        std::cout << value << std::endl;
    }

    /**
     * @brief Печатает элементы контейнера через точку.
     *
     * Поддерживаются std::vector и std::list с любым типом элементов,
     * которые можно вывести в std::cout.
     *
     * @code
     * print_ip(std::vector<int>{100, 200, 300, 400});     // 100.200.300.400
     * print_ip(std::list<short>{400, 300, 200, 100});     // 400.300.200.100
     * print_ip(std::vector<std::string>{"192","168","1","1"}); // 192.168.1.1
     * @endcode
     */
    template<typename T>
    typename std::enable_if_t<is_vector_or_list_v<T>>
        print_ip(const T& container) {
        bool first = true;
        for (const auto& elem : container) {
            if (!first) {
                std::cout << ".";
            }
            std::cout << elem;
            first = false;
        }
        std::cout << std::endl;
    }

    /**
     * @brief Печатает элементы кортежа через точку.
     *
     * @note Все элементы кортежа должны быть одного типа.
     *
     * @code
     * print_ip(std::make_tuple(123, 456, 789, 0));    // 123.456.789.0
     * print_ip(std::make_tuple(1, 2, 3, 4, 5, 6));    // 1.2.3.4.5.6
     * print_ip(std::make_tuple(std::string{"192"},    // 192.168.1.1
     *                         std::string{"168"},
     *                         std::string{"1"},
     *                         std::string{"1"}));
     * @endcode
     */
    template<typename T>
    typename std::enable_if_t<is_tuple_v<T>&& all_types_same_v<T>>
        print_ip(const T& tuple) {
        constexpr size_t tuple_size = std::tuple_size_v<T>;
        detail::print_tuple_impl(tuple, std::make_index_sequence<tuple_size>{});
    }
}

#endif // PRINT_IP