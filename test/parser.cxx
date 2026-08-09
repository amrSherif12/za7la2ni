#include <gtest/gtest.h>
#include <format>

#include "test_data.hxx"

extern "C" {
#include "parser.h"
#include "error.h"
}

std::string type_to_keyword(Type type) {
    if (type == I32) return I32_K;
    if (type == I64) return I64_K;
    if (type == F32) return F32_K;
    if (type == BOOL) return BOOL_K;
    if (type == CHAR) return CHAR_K;
    xunreachable;
}

std::string node_to_string(Node *node) {
    switch (node->type) {
        case ND_LIT: {
            if (node->data.lit.type == I32 || node->data.lit.type == I64) return std::to_string(node->data.lit.val.i);
            if (node->data.lit.type == U32 || node->data.lit.type == U64) return std::to_string(node->data.lit.val.u);
            if (node->data.lit.type == F32 || node->data.lit.type == F64)
                return std::format(
                    "{:.1f}", node->data.lit.val.f);
            if (node->data.lit.type == BOOL) return std::to_string(node->data.lit.val.b);
            if (node->data.lit.type == CHAR) return std::to_string(node->data.lit.val.c);
        }
        case ND_ADD: return "+";
        case ND_SUB: return "-";
        case ND_MUL: return "*";
        case ND_DIV: return "/";
        case ND_VAR: return std::string(node->loc, node->data.var.len);
        case ND_VAR_DECL: return "decl " + type_to_keyword(node->data.decl.type);
        case ND_ASS: return "=";
        default: xunreachable;
    }
}

std::string stmt_to_string(Node *first_node, int idx) {
    if (idx == -1) return "";
    Node *node = first_node + idx;

    if (node->type == ND_LIT || node->type == ND_VAR) return node_to_string(node);

    if (node->type == ND_EXPR_STMT) return stmt_to_string(first_node, node->left);

    if (node->type == ND_VAR_DECL) {
        if (node->data.decl.is_raw)
            return "(" + node_to_string(node) + " " +
                   stmt_to_string(first_node, node->left) + " raw)";
        if (node->right == -1)
            return "(" + node_to_string(node) + " " +
                   stmt_to_string(first_node, node->left) + ")";
    }

    return "(" + node_to_string(node) + " " +
           stmt_to_string(first_node, node->left) + " " +
           stmt_to_string(first_node, node->right) + ")";
}

std::string ast_to_string(Node *first_node, int idx) {
    std::string str;
    int cur_idx = idx;
    Node *node = first_node + cur_idx;
    while (node->type == ND_VAR_DECL || node->type == ND_EXPR_STMT) {
        str += stmt_to_string(first_node, cur_idx) + '\n';
        cur_idx = node->data.expr_stmt.next;
        if (cur_idx == -1) break;
        node = first_node + cur_idx;
    }
    return str;
}

void test_parser(const char *code, const int tk_cnt, Token *test_tks, const char *ast_string, bool exp_error) {
    size_t cursor = 0;
    for (int i = 0; i < tk_cnt; i++) {
        Token *token = test_tks + i;
        while (isspace(code[cursor]) || code[cursor] == '\n') cursor++;
        token->loc = (char *) (code + cursor);
        cursor += token->len;
    }
    CompilerCtx ctx = init_ctx();
    ctx.file = code;
    ctx.tokens = test_tks;

    if (exp_error)
        EXPECT_EXIT(parse({.tokens = test_tks, .count = (size_t) tk_cnt}, &ctx), testing::ExitedWithCode(1), ".*");
    else {
        AST ast = parse({.tokens = test_tks, .count = (size_t) tk_cnt}, &ctx);
        EXPECT_EQ(ast_to_string(ast.nodes, ast.root_idx), ast_string);
    }

    free(ctx.nodes);
}

TEST(Parser, Arithmetic) {
    test_parser(arith_code.c_str(), arith_tk_cnt, arith_tks, "(+ (+ 1.1 (* 2 33)) (/ -44 2))\n", false);
}

TEST(Parser, Variables) {
    std::string fmt = std::format(
        "(decl {} x 1)\n"
        "(= x 0)\n"
        "(decl {} y (+ 5 10))\n"
        "(decl {} z raw)\n",
        BOOL_K, I32_K, CHAR_K
    );
    test_parser(var_code.c_str(), var_tk_cnt, var_tks, fmt.c_str(), false);
}

TEST(Parser, ErrorMissingSemicolon) {
    test_parser(err_no_semi_code.c_str(),
                err_no_semi_tk_cnt, err_no_semi_tks,
                "", true);
}

TEST(Parser, ErrorMissingOperand) {
    test_parser(err_no_op_code.c_str(),
                err_no_op_tk_cnt, err_no_op_tks,
                "", true);
}

TEST(Parser, ErrorMissingTypeInVarDecl) {
    test_parser(err_missing_type_var_decl_code.c_str(),
                err_missing_type_var_decl_tk_cnt,
                err_missing_type_var_decl_tks,
                "", true);
}
