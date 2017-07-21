#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <errno.h>
#include <internal/printing.h>
#include <emutechnology/table.h>
#include <emutechnology/zalloc.h>
#include <emutechnology/list.h>
#include <emutechnology/strdupa.h>

#define ABSOLUTE_MAX   (1<<30) /* no more than a billion entries */
#define E_MAX_DEFAULT  (1<<13)
#define E_SIZE_DEFAULT (1<<10)

static int table_insert_entry(struct table *table, struct table_entry *entryp);

/* buckets from size */
static size_t bfs(size_t size)
{
	return 2*size - 1;
}

static unsigned default_hash(const char *key)
{
	unsigned h = 0x811c9dc5;
	const unsigned char *c;
	
	for (c = (const unsigned char *)key; *c; c++)
		h = (h ^ *c) *0x01000193;
	
	return h;
}

static void parse_opt(struct table *table, char *option)
{
	char *p, *v;
	
	p = strtok_r(option, "=", &v);
	if (!p)
		return;
	
	if (strcmp(p, "max_size")==0) {
		table->e_max = atol(v);
		if (table->e_max > ABSOLUTE_MAX)
			table->e_max = ABSOLUTE_MAX;
	} else if (strcmp(p, "size")==0) {
		table->e_size = atol(v);
	}
}
static void parse_opts(struct table *table, const char *options)
{
	char *tmp, *opt, *delim = ",";
	
	tmp = strdupa(options);
	for (opt = strtok_r(tmp, delim, &tmp); opt; opt = strtok_r(tmp, delim, &tmp)) {
		parse_opt(table, opt);
	}
}

static int table_init_parameters(struct table *table)
{
	if (table->e_size) {
		/* caller has set size */		
		if (!table->e_max) {
			/* caller has set size but not max => max = max(default,size)*/
			table->e_max = table->e_size < E_MAX_DEFAULT ? E_MAX_DEFAULT : table->e_size;
		} else if (table->e_max < table->e_size) {
			/* caller has set size and max and size > max => error */
			return -EINVAL; 
		} 
	} else {
		table->e_size = E_SIZE_DEFAULT;
	}
	
	if (table->e_max) {
		if (table->e_max < table->e_size)
			/* this only occurs if caller has set max but not size */
			table->e_size = table->e_max;
	} else {
		table->e_max = E_MAX_DEFAULT;
	}
	table->hash = default_hash;
	return 0;
}

static int table_init_buckets(struct table *table)
{
	table->n_entries = 0;
	table->buckets = calloc(bfs(table->e_size), sizeof(*(table->buckets)));
	if (!table->buckets)
		return -ENOMEM;	
	return 0;	
}

static void table_dest_buckets(struct table *table)
{
	struct table_entry *entryp, *tmp;
	unsigned i;
	for (i = 0; i < bfs(table->e_size); i++) 
		if (table->buckets[i]) {
			list_for_each_entry_safe(entryp, tmp, table->buckets[i], bucket) {
				list_del(&entryp->bucket);
				free((void*)entryp->key);
				free(entryp);
			}
			free(table->buckets[i]);
		}
	
	free(table->buckets);
	table->buckets = NULL;
	table->n_entries = 0;
}

int table_init(struct table *table, const char *options)
{
	int ret;

	memset(table, 0, sizeof(*table));
	parse_opts(table, options);
	ret = table_init_parameters(table);
	if (ret)
		return ret;
	ret = table_init_buckets(table);
	if (ret)
		return ret;
	return 0;
}

void table_dest(struct table *table)
{
	table_dest_buckets(table);
	return;
}

struct table *table_alloc(const char *options)
{
	struct table *table = zalloc(sizeof(*table));
	int ret;
	
	if (!table)
		return NULL;
	ret = table_init(table, options);
	if (ret) {
		free(table);
		return NULL;
	}
	return table;
}

void table_free(struct table *table)
{
	table_dest(table);
	free(table);	
}

static struct table_entry *table_search_entry(struct table *table, const char *key)
{
	unsigned h = table->hash(key)%bfs(table->e_size);
	struct list_head *bucketp = table->buckets[h];
	struct table_entry *entryp;

	pr_dbg("%s: hash for key '%s' is %u\n", __func__, key, h);
	if (!bucketp)
		return NULL;

	list_for_each_entry(entryp, bucketp, bucket)
		if (strcmp(entryp->key,key)==0) 
			return entryp;

	return NULL;
}

static int table_resize(struct table *table)
{
	size_t e_size, tmpsz;
	struct list_head **buckets, **tmpbuck;
	struct table_entry *entryp, *tmpe;
	int i;
	
	if (table->n_entries+1 >= table->e_size) {
		if (table->e_size >= table->e_max)
			return -1;
		
		e_size = table->e_size*2;
		if (e_size > table->e_max) 
			e_size = table->e_max;

		pr_dbg("%s: resizing to %zu\n", __func__, e_size);
		buckets = zalloc(bfs(e_size)*sizeof(*buckets));
		if (!buckets)
			return -1;
		
		tmpbuck = table->buckets;
		table->buckets = buckets;
		buckets = tmpbuck;		

		tmpsz = table->e_size;
		table->e_size = e_size;
		e_size = tmpsz;
		
		table->n_entries = 0;
		
		for (i = 0; i < bfs(e_size); i++) {
			if (!buckets[i]) {
				pr_dbg("%s: bucket[%d] is zero\n", __func__, i);
				continue;
			}
			list_for_each_entry_safe(entryp, tmpe, buckets[i], bucket) {
				list_del(&entryp->bucket);
				pr_dbg("%s: key=%s,data=%s\n",__func__,
				       entryp->key,
				       (const char*)entryp->data);
				table_insert_entry(table, entryp);
			}
			free(buckets[i]);
		}
	}
	return 0;
}

static int table_insert_entry(struct table *table, struct table_entry *entryp)
{
	int ret;
	unsigned h;
	struct list_head *bucketp;
	
	ret = table_resize(table); 
	if (ret)
		return ret;

	h = table->hash(entryp->key)%bfs(table->e_size);
	pr_dbg("%s: hash for key '%s' is %u\n", __func__, entryp->key, h);
	bucketp = table->buckets[h];
	if (!bucketp) {
		bucketp = zalloc(sizeof(*bucketp));
		if (!bucketp)
			return -1;
		
		INIT_LIST_HEAD(bucketp);
		list_add(&entryp->bucket, bucketp);
		table->buckets[h] = bucketp;
		table->n_entries++;
	} else {
		list_add(&entryp->bucket, bucketp);
	}
	return 0;
}

int table_update_only(struct table *table, const char *key, tdata_t data)
{
	struct table_entry *entryp = table_search_entry(table,key);

	if (!entryp) {
		return -1;
	} else {
		entryp->data = data;
		return 0;
	}
}

int table_update(struct table *table, const char *key, tdata_t data)
{
	struct table_entry *entryp;
	int ret;

	pr_dbg("%s: key=%s,data=%s\n",__func__,key,(const char*)data);
	entryp = table_search_entry(table, key);
	if (!entryp) {
		entryp = zalloc(sizeof(*entryp));
		if (!entryp)
			return -1;
		entryp->key = strdup(key);
		if (!entryp->key) {
			free(entryp);
			return -1;
		}
		entryp->data = data;
		ret = table_insert_entry(table, entryp);
		if (ret) {
			free((void*)entryp->key);
			free(entryp);
			return -1;
		}
		return 0;
	} else {
		entryp->data = data;
		return 0;
	}
}

int table_search(struct table *table, const char *key, tdata_t *data)
{
	struct table_entry *entryp = table_search_entry(table, key);	

	if (!entryp)
		return -1;
	
	*data = entryp->data;
	return 0;
}