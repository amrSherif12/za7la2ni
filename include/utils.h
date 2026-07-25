#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *xmalloc(size_t size);

void *xcalloc(size_t count, size_t size);

void *xrealloc(void *ptr, size_t count);

#endif //UTILS_H
