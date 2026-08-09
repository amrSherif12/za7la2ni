#ifndef ERROR_H
#define ERROR_H

void sys_error(const char *message, ...);

void internal_error(const char *message, ...);

void code_error(char *loc, const char *file_data, const char *message, ...);

#define xunreachable internal_error("internal error at %s:%d", __FILE__, __LINE__); exit(1);

#endif //ERROR_H
