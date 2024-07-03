#ifndef CACHE_H
#define CACHE_H

#include "cashe_level.h"

typedef struct
{
	unsigned int _cache_levels_num;
	cache_level **_cache_levels_inst;

} cache;

void cache_init(cache *cache, unsigned int levels_num, unsigned int bank_size, unsigned int page_size, unsigned int counter_width);
void release_cache_resources(cache *cache);

#endif // CACHE_H