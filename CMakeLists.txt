cmake_minimum_required(VERSION 3.28)
project(za7la2ni C)

set(CMAKE_C_STANDARD 23)
add_executable(${PROJECT_NAME} src/main.c
        src/tokenizer.c
        src/parser.c
        src/codegen.c
        src/map.c
        src/string.c)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")


add_custom_target(runz
        DEPENDS ${PROJECT_NAME}
        COMMAND ./za7la2ni code.txt ass.s
        COMMAND gcc -z noexecstack -o prog ass.s
        COMMAND ./prog || true
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running za7la2ni"
)

