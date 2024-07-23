#include "cache_simulator.h"
#include "cashe_level.h"
#include <stdlib.h>


void cache_init(cache* cache, uint levels_num, uint bank_size, uint page_size, uint sets)
{
	uint i;
	cache->_cache_levels_num = levels_num;
	for (i = 0; i < levels_num; ++i)
	{
		cache_level_init(&cache->_cache_levels_inst[i], bank_size, page_size, sets);
	}
}

RET_STATUS cache_read(cache *cache, uint addr, uint *cost)
{

	return HIT;
}

RET_STATUS cache_write(cache *cache, uint addr, uint *cost)
{

	return HIT;
}