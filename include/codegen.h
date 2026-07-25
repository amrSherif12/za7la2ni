#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

void generate_assembly(AST ast, const char *out_path);

#endif //CODEGEN_H
