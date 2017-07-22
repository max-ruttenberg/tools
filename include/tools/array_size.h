#ifndef _ARRAY_SIZE_H_
#define _ARRAY_SIZE_H_

#define array_size(x)				\
	(sizeof(x)/(sizeof((x)[0])))

#endif
