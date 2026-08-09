#include <gtest/gtest.h>
#include "test_data.hxx"


extern "C" {
#include "semantic_analyzer.h"
#include "parser.h"
}

void test_analyzer(const char *code, const int tk_cnt, Token *test_tks, void (*callback)(AST *)) {
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

    AST ast = parse({.tokens = test_tks, .count = (size_t) tk_cnt}, &ctx);

    if (!callback)
        EXPECT_EXIT(analyze(&ast, &ctx), testing::ExitedWithCode(1), ".*");
    else {
        analyze(&ast, &ctx);
        callback(&ast);
    }

    free(ctx.nodes);
}

void check_var_sem(Node *first_node, size_t idx) {
    Node *node = first_node + idx;
    if (idx == -1) return;
    if (node->type == ND_VAR) {
        NodeVar *var = &node->data.var;
        Symbol sym = var_sem_data.find(std::string(node->loc, node->loc + var->len))->second;

        EXPECT_EQ(var->offset, sym.offset);
        EXPECT_EQ(var->type, sym.type);
    }
    check_var_sem(first_node, node->left);
    check_var_sem(first_node, node->right);
}

void var_callback(AST *ast) {
    EXPECT_EQ(ast->global_offset, var_offset);
    check_var_sem(ast->nodes, ast->root_idx);
}

TEST(SemanticAnalyzer, Variables) {
    test_analyzer(var_code.c_str(), var_tk_cnt, var_tks, var_callback);
}

TEST(SemanticAnalyzer, ErrorUndeclaredVariable) {
    test_analyzer(err_undcl_code.c_str(), err_undcl_tk_cnt, err_undcl_tks, nullptr);
}

TEST(SemanticAnalyzer, ErrorRedeclaration) {
    test_analyzer(err_redcl_code.c_str(), err_redcl_tk_cnt, err_redcl_tks, nullptr);
}

TEST(SemanticAnalyzer, ErrorTypeMismatch) {
    test_analyzer(err_type_mismatch_code.c_str(), err_type_mismatch_tk_cnt, err_type_mismatch_tks, nullptr);
}

TEST(SemanticAnalyzer, ErrorAssignConst) {
    test_analyzer(err_assign_const_code.c_str(), err_assign_const_tk_cnt, err_assign_const_tks, nullptr);
}

TEST(SemanticAnalyzer, ErrorAddNonIntTypes) {
    test_analyzer(err_add_nonint_types_code.c_str(), err_add_nonint_types_tk_cnt, err_add_nonint_types_tks, nullptr);
}

TEST(SemanticAnalyzer, ErrorUnassignedConst) {
    test_analyzer(err_unassigned_const_code.c_str(), err_unassigned_const_tk_cnt, err_unassigned_const_tks, nullptr);
}
