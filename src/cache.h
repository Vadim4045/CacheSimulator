#ifndef CACHE_H
#define CACHE_H

#include "cashe_level.h"

typedef struct
{
	uint _cache_levels_num;
	cache_level **_cache_levels_inst;

} cache;

void cache_init(cache *cache, uint levels_num, uint bank_size, uint page_size, uint counter_width);
void release_cache_resources(cache *cache);

#endif // CACHE_H