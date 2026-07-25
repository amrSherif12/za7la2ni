cmake_minimum_required(VERSION 3.28)
project(za7la2ni)

set(CMAKE_C_STANDARD 23)

add_library(comp_src STATIC
        src/tokenizer.c
        src/parser.c
        src/codegen.c
        src/map.c
        src/z_string.c
        src/error.c
        src/utils.c)

target_include_directories(comp_src PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE comp_src)

add_custom_target(runz
        DEPENDS ${PROJECT_NAME}
        COMMAND ./za7la2ni code.txt ass.s
        COMMAND gcc -z noexecstack -o prog ass.s
        COMMAND ./prog || true
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running za7la2ni"
)

enable_testing()
add_subdirectory(external)

add_executable(testz test/map.cxx
        test/z_string.cxx
        test/tokenizer.cxx
        test/parser.cxx
        test/codegen.cxx)
target_link_libraries(testz PRIVATE comp_src GTest::gtest GTest::gtest_main)

add_test(NAME all_tests COMMAND testz)
