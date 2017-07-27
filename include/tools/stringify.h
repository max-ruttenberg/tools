#ifndef _TOOLS_STRINGIFY_H_
#define _TOOLS_STRINGIFY_H_

#define __STRINGIFY(x)				\
	#x

#define STRINGIFY(x)				\
	__STRINGIFY(x)

#endif
