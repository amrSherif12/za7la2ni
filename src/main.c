#include <stdio.h>
#include <stdlib.h>

#include "../include/tokenizer.h"
#include "../include/parser.h"
#include "../include/codegen.h"
#include "../include/map.h"



int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Error: Enter input and output file names.");
    }

    Tokens tokens = tokenize(argv[1]);

    AST ast = parse(tokens);

    generate_assembly(ast, argv[2]);
}
