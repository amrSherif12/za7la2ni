#include "../include/semantic_analyzer.h"

#include <error.h>

#include "../include/vector.h"
#include "map.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SCOPE(par) (Scope) {.parent = par, .tot_offset = 0 ,.symbols = create_map(8, sizeof(Symbol))}

typedef struct Scope Scope;

size_t type_size(const Type type) {
    switch (type) {
        case I32: return 4;
        case I64: return 8;
        case F32: return 4;
        case F64: return 8;
        case BOOL: return 1;
        case CHAR: return 1;
        case U32: return 4;
        case U64: return 8;
        default: xunreachable;
    }
}

bool is_numeric_type(const Type type) {
    return type == I32 ||
           type == I64 ||
           type == U32 ||
           type == U64 ||
           type == F32 ||
           type == F64;
}

const char *type_to_str(const Type type) {
    switch (type) {
        case I32: return I32_K;
        case I64: return I64_K;
        case F32: return F32_K;
        case F64: return F64_K;
        case BOOL: return BOOL_K;
        case CHAR: return CHAR_K;
        case U32: return U32_K;
        case U64: return U64_K;
        default: xunreachable;
    }
}

struct Scope {
    Map *symbols;
    Scope *parent;
    size_t tot_offset;
};

bool can_convert_to(const Type t1, const Type t2) {
    if (t1 == UND_TYPE || t2 == UND_TYPE) return false;
    if (t1 == t2) return true;

    if ((t1 == I32 && t2 == I64) ||
        (t1 == U32 && t2 == U64) ||
        (t1 == F32 && t2 == F64)) {
        return true;
    }

    return false;
}

Type analyze_node(size_t idx, Vector *scopes, CompilerCtx *ctx);

void analyze_var_decl(size_t idx, Vector *scopes, CompilerCtx *ctx);

void analyze_expr_stmt(size_t idx, Vector *scopes, CompilerCtx *ctx);

void analyze_assign(size_t idx, Vector *scopes, CompilerCtx *ctx);

Symbol *analyze_locator(size_t idx, Vector *scopes, CompilerCtx *ctx);

Type analyze_binary_arithm_op(size_t idx, Vector *scopes, CompilerCtx *ctx);

Symbol *analyze_var(size_t idx, Vector *scopes, CompilerCtx *ctx);

Type analyze_lit(size_t idx, Vector *scopes, CompilerCtx *ctx);

void analyze_var_decl(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;
    Scope *scope = vec_back(scopes);
    Node *left_node = ctx->nodes + node->left;

    NodeVar *var = &left_node->data.var;
    size_t size = type_size(node->data.decl.type);
    size_t unaligned_offset = size + scope->tot_offset;
    var->offset = unaligned_offset + (size - 1) & ~(size - 1);

    Symbol sym = (Symbol){
        .type = node->data.decl.type,
        .offset = var->offset, .is_const = node->data.decl.is_const
    };
    scope->tot_offset = var->offset;

    if (map_find(scope->symbols, left_node->loc, var->len) != NULL) {
        code_error(left_node->loc, ctx->file, "variable '%.*s' already declared", var->len, left_node->loc);
    }

    if (sym.is_const && node->right == -1) {
        code_error(left_node->loc, ctx->file, "const variables must be assigned a value", var->len, left_node->loc);

    }

    map_hash(scope->symbols, left_node->loc, var->len, &sym);
    analyze_var(node->left, scopes, ctx);
    analyze_node(node->right, scopes, ctx);
    analyze_node(node->data.decl.next, scopes, ctx);
}

void analyze_expr_stmt(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;
    analyze_node(node->left, scopes, ctx);
    analyze_node(node->data.expr_stmt.next, scopes, ctx);
}

Symbol *analyze_var(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;
    Scope *scope = vec_back(scopes);

    Symbol *sym = map_find(scope->symbols, node->loc, node->data.var.len);
    if (sym == NULL) {
        code_error(node->loc, ctx->file, "undefined symbol '%.*s'", node->data.var.len, node->loc);
    }

    node->data.var.offset = sym->offset;
    node->data.var.type = sym->type;
    return sym;
}

Type analyze_lit(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;
    return node->data.lit.type;
}

void analyze_assign(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;
    Node *left_node = ctx->nodes + node->left;
    NodeVar *var = &left_node->data.var;

    Symbol *sym = analyze_locator(node->left, scopes, ctx);
    if (sym->is_const) {
        code_error(left_node->loc, ctx->file, "can not assign to const variable '%.*s'", var->len, left_node->loc);
    }

    Node *right_node = ctx->nodes + node->right;
    Type right_type = analyze_node(node->right, scopes, ctx);

    if (!can_convert_to(var->type, right_type)) {
        code_error(right_node->loc, ctx->file, "can't assign %.*s to %.*s",
                   strlen(type_to_str(right_type)), type_to_str(right_type),
                   strlen(type_to_str(var->type)), type_to_str(var->type));
    }
}

Symbol *analyze_locator(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;

    if (node->type == ND_VAR) return analyze_var(idx, scopes, ctx);
    code_error(node->loc, ctx->file, "expected a locator here");
}

Type analyze_binary_arithm_op(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    Node *node = ctx->nodes + idx;

    Type left_type = analyze_node(node->left, scopes, ctx);
    Type right_type = analyze_node(node->right, scopes, ctx);

    if (!is_numeric_type(left_type) || !is_numeric_type(right_type))
        code_error(node->loc, ctx->file, "cannot do arithmetic operations on non numerical values");

    if (left_type == right_type) return left_type;

    if ((left_type == I32 && right_type == I64) || (left_type == I64 && right_type == I32)) return I64;
    if ((left_type == U32 && right_type == U64) || (left_type == U64 && right_type == U32)) return U64;

    if ((left_type == F32 && right_type == F64) || (left_type == F64 && right_type == F32)) return F64;

    if ((left_type == I32 || left_type == I64 || left_type == U32 || left_type == U64) &&
        (right_type == F32 || right_type == F64)) return F64;

    if ((right_type == I32 || right_type == I64 || right_type == U32 || right_type == U64) &&
        (left_type == F32 || left_type == F64)) return F64;

    return UND_TYPE;
}

Type analyze_node(size_t idx, Vector *scopes, CompilerCtx *ctx) {
    if (idx == -1) return UND_TYPE;
    Node *node = ctx->nodes + idx;

    switch (node->type) {
        case ND_VAR: return analyze_var(idx, scopes, ctx)->type;
        case ND_VAR_DECL: analyze_var_decl(idx, scopes, ctx);
            return UND_TYPE;
        case ND_ASS: analyze_assign(idx, scopes, ctx);
            return UND_TYPE;
        case ND_EXPR_STMT: analyze_expr_stmt(idx, scopes, ctx);
            return UND_TYPE;
        case ND_LIT: return analyze_lit(idx, scopes, ctx);
        case ND_ADD:
        case ND_DIV:
        case ND_MUL:
        case ND_SUB: return analyze_binary_arithm_op(idx, scopes, ctx);
        default: return UND_TYPE;
    }
}

void analyze(AST *ast, CompilerCtx *ctx) {
    Vector *scopes = create_vector(8, sizeof(Scope));
    Scope global = INIT_SCOPE(NULL);
    vec_push_back(scopes, &global);

    analyze_node(ast->root_idx, scopes, ctx);
    global = *(Scope *) vec_back(scopes);
    ast->global_offset = global.tot_offset;

    free_map(global.symbols);
    free_vector(scopes);
}
