#include <gtest/gtest.h>
#include <fstream>

extern "C" {
    #include "codegen.h"
}

int run_code(char *code) {
    const char *file_input = "test_codegen.txt";
    const char *file_output = "test_ass.s";
    std::ofstream out(file_input);
    out << code;
    out.close();

    Tokens tokens = tokenize(file_input);
    AST ast = parse(tokens);
    generate_assembly(ast, file_output);

    std::system("gcc -z noexecstack -o test_prog test_ass.s");
    int exit_code = std::system("./test_prog");

    std::remove(file_input);
    std::remove(file_output);
    std::remove("test_prog");

    return (exit_code >> 8) & 0xFF;
}

TEST(Codegen, Arithmetic) {
    std::string code = "11+2*33-44/2";
    EXPECT_EQ(run_code(code.data()), 55);
}