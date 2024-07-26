#include "cache.h"
#include "cashe_level.h"
#include "../common.h"
#include <stdlib.h>

void cache_init(cache *cache, config *cfg)
{
	unsigned int i;
	cache->_bus_width = cfg->bus_width;
	cache->_cache_levels_num = cfg->cache_cfg.cache_levels;
	for (i = 0; i < cfg->cache_cfg.cache_levels; ++i)
	{
		cache_level_init(&cache->_cache_levels_inst[i], i, &(cfg->cache_cfg.cache_configs[i]), cfg->bus_width, cfg->page_size);
	}
}

RET_STATUS cache_read(cache *cache, unsigned int *addr, unsigned int *log)
{
	DEBUG("cache %s addr = 0x%x\n", __FUNCTION__, *addr);
	BOOL write = False;
	unsigned int i, tmp_addr;
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		*log |= 1 << i;
		
		if(level_read_data(&(cache->_cache_levels_inst[i]), *addr) == HIT)
		{
			return HIT;
		}
	}

	*addr = GET_FIELD(*addr, cache->_cache_levels_inst[0].sets_arr[0]._offcet_width
		, (cache->_cache_levels_inst[0].sets_arr[0]._tag_width + cache->_cache_levels_inst[0].sets_arr[0]._set_width)); // now we work with page addr
		
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		*log |= 1 << (5 + i);
		if (level_store_page(&(cache->_cache_levels_inst[i]), addr, &write) == HIT)
		{
			break;
		}
	}
	
	return WRITE_BACK;
}

RET_STATUS cache_write(cache *cache, unsigned int *addr, unsigned int* log)
{
	DEBUG("cache %s addr = 0x%x\n", __FUNCTION__, *addr);
	BOOL write = True;
	unsigned int i;

	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		*log |= 1 << i;

		if (level_write_data(&(cache->_cache_levels_inst[i]), *addr) == HIT)
		{
			return HIT;
		}
	}

	*addr = GET_FIELD(*addr, cache->_cache_levels_inst[0].sets_arr[0]._offcet_width
		, (cache->_cache_levels_inst[0].sets_arr[0]._tag_width + cache->_cache_levels_inst[0].sets_arr[0]._set_width)); // now we work with page addr
		

	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		*log |= 1 << (5 + i);

		if (level_store_page(&(cache->_cache_levels_inst[i]), addr, &write) == HIT)
		{
			break;
		}
	}

	return WRITE_BACK;
}

void print_cache(cache *cache)
{
	unsigned int i;
	printf("Chache levels addresses:\n");
	for (i = 0; i < cache->_cache_levels_num; ++i)
	{
		print_level(&(cache->_cache_levels_inst[i]));
	}
	
}