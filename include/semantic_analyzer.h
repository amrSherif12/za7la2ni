#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H
#ifdef __cplusplus
extern "C" {
#endif


#include <parser.h>

typedef struct Symbol Symbol;

struct Symbol {
    Type type;
    size_t len;
    size_t offset;
    bool is_const;
};

void analyze(AST *ast, CompilerCtx *ctx);
#ifdef __cplusplus
}
#endif
#endif //SEMANTIC_ANALYZER_H
