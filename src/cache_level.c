#include "cashe_level.h"
#include "cache_set.h"
#include "common.h"

void cache_level_init(cache_level *cache, unsigned int level, cache_level_config *cfg, unsigned int bus_width, unsigned int page_size)
{
	unsigned int i;
	cache->level = level;
	cache->_size = cfg->size;
	cache->_page_size = page_size;
	cache->_sets = cfg->sets;

	cache->sets_status = (unsigned int *)calloc(cfg->sets, sizeof(unsigned int));
	cache->sets_arr = (cache_set *)calloc(cfg->sets, sizeof(cache_set));
	for (i = 0; i < cfg->sets; ++i)
	{
		cache_set_init(&(cache->sets_arr[i]), (cfg->size / cfg->sets), bus_width, page_size, cfg->sets);
	}
}

void release_cache_level(cache_level *cache)
{
	int i;
	for (i = 0; i < cache->_sets; ++i)
	{
		release_cache_set(&(cache->sets_arr[i]));
	}

	free(cache->sets_arr);
	free(cache->sets_status);
}

RET_STATUS level_read_data(cache_level *cache, unsigned int addr) // full DDR addr
{
	RET_STATUS ret;
	unsigned int i;
	
	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = set_read_data(&(cache->sets_arr[i]), addr)) == HIT)
		{
			if (cache->level)
			{
				set_valid(&(cache->sets_arr[i]), ((addr >> cache->sets_arr[i]._offcet_width) & GET_MASK(cache->sets_arr[i]._set_width)), False);
			}
			break;
		}		
	}

	return ret;
}

RET_STATUS level_write_data(cache_level *cache, unsigned int addr) // full DDR addr
{
	RET_STATUS ret;
	unsigned int i;

	for (i = 0; i < cache->_sets; ++i)
	{
		if ((ret = set_write_data(&(cache->sets_arr[i]), addr)) == HIT)
		{
			if(cache->level)
			{
				set_valid(&(cache->sets_arr[i]), ((addr >> cache->sets_arr[i]._offcet_width) & GET_MASK(cache->sets_arr[i]._set_width)), False);
			}
			break;
		}
	}

	return ret;
}

RET_STATUS level_store_page(cache_level *cache, unsigned int *addr, BOOL * write)// page addr
{
	RET_STATUS ret;
	unsigned int i, tmp_val, idx, tmp_addr = *addr;
	
	for (i = 0; i < cache->_sets; ++i)
	{
		cache->sets_status[i] = get_page_status(&(cache->sets_arr[i]), *addr);

		if ((cache->sets_status[i] & 1) == 0)// not valid
		{
			level_rlu_decrement(cache, tmp_addr);
			set_store_page(&(cache->sets_arr[i]), addr, write);
			return HIT;
		}
	}

	idx = 0;
	tmp_val = GET_MASK(cache->sets_arr[0]._counter_width);
	for (i = 0; i < cache->_sets; ++i)
	{
		if (GET_FIELD(cache->sets_status[i], 2, cache->sets_arr[i]._counter_width) < tmp_val)
		{
			tmp_val = GET_FIELD(cache->sets_status[i], 2, cache->sets_arr[0]._counter_width);
			idx = i;
		}
	}

	level_rlu_decrement(cache, tmp_addr);
	ret = set_store_page(&(cache->sets_arr[idx]), addr, write);

	return ret;
}

RET_STATUS level_invalidate_step(cache_level *cache, unsigned int *addr)
{
	unsigned int i;
	for (i = 0; i < cache->_sets; ++i)
	{
		return set_invalidate_step(&(cache->sets_arr[i]), addr);
	}
}

void level_rlu_decrement(cache_level *cache, unsigned int addr)
{
	unsigned int i;
	for (i = 0; i < cache->_sets; ++i)
	{
		rlu_decrement(&(cache->sets_arr[i]), addr);
	}
}


void print_level(cache_level *cache)
{
	unsigned int i;
	printf("Level %d have %d sets:\n", cache->level, cache->_sets);
	for (i = 0; i < cache->_sets; ++i)
	{
		printf("\tSet %d", i);
		print_set(&(cache->sets_arr[i]));
	}
}