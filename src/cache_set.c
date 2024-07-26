#include "cache_set.h"
#include "common.h"
#include <math.h>

void cache_set_init(cache_set *set, uint bank_size, uint bus_width, uint page_size, uint sets)
{
	set->_pages = bank_size / page_size;
	set->_offcet_width = log2(page_size);
	set->_set_width = log2(set->_pages);
	set->_tag_width = 8 * bus_width - set->_set_width - set->_offcet_width;// 2GB max in DDR
	set->_counter_width = log2(sets);

	// pages_arr       = (unsigned char*) malloc(bank_size);
	set->addr_arr = (uint *)calloc(set->_pages, sizeof(uint));
	DEBUG("%s allocated addresses for %d pages\n", __FUNCTION__,  set->_pages);
	DEBUG("\tpages = %d\n",set->_pages);
	DEBUG("\tset->_offcet_width = %d\n", set->_offcet_width);
	DEBUG("\tset->_set_width = %d\n", set->_set_width);
	DEBUG("\tset->_tag_width = %d\n", set->_tag_width);
	DEBUG("\tset->_counter_width = %d\n", set->_counter_width);
}

void release_cache_set_resources(cache_set*set)
{
	free(set->addr_arr);
}

RET_STATUS set_read_data(cache_set *set, uint addr) // full DDR addr
{
	uint page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);
	DEBUG("cache_set %s addr = 0x%x (%d)\n", __FUNCTION__, addr, page_num);
	
	if ((GET_FIELD(addr, set->_offcet_width + set->_set_width, set->_tag_width)	// tag
			 == (set->addr_arr[page_num] >> (set->_counter_width + 2)))		 	// stored tag
			&& (is_valid(set, page_num))) 										// is valid
	{
		return HIT;
	}

	return MISS;
}

RET_STATUS set_write_data(cache_set *set, uint addr) // full DDR addr
{
	uint page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);
	DEBUG("cache_set %s addr = 0x%x (%d)\n", __FUNCTION__, addr, page_num);
	if ((GET_FIELD(addr, set->_offcet_width + set->_set_width, set->_tag_width) // tag
		 == (set->addr_arr[page_num] >> (set->_counter_width + 2)))			   // stored tag
		&& (is_valid(set, page_num)))										   // is valid																															  // is valid
	{
		set_writed(set, page_num);
		return HIT;
	}
	
	return MISS;
}

RET_STATUS set_store_page(cache_set *set, uint *addr, BOOL *write) // page DDR addr
{
	RET_STATUS res = HIT;
	BOOL writeback_write = False;
	uint page_num, tag;
	
	page_num = *addr & GET_MASK(set->_set_width);
	tag = GET_FIELD(*addr, set->_set_width, set->_tag_width);

	if (is_valid(set, page_num))
	{

		*addr = (GET_FIELD(set->addr_arr[page_num], (set->_counter_width + 2), set->_tag_width) << (set->_set_width)) + page_num;
		writeback_write = ((set->addr_arr[page_num] >> 1) & 1);
		res = WRITE_BACK;
	}else
	{
		res = HIT;
	}

	set_tag(set, page_num, tag);
	rlu_setmax(set, page_num);
	set_valid(set, page_num);
	if (*write){
		set_writed(set, page_num);
	}

	*write = writeback_write;
	DEBUG("cache_set %s got addr: 0x%x and stored tag = 0x%x in sell %d\n", __FUNCTION__, *addr, tag, page_num);
	return res;
}

uint get_page_status(cache_set *set, uint addr)// page addr
{
	return set->addr_arr[addr & GET_MASK(set->_set_width)];
}

void set_valid(cache_set *set, uint page_num) // page number
{
	set->addr_arr[page_num] |= 1;
}

BOOL is_valid(cache_set *set, uint page_num) // page number
{
	return (BOOL)(set->addr_arr[page_num] & 1);
}

void set_writed(cache_set *set, uint page_num) // page number
{
	set->addr_arr[page_num] |= 2;
}

BOOL is_writed(cache_set *set, uint page_num) // page number
{
	return (BOOL)((set->addr_arr[page_num] & 2) != 0);
}

void set_tag(cache_set *set, uint page_num, uint tag) // tag
{
	set->addr_arr[page_num] = tag << (set->_counter_width + 2);
}

void rlu_decrement(cache_set *set, uint addr) // page addr
{
	uint page_num = addr & set->_set_width;
	uint counter = rlu_get_counter(set, page_num) - 1;
	set->addr_arr[page_num] &= ~(GET_MASK(set->_counter_width) << 2);
	set->addr_arr[page_num] |= counter << 2;
}

void rlu_setmax(cache_set *set, uint addr) // page number
{
	uint page_num = addr & set->_set_width;
	set->addr_arr[page_num] |= GET_MASK(set->_counter_width) << 2;
}

uint rlu_get_counter(cache_set *set, uint page_num)
{
	return GET_FIELD(set->addr_arr[page_num], 2, set->_counter_width);
}

void print_set(cache_set *set)
{
	uint i;
	printf(" have %d addresses(tag(writed, valid)):", set->_pages);
	for (i = 0; i < set->_pages; ++i)
	{
		if(i%8 == 0) printf("\n");
		printf("0x%x(%d,%d), ", (set->addr_arr[i] >> (set->_counter_width + 2)), ((set->addr_arr[i] >> 1) & 1), (set->addr_arr[i] & 1));
	}
	printf("\n");
}