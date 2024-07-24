#include "cashe_level.h"
#include "cache_set.h"

void cache_level_init(cache_level *cache, cache_level_config *cfg, uint page_size)
{
	uint i;
	cache->_bank_size = cfg->size;
	cache->_page_size = page_size;
	cache->_sets = cfg->sets;

	// pages_arr       = (unsigned char*) malloc(bank_size);
	cache->sets_arr = (cache_set *)calloc(cfg->sets, sizeof(cache_set));
	for (i = 0; i < cfg->sets; ++i)
	{
		cache_set_init(&(cache->sets_arr[i]), (cfg->size / cfg->sets), page_size, cfg->sets);
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
}

RET_STATUS read(cache_level *cache, uint addr)
{


	return MISS;
}

RET_STATUS write(cache_level *cache, uint addr)
{

	return MISS;
}

RET_STATUS store(cache_level *cache, uint addr, BOOL write)
{
	RET_STATUS res = HIT;

	return res;
}
