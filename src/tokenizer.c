#include <stdio.h>

#include "../include/tokenizer.h"

#include <ctype.h>
#include <stdlib.h>


TokenType read_punc(char *p) {
    if (*p == '+') return ADD_TK;
    if (*p == '-') return SUB_TK;
    if (*p == '*') return MUL_TK;
    if (*p == '/') return DIV_TK;
    return UND_TK;
}


Tokens tokenize(char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) printf("Error: Can't open input file.");

    fseek(file, 0, SEEK_END);
    int size = (int) ftell(file);

    char *buffer = malloc(size + 1);
    char *p = buffer;

    fseek(file, 0, SEEK_SET);
    size_t bytes_read = fread(buffer, 1, size, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    Token *first_token = calloc(size + 1, sizeof(Token));
    Token *tokens = first_token;

    while (*p != '\0') {
        if (isspace(*p)) {
            p++;
            continue;
        }
        if (isdigit(*p)) {
            char *q = p++;
            while (isdigit(*p)) p++;
            *tokens = (Token){.type = NUM_TK, .loc = q, .len = p - q};
            tokens++;
            continue;
        }
        TokenType punc_type = read_punc(p);
        if (punc_type != UND_TK) {
            *tokens = (Token){.type = punc_type, .loc = p, .len = 1};
            tokens++;
            p++;
            continue;
        }
    }
    *tokens = (Token){.type = EOF_TK};
    return (Tokens){first_token, tokens - first_token};
}
