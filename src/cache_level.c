#include "cashe_level.h"
#include "cache_set.h"
#include "common.h"

void cache_level_init(cache_level *cache, uint level, cache_level_config *cfg, uint bus_width, uint page_size)
{
	uint i;
	cache->level = level;
	cache->_size = cfg->size;
	cache->_page_size = page_size;
	cache->_sets = cfg->sets;

	// pages_arr       = (unsigned char*) malloc(bank_size);
	cache->sets_status = (uint *)calloc(cfg->sets, sizeof(uint));
	cache->sets_arr = (cache_set *)calloc(cfg->sets, sizeof(cache_set));
	for (i = 0; i < cfg->sets; ++i)
	{
		cache_set_init(&(cache->sets_arr[i]), (cfg->size / cfg->sets), bus_width, page_size, cfg->sets);
	}
	DEBUG("%s level %d allocated %d sets for %d pages\n", __FUNCTION__, cache->level, cache->_sets, cache->_size / page_size);
}

void release_cache_level_resources(cache_level *cache)
{
	int i;
	for (i = 0; i < cache->_sets; ++i)
	{
		release_cache_set_resources(&(cache->sets_arr[i]));
	}
	DEBUG("%s deallocated %d sets\n", __FUNCTION__, cache->_sets);
	free(cache->sets_arr);
	free(cache->sets_status);
}

RET_STATUS level_read_data(cache_level *cache, uint addr) // full DDR addr
{
	RET_STATUS ret;
	uint i;
	
	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = set_read_data(&(cache->sets_arr[i]), addr)) == HIT)
			break;
	}
	DEBUG("cache_level %d %s addr = 0x%x (%s)\n", cache->level, __FUNCTION__, addr, ret2str(ret));
	return ret;
}

RET_STATUS level_write_data(cache_level *cache, uint addr) // full DDR addr
{
	RET_STATUS ret;
	uint i;

	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = set_write_data(&(cache->sets_arr[i]), addr)) == HIT)
			break;
	}
	DEBUG("cache_level %d %s addr = 0x%x (%s)\n", cache->level, __FUNCTION__, addr, ret2str(ret));
	return ret;
}

RET_STATUS level_store_page(cache_level *cache, uint *addr, BOOL * write)// page addr
{
	RET_STATUS ret;
	uint i, tmp_val, idx, tmp_addr = *addr;
	
	for (i = 0; i < cache->_sets; ++i)
	{
		cache->sets_status[i] = get_page_status(&(cache->sets_arr[i]), *addr);

		if (!(cache->sets_status[i] & 1))// not valid
		{
			set_store_page(&(cache->sets_arr[i]), addr, write);
			level_rlu_decrement(cache, tmp_addr, i);
			DEBUG("cache_level %d(set %d) %s addr = 0x%x (%s)\n", cache->level, i, __FUNCTION__, *addr, ret2str(HIT));
			return HIT;
		}
	}

	idx = 0;
	tmp_val = GET_MASK(cache->sets_arr[i]._counter_width);
	for (i = 0; i < cache->_sets; ++i)
	{
		if (GET_FIELD(cache->sets_status[i], 2, cache->sets_arr[i]._counter_width) < tmp_val)
		{
			tmp_val = GET_FIELD(cache->sets_status[i], 2, cache->sets_arr[0]._counter_width);
			idx = i;
		}
	}

	ret = set_store_page(&(cache->sets_arr[idx]), addr, write);
	level_rlu_decrement(cache, tmp_addr, idx);
	DEBUG("cache_level %d(set %d) %s addr = 0x%x (%s)\n", cache->level, i, __FUNCTION__, *addr, ret2str(ret));
	return ret;
}

void level_rlu_decrement(cache_level *cache, uint addr, uint idx)
{
	uint i;
	for (i = 0; i < cache->_sets; ++i)
	{
		if (i != idx)
		{
			rlu_decrement(&(cache->sets_arr[i]), addr);
		}
	}
}

void print_level(cache_level *cache)
{
	uint i;
	printf("Level %d have %d sets:\n", cache->level, cache->_sets);
	for (i = 0; i < cache->_sets; ++i)
	{
		printf("Set %d", i);
		print_set(&(cache->sets_arr[i]));
	}
}