#include "cashe_level.h"
#include "cache_set.h"
#include "common.h"

void cache_level_init(cache_level *level, unsigned int l, cache_level_config *cfg, unsigned int bus_width, unsigned int page_size)
{
	unsigned int i;
	level->level = l;
	level->_size = cfg->size;
	level->_page_size = page_size;
	level->_sets = cfg->sets;

	level->sets_status = (unsigned int *)calloc(cfg->sets, sizeof(unsigned int));
	level->sets_arr = (cache_set *)calloc(cfg->sets, sizeof(cache_set));
	for (i = 0; i < cfg->sets; ++i)
	{
		cache_set_init(&(level->sets_arr[i]), (cfg->size / cfg->sets), bus_width, page_size, cfg->sets);
	}
}

void release_cache_level(cache_level *level)
{
	int i;
	for (i = 0; i < level->_sets; ++i)
	{
		release_cache_set(&(level->sets_arr[i]));
	}

	free(level->sets_arr);
	free(level->sets_status);
}

RET_STATUS level_read_data(cache_level *level, unsigned int addr) // full DDR addr
{
	RET_STATUS ret;
	unsigned int i;
	
	for (i = 0; i < level->_sets; ++i)
	{
		if ((ret = set_read_data(&(level->sets_arr[i]), addr)) == HIT)
		{
			if (level->level)
			{
				set_valid(&(level->sets_arr[i]), ((addr >> level->sets_arr[i]._offcet_width) & GET_MASK(level->sets_arr[i]._set_width)), False);
			}
			break;
		}		
	}

	return ret;
}

RET_STATUS level_write_data(cache_level *level, unsigned int addr) // full DDR addr
{
	RET_STATUS ret;
	unsigned int i;

	for (i = 0; i < level->_sets; ++i)
	{
		if ((ret = set_write_data(&(level->sets_arr[i]), addr)) == HIT)
		{
			if(level->level)
			{
				set_valid(&(level->sets_arr[i]), ((addr >> level->sets_arr[i]._offcet_width) & GET_MASK(level->sets_arr[i]._set_width)), False);
			}
			break;
		}
	}

	return ret;
}

RET_STATUS level_store_page(cache_level *level, unsigned int *addr, BOOL * write)// page addr
{
	RET_STATUS ret;
	unsigned int i, tmp_val, idx, tmp_addr = *addr;
	
	for (i = 0; i < level->_sets; ++i)
	{
		level->sets_status[i] = get_page_status(&(level->sets_arr[i]), *addr);

		if ((level->sets_status[i] & 1) == 0)// not valid
		{
			level_rlu_decrement(level, tmp_addr);
			set_store_page(&(level->sets_arr[i]), addr, write);
			return HIT;
		}
	}

	idx = 0;
	tmp_val = GET_MASK(level->sets_arr[0]._counter_width);
	for (i = 0; i < level->_sets; ++i)
	{
		if (GET_FIELD(level->sets_status[i], 2, level->sets_arr[i]._counter_width) < tmp_val)
		{
			tmp_val = GET_FIELD(level->sets_status[i], 2, level->sets_arr[0]._counter_width);
			idx = i;
		}
	}

	level_rlu_decrement(level, tmp_addr);
	ret = set_store_page(&(level->sets_arr[idx]), addr, write);

	return ret;
}

RET_STATUS level_invalidate_step(cache_level *level, unsigned int *addr)
{
	unsigned int i;
	for (i = 0; i < level->_sets; ++i)
	{
		return set_invalidate_step(&(level->sets_arr[i]), addr);
	}
}

void level_rlu_decrement(cache_level *level, unsigned int addr)
{
	unsigned int i;
	for (i = 0; i < level->_sets; ++i)
	{
		rlu_decrement(&(level->sets_arr[i]), addr);
	}
}


void print_level(cache_level *level)
{
	unsigned int i;
	printf("Level %d have %d sets:\n", level->level, level->_sets);
	for (i = 0; i < level->_sets; ++i)
	{
		printf("\tSet %d", i);
		print_set(&(level->sets_arr[i]));
	}
}