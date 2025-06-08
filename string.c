#include "string.h"
#include "kernel.h"

NO_DISCARD size_t strlen(const char *str) {
    size_t count = 0;
    for (const char *c=str; *c; ++c)
        count++;
    return count;
}
