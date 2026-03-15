cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(sfinae VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

# Основной исполняемый файл
add_executable(sfinae src/main.cpp)

# Библиотека print_ip
add_library(print_ip INTERFACE)

target_include_directories(print_ip INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(sfinae PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(sfinae
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(sfinae PRIVATE
    print_ip
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(sfinae_tests tests/tests.cpp)
    
    set_target_properties(sfinae_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_include_directories(sfinae_tests
        PRIVATE 
            ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    
    set_target_properties(sfinae_tests PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    )
    
    target_link_libraries(sfinae_tests
        ${Boost_LIBRARIES}
        print_ip
    )
endif()

if (MSVC)
    target_compile_options(sfinae PRIVATE /W4)
    if(WITH_BOOST_TEST)
        target_compile_options(sfinae_tests PRIVATE /W4)
    endif()
else ()
    target_compile_options(sfinae PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_BOOST_TEST)
        target_compile_options(sfinae_tests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

install(TARGETS sfinae RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(sfinae_tests sfinae_tests)
endif()