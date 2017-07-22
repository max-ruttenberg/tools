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
#ifndef _TABLE_H_
#define _TABLE_H_
#include <stddef.h>
#include <stdint.h>
#include "list.h"

typedef intptr_t tdata_t;
typedef unsigned (*table_hash_func)(const char *key);
struct table {
	size_t e_max;
	size_t e_size;
	size_t n_entries;
	table_hash_func hash;
	struct list_head **buckets;		
};

/**
   @param table a table to initialize
   @param options an option string, expects respective arguments
   
   Initializes a table with an option string, optional arguments are expected depending
   on the space delimitted values in \p options. 
   Parameters specified in \p options are:
   
   max_size: expects a size_t argument marking the maximum number of entries in \p table
   size: expects a size_t argument marking the initial capacity of \p table
   with_hash: expects an argument of type unsigned (*)(const char *) which shall produce a reproducable value.
   
   Rerturns zero on success, and a negative number on failure.
 */
int  table_init(struct table *table, const char *options, ...);

/**
   @param table a table to destroy
   
   Performs required cleanup on \p table. Calls to table_init() should be followed with a call to this function.
 */
void table_dest(struct table *table);

/**
   @param options an option string, expects respective arguments
   
   Allocates and initializes a table with an option string, optional arguments are expected depending
   on the space delimitted values in \p options. 
   Parameters specified in \p options are:
   
   max_size: expects a size_t argument marking the maximum number of entries in \p table
   size: expects a size_t argument marking the initial capacity of \p table
   with_hash: expects an argument of type unsigned (*)(const char *) which shall produce a reproducable value
 */
struct table *table_alloc(const char *options, ...);

/**
   @param table a table to destroy and free
   
   Performs cleanup on table and then frees it. Calls to table_alloc() should be followed with a call to 
   this function

   Returns NULL on failure.
 */
void table_free(struct table *table);

/**
   @param table the table to update
   @param key the key for which the data needs to be updated
   @param data the value to set for \p key

   Updates data value for \p key in \p table, if \p key is not in \p table then a new entry is created
   from \p key with the value of \p data.
   
   Returns zero on success and a negative value on failure.
 */
int table_update(struct table *table, const char *key, tdata_t data);
/**
   @param table the table to update
   @param key the key for which the data needs to be updated
   @param data the value to set for \p key

   Updates data value for \p key in \p table, if \p key is not in \p table then a negative value is returned.
   
   Returns zero on success and a negative value on failure.
 */
int table_update_only(struct table *table, const char *key, tdata_t data);

/**
   @param table the table to search
   @param key the key for which to search
   @param data the entry for \p key shall be passed back with this pointer
  
   Searches \p table for \p key and returns its entry value using \p data.
   
   Returns zero on success and a negative value if \p key is not found.
 */
int table_search(struct table *table, const char *key, tdata_t *data);


#endif
