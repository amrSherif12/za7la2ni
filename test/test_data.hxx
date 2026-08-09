#ifndef TEST_DATA_HXX
#define TEST_DATA_HXX

#include <tokenizer.h>
#include <semantic_analyzer.h>
#include <format>
#include <string>
#include <cstring>
#include <map>

// --- Standard Arithmetic ---
static const std::string arith_code = "1.1+2*33+-44/2;";
constexpr int arith_tk_cnt = 11;
static Token arith_tks[arith_tk_cnt] = {
    {.type = TK_FLT, .len = 3},
    {.type = TK_ADD, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_MUL, .len = 1},
    {.type = TK_INT, .len = 2},
    {.type = TK_ADD, .len = 1},
    {.type = TK_INT, .len = 3},
    {.type = TK_DIV, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case 5: Illegal Character (@) ---
static const std::string err_illegal_char_code = "var @x = 1;";
constexpr int err_illegal_char_tk_cnt = 7;
static Token err_illegal_char_tks[err_illegal_char_tk_cnt] = {
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_UND, .len = 1},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Illegal Decimal Format (0..2) ---
static const std::string err_illegal_decimal_code = "0..2;";
constexpr int err_illegal_decimal_tk_cnt = 3;
static Token err_illegal_decimal_tks[err_illegal_decimal_tk_cnt] = {
    {.type = TK_UND, .len = 4},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Variable Declarations & Assignments ---
static const std::string var_code = std::format(
    "var x: {} = true;\n"
    "x = false;\n"
    "const y: {} = 5 + 10;\n"
    "var z: {} = raw;\n",
    BOOL_K, I32_K, CHAR_K);

constexpr int var_tk_cnt = 28;
constexpr int var_offset = 9;
const std::map<std::string, Symbol> var_sem_data = {
    {"x", (Symbol){.type = BOOL, .len = 1, .offset = 1}},
    {"y", (Symbol){.type = I32, .len = 1, .offset = 4}},
    {"z", (Symbol){.type = CHAR, .len = 1, .offset = 8}},
};

static Token var_tks[var_tk_cnt] = {
    // line 1: var x: bool = true;
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_BOOL, .len = strlen(BOOL_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_TRUE, .len = strlen(TRUE_K)},
    {.type = TK_SEMI_COL, .len = 1},

    // line 2: x = false;
    {.type = TK_IDENT, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_FALSE, .len = strlen(FALSE_K)},
    {.type = TK_SEMI_COL, .len = 1},

    // line 3: const y: i32 = 5 + 10;
    {.type = TK_CONST, .len = strlen(CONST_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_ADD, .len = 1},
    {.type = TK_INT, .len = 2},
    {.type = TK_SEMI_COL, .len = 1},

    // line 4: const z: char = raw;
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_CHAR, .len = strlen(CHAR_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_RAW, .len = strlen(RAW_K)},
    {.type = TK_SEMI_COL, .len = 1},

    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Missing Semicolon ---
static const std::string err_no_semi_code = "1 + 2";
constexpr int err_no_semi_tk_cnt = 4;
static Token err_no_semi_tks[err_no_semi_tk_cnt] = {
    {.type = TK_INT, .len = 1},
    {.type = TK_ADD, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Missing Operand ---
static const std::string err_no_op_code = "1 + ;";
constexpr int err_no_op_tk_cnt = 4;
static Token err_no_op_tks[err_no_op_tk_cnt] = {
    {.type = TK_INT, .len = 1},
    {.type = TK_ADD, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Undeclared Variable ---
static const std::string err_undcl_code = "x = 5;";
constexpr int err_undcl_tk_cnt = 5;
static Token err_undcl_tks[err_undcl_tk_cnt] = {
    {.type = TK_IDENT, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Missing Data Type In Variable Declaration ---
static const std::string err_missing_type_var_decl_code = "var x: = 5;";
constexpr int err_missing_type_var_decl_tk_cnt = 7;
static Token err_missing_type_var_decl_tks[err_missing_type_var_decl_tk_cnt] = {
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Variable Redeclaration ---
static const std::string err_redcl_code = std::format("const x: {} = 1;\nvar x: {} = 2;", I32_K, I32_K);
constexpr int err_redcl_tk_cnt = 15;
static Token err_redcl_tks[err_redcl_tk_cnt] = {
    // const x: i32 = 1;
    {.type = TK_CONST, .len = strlen(CONST_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},

    // var x: i32 = 2;
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},

    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Assigning to Constant Variable ---
static const std::string err_assign_const_code = std::format("const x: {} = 1;\nx = 2;", I32_K);
constexpr int err_assign_const_tk_cnt = 12;
static Token err_assign_const_tks[err_assign_const_tk_cnt] = {
    {.type = TK_CONST, .len = strlen(CONST_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Type Mismatch in Assignment ---
static const std::string err_type_mismatch_code = std::format("var x: {} = 1;\nx = true;", I32_K);
constexpr int err_type_mismatch_tk_cnt = 12;
static Token err_type_mismatch_tks[err_type_mismatch_tk_cnt] = {
    {.type = TK_VAR, .len = strlen(VAR_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_INT, .len = 1},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_ASS_OP, .len = 1},
    {.type = TK_TRUE, .len = strlen(TRUE_K)},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Add NonInt Types ---
static const std::string err_add_nonint_types_code = "true + true;";
constexpr int err_add_nonint_types_tk_cnt = 5;
static Token err_add_nonint_types_tks[err_add_nonint_types_tk_cnt] = {
    {.type = TK_TRUE, .len = strlen(TRUE_K)},
    {.type = TK_ADD, .len = 1},
    {.type = TK_TRUE, .len = strlen(TRUE_K)},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};

// --- Error Case: Unassigned Constant ---
static const std::string err_unassigned_const_code = std::format("const x: {};", I32_K);
constexpr int err_unassigned_const_tk_cnt = 6;
static Token err_unassigned_const_tks[err_unassigned_const_tk_cnt] = {
    {.type = TK_CONST, .len = strlen(CONST_K)},
    {.type = TK_IDENT, .len = 1},
    {.type = TK_TYPE_OP, .len = 1},
    {.type = TK_I32, .len = strlen(I32_K)},
    {.type = TK_SEMI_COL, .len = 1},
    {.type = TK_EOF, .len = 0},
};
#endif //TEST_DATA_HXX