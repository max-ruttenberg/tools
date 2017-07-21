#ifndef _PRINTING_H_
#define _PRINTING_H_
#include <stdio.h>
#ifdef DEBUG
#define pr_dbg(fmt,...)				\
	printf("%-10s" fmt, "debug:", ##__VA_ARGS__)
#else
#define pr_dbg(...)
#endif

#define pr_err(fmt, ...)			\
	fprintf(stderr, "%-10s" fmt, "error:", ##__VA_ARGS__)

#define pr_warning(fmt, ...)			\
	fprintf(stderr, "%-10s" fmt, "warning:", ##__VA_ARGS__)
#endif
