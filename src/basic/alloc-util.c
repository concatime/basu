/* SPDX-License-Identifier: LGPL-2.1+ */

#include <stdint.h>
#include <string.h>

#include "alloc-util.h"
#include "util.h"

static void *mempcpy(void *dest, const void *src, size_t n)
{
	return (char *)memcpy(dest, src, n) + n;
}

void* memdup(const void *p, size_t l) {
        void *ret;

        assert(l == 0 || p);

        ret = malloc(l);
        if (!ret)
                return NULL;

        memcpy(ret, p, l);
        return ret;
}

void* memdup_suffix0(const void *p, size_t l) {
        void *ret;

        assert(l == 0 || p);

        /* The same as memdup() but place a safety NUL byte after the allocated memory */

        ret = malloc(l + 1);
        if (!ret)
                return NULL;

        *((uint8_t*) mempcpy(ret, p, l)) = 0;
        return ret;
}

void* greedy_realloc(void **p, size_t *allocated, size_t need, size_t size) {
        size_t a, newalloc;
        void *q;

        assert(p);
        assert(allocated);

        if (*allocated >= need)
                return *p;

        newalloc = MAX(need * 2, 64u / size);
        a = newalloc * size;

        /* check for overflows */
        if (a < size * need)
                return NULL;

        q = realloc(*p, a);
        if (!q)
                return NULL;

        *p = q;
        *allocated = newalloc;
        return q;
}
