#include "../include/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/tokenizer.h"

int parse_expr(Token **token);

int parse_mul(Token **token);

int parse_primary(Token **token);

NodeType token_to_node(Token *token) {
    switch (token->type) {
        case UND_TK: return UND_ND;
        case NUM_TK: return NUM_ND;
        case ADD_TK: return ADD_ND;
        case SUB_TK: return SUB_ND;
        case MUL_TK: return MUL_ND;
        case DIV_TK: return DIV_ND;
        case EOF_TK: return UND_ND;
    }
    return UND_ND;
}

int get_num_value(Token *token) {
    char temp = token->loc[token->len];
    token->loc[token->len] = '\0';
    int res = atoi(token->loc);
    token->loc[token->len] = temp;
    return res;
}

static Node *node;

int add_node(Node new_node) {
    static int cur_idx = 0;
    *(node + cur_idx) = new_node;
    return cur_idx++;
}

int parse_expr(Token **token) {
    int cur_idx = parse_mul(token);

    while (true) {
        if ((*token)->type == ADD_TK || (*token)->type == SUB_TK) {
            Token *op = *token;
            (*token)++;
            int right_idx = parse_mul(token);
            cur_idx = add_node((Node){.type = token_to_node(op), .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_mul(Token **token) {
    int cur_idx = parse_primary(token);

    while (true) {
        if ((*token)->type == MUL_TK || (*token)->type == DIV_TK) {
            Token *op = *token;
            (*token)++;
            int right_idx = parse_primary(token);
            cur_idx = add_node((Node){.type = token_to_node(op), .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_primary(Token **token) {
    if ((*token)->type != NUM_TK) exit(1);

    int cur_idx = add_node((Node){.type = NUM_ND, .value = get_num_value(*token), .left = -1, .right = -1});
    (*token)++;
    return cur_idx;
}

AST parse(Tokens tokens) {
    Token *token = tokens.tokens;

    if (token->type != NUM_TK) exit(1);
    node = calloc(tokens.count + 5, sizeof(Node));
    Node *first_node = node;

    int root = parse_expr(&token);

    return (AST){first_node, root};
}
