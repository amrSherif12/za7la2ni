#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct CompilerCtx CompilerCtx;

struct Token;

struct CompilerCtx {
  struct Token *tokens;
  struct Node *nodes;
  const char *file;
};

void free_ctx(CompilerCtx *ctx);

void free_ctx_nof(CompilerCtx *ctx);

CompilerCtx init_ctx();

#endif //CONTEXT_H
