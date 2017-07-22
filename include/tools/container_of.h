/* Copyright (c) 2017 Max Ruttenberg */

/* Permission is hereby granted, free of charge, to any person obtaining a copy */
/* of this software and associated documentation files (the "Software"), to deal */
/* in the Software without restriction, including without limitation the rights */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
/* copies of the Software, and to permit persons to whom the Software is */
/* furnished to do so, subject to the following conditions: */

/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
/* SOFTWARE. */
/*************************************/
/* This software is taken from Linux */
/*************************************/
#ifndef _TOOLS_CONTAINER_OF_H_
#define _TOOLS_CONTAINER_OF_H_
#include <stdlib.h>

/**
   @param ptr pointer to the member of the target struct
   @param type the type of the target structure
   @param member the name of the member \p ptr in the target struct
   
   Returns a pointer to a containing structure from \p ptr. 
   Example:
   
   struct A {
   ...
   };
   
   struct B {
       struct A a;
   };

   struct B b;
   struct A *ap = &b->a;
   container_of(ap, struct B, a) == &b; // this evaluates to true
 */
#define container_of(ptr,type,member) ({				\
			const typeof( ((type*)0)->member )		\
				*__mptr = (ptr);			\
			(type*)( (char*)__mptr - offsetof(type,member) );})

#endif
