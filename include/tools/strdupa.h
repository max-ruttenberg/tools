#ifndef _STRDUPA_H_
#define _STRDUPA_H_
#include <alloca.h>
#include <string.h>

#ifndef strdupa
#define strdupa(str) ({						\
			char *tmp = alloca(strlen(str)+1);	\
			strcpy(tmp, str);})
#endif
	

#endif
