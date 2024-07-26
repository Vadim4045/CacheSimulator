#include "cache_set.h"
#include "../common.h"

void cache_set_init(cache_set *set, unsigned int bank_size, unsigned int bus_width, unsigned int page_size, unsigned int sets)
{
	unsigned int i;
	set->_pages = bank_size / page_size;
	set->_offcet_width = log2int(page_size);
	set->_set_width = log2int(set->_pages);
	set->_tag_width = 8 * bus_width - set->_set_width - set->_offcet_width;// 2GB max in DDR
	set->_counter_width = log2int(sets);

	// pages_arr       = (unsigned char*) malloc(bank_size);
	set->addr_arr = (unsigned int *)calloc(set->_pages, sizeof(unsigned int));
	
	DEBUG("%s allocated addresses for %d pages\n", __FUNCTION__, set->_pages);
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

RET_STATUS set_read_data(cache_set *set, unsigned int addr) // full DDR addr
{
	unsigned int page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);
	DEBUG("cache_set %s addr = 0x%x (%d)\n", __FUNCTION__, addr, page_num);
	
	if ((GET_FIELD(addr, set->_offcet_width + set->_set_width, set->_tag_width)	// tag
			 == (set->addr_arr[page_num] >> (set->_counter_width + 2)))		 	// stored tag
			&& (is_valid(set, page_num))) 										// is valid
	{
		return HIT;
	}

	return MISS;
}

RET_STATUS set_write_data(cache_set *set, unsigned int addr) // full DDR addr
{
	unsigned int page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);
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

RET_STATUS set_store_page(cache_set *set, unsigned int *addr, BOOL *write) // page DDR addr
{
	RET_STATUS res = HIT;
	BOOL writeback_write = False;
	unsigned int page_num, tag;
	
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
	set_valid(set, page_num);
	if (*write){
		set_writed(set, page_num);
	}
	rlu_setmax(set, page_num);
	
	*write = writeback_write;
	DEBUG("cache_set %s got addr: 0x%x and stored tag = 0x%x in sell %d\n", __FUNCTION__, *addr, tag, page_num);
	return res;
}

unsigned int get_page_status(cache_set *set, unsigned int addr)// page addr
{
	return set->addr_arr[addr & GET_MASK(set->_set_width)];
}

void set_valid(cache_set *set, unsigned int page_num) // page number
{
	set->addr_arr[page_num] |= 1;
}

BOOL is_valid(cache_set *set, unsigned int page_num) // page number
{
	return (BOOL)(set->addr_arr[page_num] & 1);
}

void set_writed(cache_set *set, unsigned int page_num) // page number
{
	set->addr_arr[page_num] |= 2;
}

BOOL is_writed(cache_set *set, unsigned int page_num) // page number
{
	return (BOOL)((set->addr_arr[page_num] & 2) != 0);
}

void set_tag(cache_set *set, unsigned int page_num, unsigned int tag) // tag
{
	set->addr_arr[page_num] = (tag << (set->_counter_width + 2));
}

void rlu_decrement(cache_set *set, unsigned int addr) // page addr
{
	unsigned int page_num = addr & set->_set_width;
	unsigned int counter = rlu_get_counter(set, page_num) & set->_counter_width;

	if(counter)
	{
		SET_FIELD(set->addr_arr[page_num], 2, set->_counter_width, (counter - 1));
	}
	
}

void rlu_setmax(cache_set *set, unsigned int addr) // page number
{
	unsigned int page_num = addr & set->_set_width;
	SET_FIELD(set->addr_arr[page_num], 2, set->_counter_width, GET_MASK(set->_counter_width));
}

unsigned int rlu_get_counter(cache_set *set, unsigned int page_num)
{
	return GET_FIELD(set->addr_arr[page_num], 2, set->_counter_width);
}

void print_set(cache_set *set)
{
	unsigned int i;
	printf(" (tag(counter,writed, valid)):");
	for (i = 0; i < set->_pages; ++i)
	{
		if(i%8 == 0) printf("\n\t\t");
		printf("0x%x(%d,%d,%d), ", (set->addr_arr[i] >> (set->_counter_width + 2))
			, GET_FIELD(set->addr_arr[i], 2, set->_counter_width), ((set->addr_arr[i] >> 1) & 1), (set->addr_arr[i] & 1));
	}
	printf("\n");
}