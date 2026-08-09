#include "../include/context.h"

#include <stdlib.h>

void free_ctx(CompilerCtx *ctx) {
    if (ctx->tokens) free(ctx->tokens);
    if (ctx->nodes) free(ctx->nodes);
    if (ctx->file) free(ctx->file);
}

void free_ctx_nof(CompilerCtx *ctx) {
    if (ctx->tokens) free(ctx->tokens);
    if (ctx->nodes) free(ctx->nodes);
}



CompilerCtx init_ctx() {
    return (CompilerCtx) {
        .tokens = nullptr,
        .nodes = nullptr,
        .file = nullptr,
    };
}
