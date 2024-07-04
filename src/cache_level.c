#include "cashe_level.h"

#include <math.h>

void cache_level_init(cache_level *cache, uint bank_size, uint page_size, uint counter_width)
{
	cache->_pages = bank_size / page_size;
	cache->_offcet_width = log(page_size);
	cache->_set_width = log(cache->_pages);
	cache->_tag_width = 8 * SYS_BUS - cache->_set_width - cache->_offcet_width;
	cache->_counter_width = counter_width;

	// pages_arr       = (unsigned char*) malloc(bank_size);
	cache->addr_arr = (uint *)malloc(4 * cache->_pages);
	DEBUG("%s allocated %d bytes for %d pages\n", __FUNCTION__, (page_size * cache->_pages), cache->_pages);
}

void release_cache_level_resources(cache_level *cache)
{
	free(cache->addr_arr);
}

void set_valid(cache_level *cache, uint set)
{
	cache->addr_arr[set] |= (1 << cache->_tag_width);
}

BOOL is_valid(cache_level *cache, uint set)
{
	return (BOOL)((cache->addr_arr[set] & (1 << cache->_tag_width)) != 0);
}

void set_writed(cache_level *cache, uint set)
{
	cache->addr_arr[set] |= (1 << (cache->_tag_width + 1));
}

BOOL is_writed(cache_level *cache, uint set)
{
	return (BOOL)(cache->addr_arr[set] & (1 << (cache->_tag_width + 1)));
}

void set_tag(cache_level *cache, uint addr)
{
	cache->addr_arr[PART_NUM(addr, cache->_offcet_width, cache->_set_width)] 
			= PART_NUM(addr, cache->_offcet_width + cache->_set_width, cache->_tag_width);
}

void rlu_decrement(cache_level *cache, uint set)
{
	uint counter = rlu_get_counter(cache, set) - 1;
	cache->addr_arr[set] &= ~(MASK(cache->_counter_width) << (cache->_tag_width + 2));
	cache->addr_arr[set] |= counter << (cache->_tag_width + 2);
}

void rlu_setmax(cache_level *cache, uint set)
{
	cache->addr_arr[set] |= MASK(cache->_counter_width) << (cache->_tag_width + 2);
}

uint rlu_get_counter(cache_level *cache, uint set)
{
	return PART_NUM(cache->addr_arr[set], (cache->_tag_width + 2), cache->_counter_width);
}

RET_STATUS read_data(cache_level *cache, uint addr)
{
	if ((PART_NUM(addr, cache->_offcet_width + cache->_set_width, cache->_tag_width) // tag
			 == cache->addr_arr[PART_NUM(addr, cache->_offcet_width, cache->_set_width)] &
		 MASK(cache->_tag_width))														// stored tag
		& (is_valid(cache, PART_NUM(addr, cache->_offcet_width, cache->_set_width)))) // is valid
	{
		return HIT;
	}

	return MISS;
}

RET_STATUS write_data(cache_level *cache, uint addr)
{
	if ((PART_NUM(addr, cache->_offcet_width + cache->_set_width, cache->_tag_width) 
		== cache->addr_arr[PART_NUM(addr, cache->_offcet_width, cache->_set_width)] & MASK(cache->_tag_width)) // stored tag
		& (is_valid(cache, PART_NUM(addr, cache->_offcet_width, cache->_set_width))))// is valid
	{
		set_writed(cache, PART_NUM(addr, cache->_offcet_width, cache->_set_width));
		return HIT;
	}
	return MISS;
}

RET_STATUS store_page(cache_level *cache, uint addr, BOOL write)
{
	RET_STATUS res = HIT;
	uint set = PART_NUM(addr, (cache->_offcet_width), cache->_set_width);

	if (is_valid(cache, set) && is_writed(cache, set))
	{
		res = WRITE_BACK;
	}

	set_tag(cache, addr);
	rlu_setmax(cache, set);
	set_valid(cache, set);
	if (write)
		set_writed(cache, set);

	return res;
}
