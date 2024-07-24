#include "cache.h"
#include "cashe_level.h"
#include <stdlib.h>

void cache_init(cache *cache, config *cfg)
{
	uint i;
	cache->_cache_levels_num = cfg->cache_cfg.cache_levels;
	for (i = 0; i < cfg->cache_cfg.cache_levels; ++i)
	{
		cache_level_init(&cache->_cache_levels_inst[i], &(cfg->cache_cfg.cache_configs[i]), cfg->bus_width, cfg->page_size);
	}
}

RET_STATUS cache_read(cache *cache, uint addr, uint *cost)
{
	RET_STATUS ret;
	uint i;
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		if ((ret = level_read_data(&(cache->_cache_levels_inst[i]), addr)) == HIT)
		{
			
		}
	}
		return HIT;
}

RET_STATUS cache_write(cache *cache, uint addr, uint* cost)
{
	RET_STATUS ret;
	uint i;
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		if ((ret = level_write_data(&(cache->_cache_levels_inst[i]), addr)) == HIT)
		{
			
		}
	}
	return HIT;
}