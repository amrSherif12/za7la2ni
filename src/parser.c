#include "../include/parser.h"

#include <ctype.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <utils.h>

#include "../include/tokenizer.h"

typedef struct Parser Parser;

struct Parser {
    Node *nodes;
    Token **token;
    size_t cur_idx;
    char *file_data;
};

int parse_expr(Parser *p);

int parse_mul(Parser *p);

int parse_primary(Parser *p);

NodeType token_to_node(Token *token) {
    switch (token->type) {
        case NUM_TK: return NUM_ND;
        case ADD_TK: return ADD_ND;
        case SUB_TK: return SUB_ND;
        case MUL_TK: return MUL_ND;
        case DIV_TK: return DIV_ND;
        default: return UND_ND;
    }
}

int get_num_value(Token *token) {
    int i = 0;
    int res = 0;
    while (i < token->len && isdigit(token->loc[i])) {
        res = res * 10 + (token->loc[i] - '0');
        i++;
    }
    return res;
}

int add_node(Parser *p, Node new_node) {
    *(p->nodes + p->cur_idx) = new_node;
    return p->cur_idx++;
}

int parse_expr(Parser *p) {
    int cur_idx = parse_mul(p);

    while (true) {
        if ((*p->token)->type == ADD_TK || (*p->token)->type == SUB_TK) {
            Token *op = *p->token;
            (*p->token)++;
            int right_idx = parse_mul(p);
            cur_idx = add_node(p, (Node){.type = token_to_node(op), .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_mul(Parser *p) {
    int cur_idx = parse_primary(p);

    while (true) {
        if ((*p->token)->type == MUL_TK || (*p->token)->type == DIV_TK) {
            Token *op = *p->token;
            (*p->token)++;
            int right_idx = parse_primary(p);
            cur_idx = add_node(p, (Node){.type = token_to_node(op), .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_primary(Parser *p) {
    if ((*p->token)->type != NUM_TK) code_error("Expected a number here.", (*p->token)->loc, p->file_data);

    int cur_idx = add_node(p, (Node){.type = NUM_ND, .value = get_num_value(*p->token), .left = -1, .right = -1});
    (*p->token)++;
    return cur_idx;
}

AST parse(Tokens tokens) {

    Parser p = {
        .nodes = xcalloc(tokens.count + 5, sizeof(Node)),
        .token = &tokens.tokens,
        .cur_idx = 0,
        .file_data = tokens.file_data
    };

    int root = parse_expr(&p);

    // for (int i = 0; i < 9; i++) {
    //     printf("%d %d %d %d %d\n", i, (node + i)->type, (node + i)->value, (node + i)->left, (node + i)->right);
    // }
    // printf("%d", root);

    return (AST){p.nodes, root};
}
