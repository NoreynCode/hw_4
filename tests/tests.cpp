#define BOOST_TEST_MODULE print_ip_tests
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <sstream>
#include <iostream>

// ЧТо бы проверить вывод на терминал
struct cout_redirect {
    cout_redirect(std::streambuf* new_buffer)
        : old(std::cout.rdbuf(new_buffer)) {
    }

    ~cout_redirect() {
        std::cout.rdbuf(old);
    }

private:
    std::streambuf* old;
};

#include "print_ip.hpp"

using namespace ip_printer;

BOOST_AUTO_TEST_SUITE(print_ip_tests)

// Целочисленные типы
BOOST_AUTO_TEST_SUITE(integer_tests)

BOOST_AUTO_TEST_CASE(test_int8_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(int8_t{ -1 });
    }
    BOOST_CHECK(output.is_equal("255\n"));
}

BOOST_AUTO_TEST_CASE(test_int16_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(int16_t{ 0 });
    }
    BOOST_CHECK(output.is_equal("0.0\n"));
}

BOOST_AUTO_TEST_CASE(test_int32_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(int32_t{ 2130706433 });
    }
    BOOST_CHECK(output.is_equal("127.0.0.1\n"));
}

BOOST_AUTO_TEST_CASE(test_int64_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(int64_t{ 8875824491850138409 });
    }
    BOOST_CHECK(output.is_equal("123.45.67.89.101.112.131.41\n"));
}

BOOST_AUTO_TEST_CASE(test_uint8_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(uint8_t{ 255 });
    }
    BOOST_CHECK(output.is_equal("255\n"));
}

BOOST_AUTO_TEST_CASE(test_uint16_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(uint16_t{ 0x0A0B });
    }
    BOOST_CHECK(output.is_equal("10.11\n"));
}

BOOST_AUTO_TEST_CASE(test_uint32_t) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(uint32_t{ 0xC0A80101 }); // 192.168.1.1
    }
    BOOST_CHECK(output.is_equal("192.168.1.1\n"));
}

BOOST_AUTO_TEST_CASE(test_negative_values) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(int8_t{ -128 });
    }
    BOOST_CHECK(output.is_equal("128\n"));
}

BOOST_AUTO_TEST_SUITE_END()

// Cтроки
BOOST_AUTO_TEST_SUITE(string_tests)

BOOST_AUTO_TEST_CASE(test_regular_string) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::string{ "Hello, World!" });
    }
    BOOST_CHECK(output.is_equal("Hello, World!\n"));
}

BOOST_AUTO_TEST_CASE(test_empty_string) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::string{ "" });
    }
    BOOST_CHECK(output.is_equal("\n"));
}

BOOST_AUTO_TEST_CASE(test_string_with_dots) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::string{ "192.168.1.1" });
    }
    BOOST_CHECK(output.is_equal("192.168.1.1\n"));
}

BOOST_AUTO_TEST_CASE(test_string_with_numbers) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::string{ "123.456.789.0" });
    }
    BOOST_CHECK(output.is_equal("123.456.789.0\n"));
}

BOOST_AUTO_TEST_SUITE_END()

// Контейнеры
BOOST_AUTO_TEST_SUITE(container_tests)

BOOST_AUTO_TEST_CASE(test_vector_int) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::vector<int>{100, 200, 300, 400});
    }
    BOOST_CHECK(output.is_equal("100.200.300.400\n"));
}

BOOST_AUTO_TEST_CASE(test_vector_string) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::vector<std::string>{"192", "168", "1", "1"});
    }
    BOOST_CHECK(output.is_equal("192.168.1.1\n"));
}

BOOST_AUTO_TEST_CASE(test_list_short) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::list<short>{400, 300, 200, 100});
    }
    BOOST_CHECK(output.is_equal("400.300.200.100\n"));
}

BOOST_AUTO_TEST_CASE(test_list_mixed) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::list<int>{1, 2, 3, 4, 5, 6, 7, 8});
    }
    BOOST_CHECK(output.is_equal("1.2.3.4.5.6.7.8\n"));
}

BOOST_AUTO_TEST_CASE(test_empty_vector) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::vector<int>{});
    }
    BOOST_CHECK(output.is_equal("\n"));
}

BOOST_AUTO_TEST_CASE(test_empty_list) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::list<int>{});
    }
    BOOST_CHECK(output.is_equal("\n"));
}

BOOST_AUTO_TEST_CASE(test_vector_one_element) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::vector<int>{42});
    }
    BOOST_CHECK(output.is_equal("42\n"));
}

BOOST_AUTO_TEST_SUITE_END()

// Tupel
BOOST_AUTO_TEST_SUITE(tuple_tests)

BOOST_AUTO_TEST_CASE(test_tuple_int) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::make_tuple(123, 456, 789, 0));
    }
    BOOST_CHECK(output.is_equal("123.456.789.0\n"));
}

BOOST_AUTO_TEST_CASE(test_tuple_string) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        auto t = std::make_tuple(std::string{ "192" },
            std::string{ "168" },
            std::string{ "1" },
            std::string{ "1" });
        print_ip(t);
    }
    BOOST_CHECK(output.is_equal("192.168.1.1\n"));
}

BOOST_AUTO_TEST_CASE(test_tuple_single_element) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::make_tuple(42));
    }
    BOOST_CHECK(output.is_equal("42\n"));
}

BOOST_AUTO_TEST_CASE(test_tuple_eight_elements) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());
        print_ip(std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8));
    }
    BOOST_CHECK(output.is_equal("1.2.3.4.5.6.7.8\n"));
}

BOOST_AUTO_TEST_SUITE_END()

// Тесты что метафункции работают правильно
BOOST_AUTO_TEST_SUITE(sfinae_tests)

BOOST_AUTO_TEST_CASE(test_is_vector) {
    BOOST_CHECK((is_vector_v<std::vector<int>>));
    BOOST_CHECK((is_vector_v<std::vector<std::string>>));
    BOOST_CHECK(!(is_vector_v<std::list<int>>));
    BOOST_CHECK(!(is_vector_v<int>));
    BOOST_CHECK(!(is_vector_v<std::string>));
}

BOOST_AUTO_TEST_CASE(test_is_list) {
    BOOST_CHECK((is_list_v<std::list<int>>));
    BOOST_CHECK((is_list_v<std::list<std::string>>));
    BOOST_CHECK(!(is_list_v<std::vector<int>>));
    BOOST_CHECK(!(is_list_v<int>));
    BOOST_CHECK(!(is_list_v<std::string>));
}

BOOST_AUTO_TEST_CASE(test_is_vector_or_list) {
    BOOST_CHECK((is_vector_or_list_v<std::vector<int>>));
    BOOST_CHECK((is_vector_or_list_v<std::list<int>>));
    BOOST_CHECK(!(is_vector_or_list_v<std::string>));
    BOOST_CHECK(!(is_vector_or_list_v<int>));
    BOOST_CHECK(!(is_vector_or_list_v<std::tuple<int>>));
}

BOOST_AUTO_TEST_CASE(test_all_types_same) {
    constexpr bool same1 = all_types_same_v<std::tuple<int, int, int>>;
    constexpr bool same2 = all_types_same_v<std::tuple<std::string, std::string>>;
    constexpr bool same3 = all_types_same_v<std::tuple<int, double, int>>;
    constexpr bool same4 = all_types_same_v<std::tuple<int, std::string>>;
    constexpr bool same5 = all_types_same_v<std::tuple<int>>;

    BOOST_CHECK(same1);
    BOOST_CHECK(same2);
    BOOST_CHECK(!same3);
    BOOST_CHECK(!same4);
    BOOST_CHECK(same5);
}

BOOST_AUTO_TEST_CASE(test_is_tuple) {
    BOOST_CHECK((is_tuple_v<std::tuple<int>>));
    BOOST_CHECK((is_tuple_v<std::tuple<int, double>>));
    BOOST_CHECK(!(is_tuple_v<int>));
    BOOST_CHECK(!(is_tuple_v<std::vector<int>>));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(integration_tests)

BOOST_AUTO_TEST_CASE(test_multiple_calls) {
    boost::test_tools::output_test_stream output;
    {
        cout_redirect guard(output.rdbuf());

        print_ip(int32_t{ 2130706433 });           // 127.0.0.1
        print_ip(std::string{ "Separator" });       // Separator
        print_ip(std::vector<int>{1, 2, 3, 4});   // 1.2.3.4
        print_ip(std::make_tuple(10, 20, 30));    // 10.20.30
    }

    std::string expected = "127.0.0.1\nSeparator\n1.2.3.4\n10.20.30\n";
    BOOST_CHECK(output.is_equal(expected));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()