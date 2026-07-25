#ifndef ERROR_H
#define ERROR_H

void sys_error(const char *message, ...);

void code_error(const char *message, char *loc, char *file_data);

#endif //ERROR_H
