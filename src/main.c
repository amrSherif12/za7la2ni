#include <context.h>
#include <error.h>
#include <semantic_analyzer.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "codegen.h"


int main(int argc, char **argv) {
    char *input_file = NULL;
    char *output_file = NULL;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            output_file = argv[i + 1];
            i++;
        } else {
            input_file = argv[i];
        }
    }

    if (!input_file) {
        sys_error("no input file");
    }

    CompilerCtx ctx = init_ctx();

    Tokens tokens = tokenize(input_file, &ctx);

    AST ast = parse(tokens, &ctx);

    analyze(&ast, &ctx);

    generate_assembly(ast, output_file, &ctx);
}


//TODO: add parsing var
//TODO: add semantic analyzer
//TODO: add ir
