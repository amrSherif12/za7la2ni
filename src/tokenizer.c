#include <stdio.h>

#include "../include/tokenizer.h"

#include <ctype.h>
#include <string.h>
#include <utils.h>


TokenType read_punc(const char *p) {
    switch (*p) {
        case '+': return ADD_TK;
        case '-': return SUB_TK;
        case '*': return MUL_TK;
        case '/': return DIV_TK;
        case '=': return ASS_OPP_TK;
        case ';': return SEMI_COL_TK;
        default: return UND_TK;
    }
}

TokenType is_keyword(const char *start, const char *end) {
    if (strncmp(INT, start, end - start) == 0) return INT_TK;
    if (strncmp(LONG, start, end - start) == 0) return LONG_TK;
    if (strncmp(FLOAT, start, end - start) == 0) return FLOAT_TK;
    if (strncmp(BOOL, start, end - start) == 0) return BOOL_TK;
    if (strncmp(CHAR, start, end - start) == 0) return CHAR_TK;
    return UND_TK;
}

void add_token(Token **tokens, Token new_tk) {
    **tokens = new_tk;
    (*tokens)++;
}

Tokens tokenize(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) printf("Error: Can't open input file.");

    fseek(file, 0, SEEK_END);
    int size = (int) ftell(file);

    char *buffer = xmalloc(size + 1);
    char *p = buffer;

    fseek(file, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    Token *first_token = xcalloc(size + 1, sizeof(Token));
    Token *tokens = first_token;

    while (*p != '\0') {
        if (isspace(*p) || *p == '\n') {
            p++;
            continue;
        }

        if (isdigit(*p)) {
            char *q = p++;
            while (isdigit(*p)) p++;
            add_token(&tokens, (Token){.type = NUM_TK, .loc = q, .len = p - q});
            continue;
        }

        TokenType punc_type = read_punc(p);
        if (punc_type != UND_TK) {
            add_token(&tokens, (Token){.type = punc_type, .loc = p, .len = 1});
            p++;
            continue;
        }

        if (isalnum(*p) || *p == '_') {
            char *q = p++;
            while (isalnum(*p) || *p == '_') p++;

            TokenType type = is_keyword(q, p);
            if (type == UND_TK) type = IDENT_TK;

            add_token(&tokens, (Token){.type = type, .loc = q, .len = p - q});
        }

    }
    *tokens = (Token){.type = EOF_TK};
    return (Tokens){first_token, tokens - first_token, buffer};
}
