#ifndef LEXER_H
#define LEXER_H

#define INT "number_one"
#define LONG "number_kbir"
#define FLOAT "number_nos"
#define BOOL "tartara"
#define CHAR "char"

typedef struct Token Token;
typedef struct Tokens Tokens;

typedef enum TokenType {
    NUM_TK,
    ADD_TK,
    MUL_TK,
    DIV_TK,
    SUB_TK,
    INT_TK,
    LONG_TK,
    FLOAT_TK,
    BOOL_TK,
    CHAR_TK,
    ASS_OPP_TK,
    SEMI_COL_TK,
    IDENT_TK,
    UND_TK,
    EOF_TK
} TokenType;

struct Token {
    char *loc;
    TokenType type;
    size_t len;
};

struct Tokens {
    Token *tokens;
    size_t count;
    char *file_data;
};

Tokens tokenize(const char *path);

#endif //LEXER_H
