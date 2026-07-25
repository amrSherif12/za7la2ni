#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "tokenizer.h"

typedef struct Node Node;
typedef struct AST AST;

typedef enum NodeType {
    NUM_ND,
    ADD_ND,
    SUB_ND,
    MUL_ND,
    DIV_ND,
    UND_ND,
} NodeType;

struct Node {
    NodeType type;
    int value;
    int left;
    int right;
};

struct AST {
    Node *nodes;
    size_t root_idx;
};

AST parse(Tokens tokens);

#endif //PARSER_H
