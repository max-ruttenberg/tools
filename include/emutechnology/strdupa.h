#ifndef _STRDUPA_H_
#define _STRDUPA_H_
#include <alloca.h>
#include <string.h>

#ifndef strdupa
#define strdupa(str)				\
	strcpy((char*)alloca(strlen(str)+1),str)
#endif
	

#endif