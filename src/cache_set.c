#include "cache_set.h"
#include <math.h>

void cache_set_init(cache_set *set, uint bank_size, uint page_size, uint sets)
{
	set->_pages = bank_size / page_size;
	set->_offcet_width = log(page_size);
	set->_set_width = log(set->_pages);
	set->_tag_width = 8 * SYS_BUS - set->_set_width - set->_offcet_width;
	set->_counter_width = log(sets);

	// pages_arr       = (unsigned char*) malloc(bank_size);
	set->addr_arr = (uint *)calloc(set->_pages, sizeof(uint));
	DEBUG("%s allocated addresses for %d pages\n", __FUNCTION__,  set->_pages);
}

void release_cache_set_resources(cache_set*set)
{
	free(set->addr_arr);
}

void set_valid(cache_set*set, uint val)
{
	set->addr_arr[val] |= (1 << set->_tag_width);
}

BOOL is_valid(cache_set *set, uint val)
{
	return (BOOL)((set->addr_arr[val] & (1 << set->_tag_width)) != 0);
}

void set_writed(cache_set *set, uint val)
{
	set->addr_arr[val] |= (1 << (set->_tag_width + 1));
}

BOOL is_writed(cache_set *set, uint val)
{
	return (BOOL)(set->addr_arr[val] & (1 << (set->_tag_width + 1)));
}

void set_tag(cache_set*set, uint addr)
{
	set->addr_arr[PART_NUM(addr, set->_offcet_width, set->_set_width)] = PART_NUM(addr, set->_offcet_width + set->_set_width, set->_tag_width);
}

void rlu_decrement(cache_set *set, uint val)
{
	uint counter = rlu_get_counter(set, val) - 1;
	set->addr_arr[val] &= ~(MASK(set->_counter_width) << (set->_tag_width + 2));
	set->addr_arr[val] |= counter << (set->_tag_width + 2);
}

void rlu_setmax(cache_set *set, uint val)
{
	set->addr_arr[val] |= MASK(set->_counter_width) << (set->_tag_width + 2);
}

uint rlu_get_counter(cache_set *set, uint val)
{
	return PART_NUM(set->addr_arr[val], (set->_tag_width + 2), set->_counter_width);
}

RET_STATUS read_data(cache_set*set, uint addr)
{
	if ((PART_NUM(addr, set->_offcet_width + set->_set_width, set->_tag_width) // tag
			 == set->addr_arr[PART_NUM(addr, set->_offcet_width, set->_set_width)] &
		 MASK(set->_tag_width))													  // stored tag
		& (is_valid(set, PART_NUM(addr, set->_offcet_width, set->_set_width)))) // is valid
	{
		return HIT;
	}

	return MISS;
}

RET_STATUS write_data(cache_set*set, uint addr)
{
	if ((PART_NUM(addr, set->_offcet_width + set->_set_width, set->_tag_width) == set->addr_arr[PART_NUM(addr, set->_offcet_width, set->_set_width)] & MASK(set->_tag_width)) // stored tag
		& (is_valid(set, PART_NUM(addr, set->_offcet_width, set->_set_width))))																										// is valid
	{
		set_writed(set, PART_NUM(addr, set->_offcet_width, set->_set_width));
		return HIT;
	}
	return MISS;
}

RET_STATUS store_page(cache_set*set, uint addr, BOOL write)
{
	RET_STATUS res = HIT;
	uint val = PART_NUM(addr, (set->_offcet_width), set->_set_width);

	if (is_valid(set, val) && is_writed(set, val))
	{
		res = WRITE_BACK;
	}

	set_tag(set, addr);
	rlu_setmax(set, val);
	set_valid(set, val);
	if (write)
		set_writed(set, val);

	return res;
}