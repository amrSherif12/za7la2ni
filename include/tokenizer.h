#ifndef LEXER_H
#define LEXER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "context.h"

#define I32_K "i32"
#define I64_K "i64"
#define U32_K "u32"
#define U64_K "u64"
#define F32_K "f32"
#define F64_K "f64"
#define BOOL_K "bool"
#define CHAR_K "char"
#define TRUE_K "true"
#define FALSE_K "false"
#define VAR_K "var"
#define CONST_K "const"
#define RAW_K "raw"

typedef struct Token Token;
typedef struct Tokens Tokens;

typedef enum TokenType {
    TK_INT,
    TK_FLT,
    TK_ADD,
    TK_MUL,
    TK_DIV,
    TK_SUB,
    TK_I32,
    TK_I64,
    TK_U32,
    TK_U64,
    TK_F32,
    TK_F64,
    TK_BOOL,
    TK_CHAR,
    TK_TRUE,
    TK_FALSE,
    TK_NEG,
    TK_ASS_OP,
    TK_VAR,
    TK_CONST,
    TK_RAW,
    TK_TYPE_OP,
    TK_SEMI_COL,
    TK_IDENT,
    TK_UND,
    TK_EOF
} TokenType;

    struct Token {
    char *loc;
    TokenType type;
    size_t len;
};

struct Tokens {
    Token *tokens;
    size_t count;
};

void init_keywords(bool is_meme);

Tokens tokenize(const char *path, CompilerCtx *ctx);

#ifdef __cplusplus
}
#endif
#endif //LEXER_H
