cmake_minimum_required(VERSION 3.12)

# Версия будет передана через -DPATCH_VERSION из CI
set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(sfinae VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Основной исполняемый файл
add_executable(sfinae src/main.cpp)

# Библиотека с реализацией print_ip (header-only)
add_library(print_ip INTERFACE)

target_include_directories(print_ip INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Настройки для основного исполняемого файла
set_target_properties(sfinae PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Подключаем директории для include
target_include_directories(sfinae
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Линковка с библиотекой print_ip
target_link_libraries(sfinae PRIVATE
    print_ip
)

# Тестирование
if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(tests tests/tests.cpp)
    
    set_target_properties(tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(tests
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    set_target_properties(tests PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
    
    target_link_libraries(tests
        ${Boost_LIBRARIES}
        print_ip
    )
endif()

# Компиляционные флаги
if (MSVC)
    target_compile_options(sfinae PRIVATE /W4)
    if(WITH_BOOST_TEST)
        target_compile_options(tests PRIVATE /W4)
    endif()
else ()
    target_compile_options(sfinae PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_BOOST_TEST)
        target_compile_options(tests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

# Установка
install(TARGETS sfinae RUNTIME DESTINATION bin)

# Пакетирование - используем динамическую версию
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

# Тестирование через CTest
if(WITH_BOOST_TEST)
    enable_testing()
    add_test(tests tests)
endif()