#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "context.h"

void generate_assembly(AST ast, const char *out_path, CompilerCtx *ctx);

#endif //CODEGEN_H
