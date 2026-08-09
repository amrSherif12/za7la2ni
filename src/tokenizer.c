#include <stdio.h>

#include "../include/tokenizer.h"

#include <ctype.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <vector.h>


TokenType read_punc(const char *p) {
    switch (*p) {
        case '+': return TK_ADD;
        case '-': return TK_SUB;
        case '*': return TK_MUL;
        case '/': return TK_DIV;
        case '=': return TK_ASS_OP;
        case ';': return TK_SEMI_COL;
        case ':': return TK_TYPE_OP;
        default: return TK_UND;
    }
}

TokenType is_keyword(const char *start, const char *end) {
    if (strncmp(I32_K, start, end - start) == 0) return TK_I32;
    if (strncmp(I64_K, start, end - start) == 0) return TK_I64;
    if (strncmp(U32_K, start, end - start) == 0) return TK_U32;
    if (strncmp(U64_K, start, end - start) == 0) return TK_U64;
    if (strncmp(F32_K, start, end - start) == 0) return TK_F32;
    if (strncmp(F64_K, start, end - start) == 0) return TK_F64;
    if (strncmp(BOOL_K, start, end - start) == 0) return TK_BOOL;
    if (strncmp(CHAR_K, start, end - start) == 0) return TK_CHAR;
    if (strncmp(TRUE_K, start, end - start) == 0) return TK_TRUE;
    if (strncmp(FALSE_K, start, end - start) == 0) return TK_FALSE;
    if (strncmp(VAR_K, start, end - start) == 0) return TK_VAR;
    if (strncmp(CONST_K, start, end - start) == 0) return TK_CONST;
    if (strncmp(RAW_K, start, end - start) == 0) return TK_RAW;
    return TK_UND;
}

void add_token(Vector *tokens, Token new_tk) {
    vec_push_back(tokens, &new_tk);
}

Token *prev_token(Vector *tokens) {
    return vec_find(tokens, tokens->size - 1);
}


Tokens tokenize(const char *path, CompilerCtx *ctx) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) sys_error("can't open file at %s.", path);

    fseek(file, 0, SEEK_END);
    int size = (int) ftell(file);

    char *buffer = xmalloc(size + 1);
    ctx->file = buffer;
    char *p = buffer;

    fseek(file, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    Vector *tokens = create_vector(size / 4, sizeof(Token));

    while (*p != '\0') {
        if (isspace(*p) || *p == '\n') {
            p++;
            continue;
        }

        // PUNCTUATION

        TokenType punc_type = read_punc(p);
        if (punc_type == TK_UND) goto end_punc;

        if (punc_type == TK_SUB) {
            Token *prev = prev_token(tokens);
            if (prev->type != TK_IDENT &&
                prev->type != TK_INT &&
                prev->type != TK_FLT &&
                prev->type != TK_CHAR &&
                prev->type != TK_BOOL &&
                prev->type != TK_TRUE &&
                prev->type != TK_FALSE
            ) {
                if (isdigit(*(p + 1))) goto end_punc;
                punc_type = TK_NEG;
            }
        }
        add_token(tokens, (Token){.type = punc_type, .loc = p, .len = 1});
        p++;
        continue;
        end_punc:

        // NUMBERS

        if (!isdigit(*p) && !(*p == '-' && isdigit(*(p + 1)))) goto end_num;

        bool is_dec = false;
        char *q = p++;
        while (isdigit(*p) || *p == '.') {
            if (*p == '.' && is_dec) code_error(q, buffer, "invalid decimal literal");
            else if (*p == '.') is_dec = true;
            p++;
        }
        add_token(tokens, (Token){.type = is_dec ? TK_FLT : TK_INT, .loc = q, .len = p - q});
        continue;
        end_num:

        // IDENTIFIERS

        if (!isalpha(*p) && *p != '_') goto end_ident;
        q = p++;
        while (isalnum(*p) || *p == '_') p++;

        TokenType type = is_keyword(q, p);
        if (type == TK_UND) type = TK_IDENT;

        add_token(tokens, (Token){.type = type, .loc = q, .len = p - q});
        continue;
        end_ident:

        code_error(p, buffer, "invalid token");
    }

    add_token(tokens, (Token){.type = TK_EOF, .len = 0, .loc = p});

    Token *first_token = (Token *) tokens->data;
    size_t tk_size = tokens->size;
    ctx->tokens = first_token;

    // for (int i = 0; i < tokens->size - 1; i++) {
    //     printf("%d ", (first_token + i)->type);
    // }
    free(tokens);

    return (Tokens){first_token, tk_size};
}
