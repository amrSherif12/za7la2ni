#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include<utils.h>
#include <stdarg.h>
#include <string.h>


void *xmalloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) sys_error("Out of memory. Cannot allocate more space.");
    return ptr;
}

void *xcalloc(size_t count, size_t size) {
    void *ptr = calloc(count, size);
    if (ptr == NULL) sys_error("Out of memory. Cannot allocate more space.");
    return ptr;
}

void * xrealloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL) sys_error("Out of memory. Cannot allocate more space.");
    return new_ptr;
}
