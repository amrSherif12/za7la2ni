#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *xmalloc(size_t size);

void *xcalloc(size_t count, size_t size);

void *xrealloc(void *ptr, size_t size);

#define FMT_STR(fmt_message ,message) char fmt_message[1 << 8];\
va_list argptr;\
va_start(argptr,message);\
vsnprintf(fmt_message, sizeof(fmt_message), message, argptr);\
va_end(argptr);

#endif //UTILS_H
