#include <fstream>
#include <gtest/gtest.h>

#include "test_data.hxx"

extern "C" {
#include "tokenizer.h"
}

void test_tokenizer(const char *code, const int tk_cnt, Token *test_tks, bool is_error) {
    CompilerCtx ctx = init_ctx();
    const char *file_name = "test_tokenizer.txt";

    std::ofstream out(file_name);
    out << code;
    out.close();

    if (is_error) {
        EXPECT_EXIT(tokenize(file_name, &ctx), testing::ExitedWithCode(1), ".*");
        return;
    }
    Tokens tokens = tokenize(file_name, &ctx);

    std::remove(file_name);
    const char *cursor = code;

    EXPECT_EQ(tokens.count, tk_cnt);
    for (int i = 0; i < tk_cnt; i++) {
        EXPECT_EQ(tokens.tokens[i].type, test_tks[i].type);
        EXPECT_EQ(tokens.tokens[i].len, test_tks[i].len);

        std::string_view slice(tokens.tokens[i].loc, tokens.tokens[i].len);
        std::string_view correct_slice(cursor, test_tks[i].len);
        EXPECT_EQ(slice, correct_slice);

        cursor += test_tks[i].len;
        while (isspace(*cursor) || *cursor == '\n') cursor++;
    }

    free_ctx(&ctx);
}

TEST(Tokenizer, Arithmetic) {
    test_tokenizer(arith_code.c_str(), arith_tk_cnt, arith_tks, false);
}

TEST(Tokenizer, Variables) {
    test_tokenizer(var_code.c_str(), var_tk_cnt, var_tks, false);
}

TEST(Tokenizer, ErrorIllegalCharacter) {
    test_tokenizer(err_illegal_char_code.c_str(), err_illegal_char_tk_cnt, err_illegal_char_tks, true);
}

TEST(Tokenizer, ErrorIllegalDecimalFormat) {
    test_tokenizer(err_illegal_decimal_code.c_str(), err_illegal_decimal_tk_cnt, err_illegal_decimal_tks, true);
}
