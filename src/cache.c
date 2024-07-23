#include "cache.h"
#include "cashe_level.h"
#include <stdlib.h>

void cache_init(cache *cache, config *cfg)
{
	uint i;
	cache->_cache_levels_num = cfg->cache_cfg.cache_levels;
	for (i = 0; i < cfg->cache_cfg.cache_levels; ++i)
	{
		cache_level_init(&cache->_cache_levels_inst[i], &(cfg->cache_cfg.cache_configs[i]), cfg->page_size);
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