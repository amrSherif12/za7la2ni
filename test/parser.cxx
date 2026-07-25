#include <gtest/gtest.h>

extern "C" {
#include "parser.h"
}

std::string node_to_string(Node *node) {
    switch (node->type) {
        case NUM_ND: return std::to_string(node->value);
        case ADD_ND: return "+";
        case SUB_ND: return "-";
        case MUL_ND: return "*";
        case DIV_ND: return "/";
        default: return "und";
    }
}

std::string ast_to_string(Node *first_node, int idx) {
    std::string str;
    Node *node = first_node + idx;

    if (node->type == NUM_ND) return std::to_string(node->value);

    return "(" + node_to_string(node) + " " +
           ast_to_string(first_node, node->left) + " " +
           ast_to_string(first_node, node->right) + ")";
}

TEST(Parser, Arithmetic) {
    std::string code = "11+2*33-44/2";
    Token tokens[9] = {
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

    int cursor = 0;
    for (auto & token : tokens) {
        token.loc = (char *) (code.c_str() + cursor);
        cursor += token.len;
    }

    AST ast = parse({.tokens = tokens, .count = 9});

    EXPECT_EQ(ast_to_string(ast.nodes, ast.root_idx), "(- (+ 11 (* 2 33)) (/ 44 2))");
}
