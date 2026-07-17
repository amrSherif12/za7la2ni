#include "../include/codegen.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool reg_in_use[4] = {false, false, false, false};
static const char *registers[4] = { "r8", "r9", "r10", "r11" };

int alloc_reg() {
    for (int i = 0; i < 4; i++) {
        if (!reg_in_use[i]) {
            reg_in_use[i] = true;
            return i;
        }
    }
    exit(1);
}

void free_reg(int i) {
    reg_in_use[i] = false;
}

int gen_code(Node *first_node, int idx, FILE *file) {
    Node *node = first_node + idx;

    if (node->type == NUM_ND) {
        int reg = alloc_reg();
        fprintf(file, "mov %s, %d\n", registers[reg], node->value);
        return reg;
    }

    int left_reg = gen_code(first_node, node->left, file);
    int right_reg = gen_code(first_node, node->right, file);

    switch (node->type) {
        case ADD_ND:
            fprintf(file, "add %s, %s\n", registers[left_reg], registers[right_reg]);
            break;
        case SUB_ND:
            fprintf(file, "sub %s, %s\n", registers[left_reg], registers[right_reg]);
            break;
        case MUL_ND:
            fprintf(file, "imul %s, %s\n", registers[left_reg], registers[right_reg]);
            break;
        case DIV_ND:
            fprintf(file, "mov rax, %s\n", registers[left_reg]);
            fprintf(file, "cqo\n");
            fprintf(file, "idiv %s\n", registers[right_reg]);
            fprintf(file, "mov %s, rax\n", registers[left_reg]);
            break;
    }
    free_reg(right_reg);
    return left_reg;
}

void generate_assembly(AST ast, char *out_path) {
    FILE *out_file = fopen(out_path, "w");
    if (out_file == NULL) printf("Error: Can't open output file.");

    fprintf(out_file, ".intel_syntax noprefix\n");
    fprintf(out_file,".global main\n");
    fprintf(out_file,"main:\n");

    int final_reg = gen_code(ast.nodes, ast.root_idx, out_file);

    fprintf(out_file, "mov rax, %s\n", registers[final_reg]);
    fprintf(out_file,"ret\n");
}
