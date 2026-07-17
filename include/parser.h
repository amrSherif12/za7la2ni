#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

typedef struct Node Node;
typedef struct AST AST;
typedef enum NodeType NodeType;

enum NodeType {
    UND_ND,
    NUM_ND,
    ADD_ND,
    SUB_ND,
    MUL_ND,
    DIV_ND,
};

struct Node {
    NodeType type;
    int value;
    int left;
    int right;
};

struct AST {
    Node *nodes;
    int root_idx;
};

AST parse(Tokens tokens);

#endif //PARSER_H
