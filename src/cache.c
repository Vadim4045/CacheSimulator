#include "cache.h"
#include <stdlib.h>

void cache_init
(cache* cache, uint levels_num, uint bank_size, uint page_size, uint counter_width)
{
	uint i;
	cache->_cache_levels_num = levels_num;
	cache->_cache_levels_inst = (cache_level **)malloc(levels_num * sizeof(cache_level *));
	DEBUG("%s will make %d cache levels:\n", __FUNCTION__, levels_num);
	for (i = 0; i < levels_num; ++i)
	{
		cache->_cache_levels_inst[i] = (cache_level*)malloc(sizeof(cache_level));
		cache_level_init(cache->_cache_levels_inst[i], bank_size, page_size, counter_width);
	}
}

void release_cache_resources(cache *cache)
{
	uint i;
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		DEBUG("%s release level %d\n", __FUNCTION__, i);
		release_cache_level_resources(cache->_cache_levels_inst[i]);
		free(cache->_cache_levels_inst[i]);
	}
	free(cache->_cache_levels_inst);
	DEBUG("%s released cache\n", __FUNCTION__);
}