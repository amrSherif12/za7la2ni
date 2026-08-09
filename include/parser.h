#ifndef PARSER_H
#define PARSER_H
#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>
#include <stdint.h>

#include "context.h"
#include "tokenizer.h"

typedef struct Node Node;
typedef struct NodeVar NodeVar;
typedef struct NodeLit NodeLit;
typedef union LitVal LitVal;
typedef struct NodeDecl NodeDecl;
typedef struct NodeExprStmt NodeExprStmt;
typedef struct AST AST;

typedef enum NodeType {
    ND_LIT,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_ASS,
    ND_EXPR_STMT,
    ND_VAR,
    ND_VAR_DECL,
} NodeType;

typedef enum Type {
    I32,
    I64,
    U32,
    U64,
    F32,
    F64,
    BOOL,
    CHAR,
    UND_TYPE,
} Type;

// TODO next always first
// TODO loc always first

union LitVal {
    int64_t i;
    double f;
    uint64_t u;
    bool b;
    char c;
};

struct NodeLit {
    Type type;
    LitVal val;
};

struct NodeDecl {
    int next;
    Type type;
    bool is_const;
    bool is_raw;
};

struct NodeExprStmt {
    int next;
};

struct NodeVar {
    uint32_t len;
    uint32_t offset;
    Type type;
};

struct Node {
    NodeType type;
    int left;
    int right;
    char *loc;

    union {
        NodeLit lit;
        NodeDecl decl;
        NodeExprStmt expr_stmt;
        NodeVar var;
    } data;
};

struct AST {
    Node *nodes;
    size_t root_idx;
    size_t global_offset;
};

AST parse(Tokens tokens, CompilerCtx *ctx);

#ifdef __cplusplus
}
#endif
#endif //PARSER_H
