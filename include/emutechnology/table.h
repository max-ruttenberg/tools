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

struct table_entry {
	const char *key;
	intptr_t data;
	struct list_head bucket;
};

struct table *table_alloc(const char *options);
void table_free(struct table *table);

int  table_init(struct table *table, const char *options);
void table_dest(struct table *table);

int table_update(struct table *table, const char *key, tdata_t data);
int table_update_only(struct table *table, const char *key, tdata_t data);
int table_search(struct table *table, const char *key, tdata_t *data);


#endif
