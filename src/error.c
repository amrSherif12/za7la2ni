#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <utils.h>

void sys_error(const char *message, ...) {
     FMT_STR(fmt_message, message);
     printf("\033[1;31msystem johnson: \033[0m%s\n", fmt_message);
     exit(1);
}

void internal_error(const char *message, ...) {
     FMT_STR(fmt_message, message);
     printf("\033[1;31minternal johnson: \033[0m%s\n", fmt_message);
     exit(1);
}

void code_error(char *loc, const char *file_data, const char *message, ...) {
     FMT_STR(fmt_message, message);

     int line = 1;
     int col = 1;

     char *col_loc = loc;
     while (col_loc > file_data) {
          col_loc--;
          if (*col_loc == '\n') {
               col_loc++;
               break;
          }
          col++;
     }
     const char *line_loc = file_data;
     while (line_loc < loc) {
          line_loc++;
          if (*line_loc == '\n') line++;
     }

     int len = strcspn(col_loc, "\n");

     printf("\033[1;31m%d:%d johnson:\033[0m %s\n", line, col, fmt_message);
     printf("%.*s\n", len, col_loc);
     printf("%*s\033[1;31m^\033[0m\n", col - 1, "");

     exit(1);
}
