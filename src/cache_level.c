#include "cashe_level.h"
#include "cache_set.h"

void cache_level_init(cache_level *cache, cache_level_config *cfg, uint bus_width, uint page_size)
{
	uint i;
	cache->_bank_size = cfg->size;
	cache->_page_size = page_size;
	cache->_sets = cfg->sets;

	// pages_arr       = (unsigned char*) malloc(bank_size);
	cache->sets_status = (uint *)calloc(cfg->sets, sizeof(uint));
	cache->sets_arr = (cache_set *)calloc(cfg->sets, sizeof(cache_set));
	for (i = 0; i < cfg->sets; ++i)
	{
		cache_set_init(&(cache->sets_arr[i]), (cfg->size / cfg->sets), bus_width, page_size, cfg->sets);
	}
	DEBUG("%s allocated %d sets for %d pages\n", __FUNCTION__, cache->_sets, cache->_bank_size / cache->_sets);
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

RET_STATUS read(cache_level *cache, uint addr) // full DDR addr
{
	RET_STATUS ret;
	uint i;
	
	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = read_data(&(cache->sets_arr[i]), addr)) == HIT) break;
	}

	return ret;
}

RET_STATUS write(cache_level *cache, uint addr)// full DDR addr
{
	RET_STATUS ret;
	uint i;

	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = write_data(&(cache->sets_arr[i]), addr)) == HIT)
			break;
	}

	return ret;
}

RET_STATUS store_page(cache_level *cache, uint addr, BOOL write)// page addr
{
	RET_STATUS ret;
	uint i, tmp_val, idx;
	for (i = 0; i < cache->_sets; ++i)
	{
		cache->sets_status[i] = get_page_status(&(cache->sets_arr[i]), addr);
		if ((cache->sets_status[i] & 0x1) == 0)// not valid
		{
			store_page(&(cache->sets_arr[i]), addr, write);
			level_rlu_decrement(cache, addr, i);
			return HIT;
		}
	}

	idx = 0;
	tmp_val = MASK(cache->sets_arr[0]._counter_width);
	for (i = 0; i < cache->_sets; ++i)
	{
		if (PART_NUM(cache->sets_status[i], 2, cache->sets_arr[0]._counter_width) < tmp_val)
		{
			tmp_val = PART_NUM(cache->sets_status[i], 2, cache->sets_arr[0]._counter_width);
			idx = i;
		}
	}

	ret = store_page(&(cache->sets_arr[idx]), addr, write);
	level_rlu_decrement(cache, addr, idx);

	return ret;
}

void level_rlu_decrement(cache_level *cache, uint addr, uint idx)
{
	uint i;
	for (i = 0; i < cache->_sets; ++i)
	{
		if (i != idx)
		{
			rlu_decrement(&(cache->sets_arr[idx]), addr);
		}
	}
}