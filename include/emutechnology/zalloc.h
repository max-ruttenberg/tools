#ifndef _ZALLOC_H_
#define _ZALLOC_H_
#include <stdlib.h>
#include <stddef.h>

#define zalloca(len)				\
	calloc(len, 1)

static inline void *zalloc(size_t size)
{
	return calloc(size,1);
}
#endif
