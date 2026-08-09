#include "../include/parser.h"

#include <ctype.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <utils.h>
#include <limits.h>
#include <float.h>

#include "../include/tokenizer.h"

#define TOK p->ctx->tokens
#define NODE p->ctx->nodes
#define FILE p->ctx->file
#define SKIP_TOK (TOK)++;
#define I32_MAX INT_MAX
#define I32_MIN INT_MIN
#define I64_MAX LLONG_MAX
#define I64_MIN LLONG_MIN
#define U32_MAX UINT32_MAX
#define U32_MIN 0
#define U64_MAX ULLONG_MAX
#define U64_MIN 0
#define F32_MAX FLT_MAX
#define F32_MIN FLT_MIN
#define F64_MAX DBL_MAX
#define F64_MIN DBL_MIN

typedef struct Parser Parser;

struct Parser {
    CompilerCtx *ctx;
    size_t cur_idx;
};

int parse_stmt(Parser *p);

int parse_var_decl(Parser *p);

int parse_expr_stmt(Parser *p);

int parse_expr(Parser *p);

int parse_assign(Parser *p);

int parse_add(Parser *p);

int parse_mul(Parser *p);

int parse_primary(Parser *p);

int parse_var(Parser *p);

int parse_lit(Parser *p);

NodeType token_to_node(Token *token) {
    switch (token->type) {
        case TK_INT:
        case TK_TRUE:
        case TK_FALSE:
        case TK_FLT: return ND_LIT;
        case TK_ADD: return ND_ADD;
        case TK_SUB: return ND_SUB;
        case TK_MUL: return ND_MUL;
        case TK_DIV: return ND_DIV;
        case TK_ASS_OP: return ND_ASS;
        default: xunreachable;
    }
}

__int128 get_int_value(Token *token) {
    int i = 0;
    int res = 0;
    int neg = 1;
    if (token->loc[i] == '-') {
        neg = -1;
        i++;
    }
    while (i < token->len && isdigit(token->loc[i])) {
        res = res * 10 + (token->loc[i] - '0');
        i++;
    }
    return res * neg;
}

double get_float_value(Token *token) {
    int i = 0;
    double res = 0.0f, neg = 1.0f;
    if (token->loc[i] == '-') {
        neg = -1.0f;
        i++;
    }
    while (i < token->len && isdigit(token->loc[i])) {
        res = res * 10.0f + (double) (token->loc[i] - '0');
        i++;
    }
    if (i < token->len && token->loc[i] == '.') {
        i++;
        double weight = 0.1f;
        while (i < token->len && isdigit(token->loc[i])) {
            res += (double) (token->loc[i] - '0') * weight;
            weight *= 0.1f;
            i++;
        }
    }
    return res * neg;
}

int add_node(Parser *p, Node new_node) {
    *(NODE + p->cur_idx) = new_node;
    return p->cur_idx++;
}

bool equal(Parser *p, TokenType token) {
    if (TOK->type == token) {
        SKIP_TOK;
        return true;
    }
    return false;
}

Type get_type(Token *token) {
    switch (token->type) {
        case TK_I32: return I32;
        case TK_I64: return I64;
        case TK_U32: return U32;
        case TK_U64: return U64;
        case TK_F32: return F32;
        case TK_F64: return F64;
        case TK_BOOL: return BOOL;
        case TK_CHAR: return CHAR;
        default: return UND_TYPE;
    }
}


void print_nodes(Parser *p, int root) {
    for (int i = 0; i < p->cur_idx; i++) {
        printf("%d %d %d %d %d\n", i, (NODE + i)->type, (NODE + i)->data.expr_stmt.next, (NODE + i)->left,
               (NODE + i)->right);
    }
    printf("%d\n", root);
}

int parse_stmt(Parser *p) {
    Node *prev_node = nullptr;
    int root = -1;

    while (TOK->type != TK_EOF) {
        int cur_idx;
        if (TOK->type == TK_VAR || TOK->type == TK_CONST) {
            cur_idx = parse_var_decl(p);
        } else {
            cur_idx = parse_expr_stmt(p);
        }
        if (prev_node) prev_node->data.expr_stmt.next = cur_idx;
        else root = cur_idx;

        prev_node = NODE + cur_idx;
    }
    prev_node->data.expr_stmt.next = -1;
    return root;
}

int parse_var_decl(Parser *p) {
    Node node = (Node){};

    node.type = ND_VAR_DECL;
    node.loc = TOK->loc;
    node.data.decl.is_const = TOK->type == TK_CONST;
    SKIP_TOK;

    node.left = parse_expr(p);

    if (!equal(p, TK_TYPE_OP)) {
        code_error(TOK->loc, FILE, "expected ':' here");
    }

    node.data.decl.type = get_type(TOK);
    if (node.data.decl.type == UND_TYPE) {
        code_error((TOK - 1)->loc, FILE, "missing data type in variable declaration");
    }
    SKIP_TOK;

    if (equal(p, TK_SEMI_COL)) {
        node.right = -1;
        return add_node(p, node);
    }

    if (!equal(p, TK_ASS_OP)) {
        code_error(TOK->loc, FILE, "expected '=' or ';' here");
    }

    if (equal(p, TK_RAW)) {
        if (equal(p, TK_SEMI_COL)) {
            node.data.decl.is_raw = true;
            return add_node(p, node);
        }
        code_error(TOK->loc, FILE, "expected ';' here");
    }

    node.data.decl.is_raw = false;
    node.right = parse_expr(p);

    if (!equal(p, TK_SEMI_COL)) {
        code_error(TOK->loc, FILE, "expected ';' here");
    }

    return add_node(p, node);
}


int parse_expr_stmt(Parser *p) {
    char *loc = TOK->loc;
    int cur_idx = parse_expr(p);

    if (!equal(p, TK_SEMI_COL)) {
        code_error(TOK->loc, FILE, "expected ';' here");
    }

    return add_node(p, (Node){
                        .type = ND_EXPR_STMT, .loc = loc, .left = cur_idx, .right = -1
                    });
}

int parse_expr(Parser *p) {
    return parse_assign(p);
}

int parse_assign(Parser *p) {
    int cur_idx = parse_add(p);

    while (true) {
        if (TOK->type == TK_ASS_OP) {
            Token *op = TOK;
            SKIP_TOK;
            int right_idx = parse_add(p);
            cur_idx = add_node(p, (Node){.type = token_to_node(op), .loc = TOK->loc, .left = cur_idx, .right = right_idx});
        }
        return cur_idx;
    }
}

int parse_add(Parser *p) {
    int cur_idx = parse_mul(p);

    while (true) {
        if (TOK->type == TK_ADD || TOK->type == TK_SUB) {
            Token *op = TOK;
            SKIP_TOK;
            int right_idx = parse_mul(p);
            cur_idx = add_node(p, (Node){.type = token_to_node(op), .loc = TOK->loc, .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_mul(Parser *p) {
    int cur_idx = parse_primary(p);

    while (true) {
        if (TOK->type == TK_MUL || TOK->type == TK_DIV) {
            Token *op = TOK;
            SKIP_TOK;
            int right_idx = parse_primary(p);
            cur_idx = add_node(p, (Node){.type = token_to_node(op), .loc = TOK->loc, .left = cur_idx, .right = right_idx});
            continue;
        }
        return cur_idx;
    }
}

int parse_primary(Parser *p) {
    switch (TOK->type) {
        case TK_IDENT: return parse_var(p);
        case TK_INT:
        case TK_FLT:
        case TK_CHAR:
        case TK_TRUE:
        case TK_FALSE: return parse_lit(p);
        default: code_error(TOK->loc, FILE, "expected an expression here.");
    }
}

int parse_var(Parser *p) {
    Node node = (Node){
        .type = ND_VAR, .loc = TOK->loc, .data = {.var = (NodeVar){.len = TOK->len}},
        .left = -1, .right = -1
    };
    int cur_idx = add_node(p, node);
    SKIP_TOK;
    return cur_idx;
}

int parse_lit(Parser *p) {
    Node node;

    if (TOK->type == TK_INT) {
        __int128 num = get_int_value(TOK);
        Type type = (Type){};
        if (U32_MIN <= num && num <= U32_MAX) type = U32;
        if (U64_MIN <= num && num <= U64_MAX) type = U64;
        if (I32_MIN <= num && num <= I32_MAX) type = I32;
        if (I64_MIN <= num && num <= I64_MAX) type = I64;
        else code_error(TOK->loc, FILE, "invalid integer literal");
        node = (Node){
            .data = (NodeLit){.type = type, .val = {.i = (int64_t) num}},
        };
    } else if (TOK->type == TK_FLT) {
        double num = get_float_value(TOK);
        Type type = (Type){};
        if (F32_MIN <= num && num <= F32_MAX) type = F32;
        else type = F64;
        node = (Node){
            .data = (NodeLit){.type = type, .val = {.f = num}},
        };
    } else if (TOK->type == TK_CHAR) {
        if (CHAR_MIN > *(TOK->loc + 1) || *(TOK->loc + 1) < CHAR_MAX)
            code_error(
                TOK->loc, FILE, "invalid integer literal");
        node = (Node){
            .data = (NodeLit){.type = CHAR, .val = {.c = *(TOK->loc + 1)}},
        };
    } else if (TOK->type == TK_TRUE || TOK->type == TK_FALSE) {
        node = (Node){
            .data = (NodeLit){.type = BOOL, .val = {.b = TOK->type == TK_TRUE ? true : false}},
        };
    } else {
        xunreachable;
    }
    node.left = -1;
    node.right = -1;
    node.loc = TOK->loc;
    int cur_idx = add_node(p, node);
    SKIP_TOK;
    return cur_idx;
}

AST parse(Tokens tokens, CompilerCtx *ctx) {
    ctx->nodes = xcalloc(tokens.count + 5, sizeof(Node));
    Parser p = {
        .ctx = ctx,
        .cur_idx = 0,
    };

    int root = parse_stmt(&p);
    //print_nodes(&p, root);
    return (AST){p.ctx->nodes, root};
}
