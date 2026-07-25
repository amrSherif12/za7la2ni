#include <fstream>
#include <gtest/gtest.h>

extern "C" {
    #include "tokenizer.h"
}

Tokens test_tokenizer_wrapper(const char *code) {
    const char *file_name = "test_tokenizer.txt";

    std::ofstream out(file_name);
    out << code;
    out.close();

    Tokens tokens = tokenize(file_name);

    std::remove(file_name);
    return tokens;
}

TEST(Tokenizer, Arithmetic) {
    const char *code = "11+2*33-44/2";
    const Token correct_tokens[9] = {
        {.type = NUM_TK, .len = 2},
        {.type = ADD_TK, .len = 1},
        {.type = NUM_TK, .len = 1},
        {.type = MUL_TK, .len = 1},
        {.type = NUM_TK, .len = 2},
        {.type = SUB_TK, .len = 1},
        {.type = NUM_TK, .len = 2},
        {.type = DIV_TK, .len = 1},
        {.type = NUM_TK, .len = 1},
    };

    Tokens tokens = test_tokenizer_wrapper(code);
    const char *cursor = code;

    EXPECT_EQ(tokens.count, 9);
    for (int i = 0; i < 9; i++) {
        EXPECT_EQ(tokens.tokens[i].type, correct_tokens[i].type);
        EXPECT_EQ(tokens.tokens[i].len, correct_tokens[i].len);

        std::string_view slice(tokens.tokens[i].loc, tokens.tokens[i].len);
        std::string_view correct_slice(cursor, correct_tokens[i].len);
        EXPECT_EQ(slice, correct_slice);

        cursor += correct_tokens[i].len;
        while (isspace(*cursor) || *cursor == '\n') cursor++;
    }
}

TEST(Tokenizer, Variables) {
    const char *code = "number_one x = 1;\nx = 10;";
    const Token correct_tokens[9] = {
        {.type = INT_TK, .len = sizeof(INT) - 1},
        {.type = IDENT_TK, .len = 1},
        {.type = ASS_OPP_TK, .len = 1},
        {.type = NUM_TK, .len = 1},
        {.type = SEMI_COL_TK, .len = 1},
        {.type = IDENT_TK, .len = 1},
        {.type = ASS_OPP_TK, .len = 1},
        {.type = NUM_TK, .len = 2},
        {.type = SEMI_COL_TK, .len = 1},
    };

    Tokens tokens = test_tokenizer_wrapper(code);
    const char *cursor = code;

    EXPECT_EQ(tokens.count, 9);
    for (int i = 0; i < 9; i++) {
        EXPECT_EQ(tokens.tokens[i].type, correct_tokens[i].type);
        EXPECT_EQ(tokens.tokens[i].len, correct_tokens[i].len);

        std::string_view slice(tokens.tokens[i].loc, tokens.tokens[i].len);
        std::string_view correct_slice(cursor, correct_tokens[i].len);
        EXPECT_EQ(slice, correct_slice);

        cursor += correct_tokens[i].len;
        while (isspace(*cursor) || *cursor == '\n') cursor++;
    }
}