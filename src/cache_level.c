#include "cashe_level.h"
#include "cache_set.h"

void cache_level_init(cache_level *cache, uint bank_size, uint page_size, uint sets)
{
	uint i;
	cache->_bank_size = bank_size;
	cache->_page_size = page_size;
	cache->_sets = sets;

	// pages_arr       = (unsigned char*) malloc(bank_size);
	cache->sets_arr = (cache_set*)calloc(sets, sizeof(cache_set));
	for(i = 0; i < sets; ++i)
	{
		cache_level_init(&(cache->sets_arr[i]), bank_size, page_size, sets);
	}
	DEBUG("%s allocated %d sets for %d pages\n", __FUNCTION__, (page_size * cache->_sets), cache->_bank_size / cache->_page_size);
}

void release_cache_level_resources(cache_level *cache)
{
	int i;
	for (i = 0; i < cache->_sets; ++i)
	{
		release_cache_level_resources(&(cache->sets_arr[i]));
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
