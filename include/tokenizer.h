#ifndef LEXER_H
#define LEXER_H

typedef struct Token Token;
typedef struct Tokens Tokens;
typedef enum TokenType TokenType;

enum TokenType {
    UND_TK,
    NUM_TK,
    ADD_TK,
    MUL_TK,
    DIV_TK,
    SUB_TK,
    INT_TK,
    LONG_TK,
    BOOL_TK,
    CHAR_TK,
    ASS_OPP_TK,
    SEMI_COL_TK,
    SYM_TK,
    EOF_TK
};

struct Token {
    char *loc;
    TokenType type;
    int len;
};

struct Tokens {
    Token *tokens;
    int count;
};

Tokens tokenize(char *path);

#endif //LEXER_H
