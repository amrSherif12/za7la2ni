#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void sys_error(const char *message, ...) {
     printf("\033[1;31msystem johnson: \033[0m %s\n", message);
     exit(1);
}

void code_error(const char *message, char *loc, char *file_data) {
     int line = 1;
     int col = 1;

     char *col_loc = loc;
     while (col_loc > file_data) {
          col_loc--;
          if (*col_loc == '\n') break;
          col++;
     }

     char *line_loc = file_data;
     while (line_loc++ < loc) {
          if (*line_loc == '\n') line_loc++;
     }

     printf("\033[1;31m%d:%d johnson:\033[0m %s\n", line, col, message);
     printf("%s\n", col_loc);
     printf("%*s\033[1;31m^\033[0m\n", col - 1, "");

     exit(1);
}
