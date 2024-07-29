#include "cache_set.h"
#include "common.h"

void cache_set_init(cache_set *set, unsigned int bank_size, unsigned int bus_width, unsigned int page_size, unsigned int sets)
{
	unsigned int i;
	set->_pages = bank_size / page_size;
	set->_offcet_width = log2int(page_size);
	set->_set_width = log2int(set->_pages);
	set->_tag_width = BITS_IN_BYTE * bus_width - set->_set_width - set->_offcet_width; // 2GB max in DDR
	set->_counter_width = log2int(sets);

	// pages_arr       = (unsigned char*) malloc(bank_size);
	set->addr_arr = (unsigned int *)calloc(set->_pages, sizeof(unsigned int));
}

void release_cache_set(cache_set*set)
{
	free(set->addr_arr);
}

RET_STATUS set_read_data(cache_set *set, unsigned int addr) // full DDR addr
{
	unsigned int page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);

	if (((addr >> (set->_offcet_width + set->_set_width))			// tag
		 == (set->addr_arr[page_num] >> (set->_counter_width + 2))) // stored tag
		&& (is_valid(set, page_num)))								// is valid
	{
		return HIT;
	}

	return MISS;
}

RET_STATUS set_write_data(cache_set *set, unsigned int addr) // full DDR addr
{
	unsigned int page_num = GET_FIELD(addr, set->_offcet_width, set->_set_width);
	
	if (((addr >> (set->_offcet_width + set->_set_width)) 				// tag
		 == (set->addr_arr[page_num] >> (set->_counter_width + 2)))		// stored tag
		&& (is_valid(set, page_num)))									// is valid																															  // is valid
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
	tag = *addr >> set->_set_width;

	if (is_valid(set, page_num))// busy
	{
		*addr = get_page_addr(set, page_num);
		writeback_write = ((set->addr_arr[page_num] >> 1) & 1);
		res = WRITE_BACK;
	}else
	{
		res = HIT;
	}

	set_tag(set, page_num, tag);
	set_valid(set, page_num, True);
	if (*write){
		set_writed(set, page_num);
	}
	rlu_setmax(set, page_num);
	
	*write = writeback_write;
	
	return res;
}

RET_STATUS set_invalidate_step(cache_set *set, unsigned int *addr)
{
	unsigned int i;
	for (i = 0; i < set->_pages; ++i)
	{
		if ((set->addr_arr[i] & 1) && (set->addr_arr[i] & 2))// valid and writed
		{
			*addr = get_page_addr(set, i);
			set->addr_arr[i] = 0;
			return HIT;
		}
		else
		{
			set->addr_arr[i] = 0;
		}
	}
	return MISS;
}

void print_set(cache_set *set)
{
	unsigned int i;
	printf(" <tag|valid|writed|counter>:");
	for (i = 0; i < set->_pages; ++i)
	{
		if (i % 8 == 0)
			printf("\n\t");
		printf("%8x|%d|%d|%d\t", (set->addr_arr[i] >> (set->_counter_width + 2)), (set->addr_arr[i] & 1), ((set->addr_arr[i] >> 1) & 1), GET_FIELD(set->addr_arr[i], 2, set->_counter_width));
	}
	printf("\n");
}

unsigned int get_page_status(cache_set *set, unsigned int addr)// page addr
{
	return set->addr_arr[addr & GET_MASK(set->_set_width)];
}

void set_valid(cache_set *set, unsigned int page_num, BOOL val) // page number
{
	if(val)
	{
		set->addr_arr[page_num] |= 1;
	}
	else
	{
		set->addr_arr[page_num] = 0;
		
	}
	
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

unsigned int get_page_addr(cache_set *set, unsigned int page_num)
{
	return ((set->addr_arr[page_num] >> (set->_counter_width + 2)) << (set->_set_width)) + page_num;
}

void rlu_decrement(cache_set *set, unsigned int addr) // page addr
{
	unsigned int page_num = addr & GET_MASK(set->_set_width);
	unsigned int counter = rlu_get_counter(set, page_num);

	if(counter)
	{
		SET_FIELD(set->addr_arr[page_num], 2, set->_counter_width, (counter - 1));
	}
	
}

void rlu_setmax(cache_set *set, unsigned int page_num) // page number
{
	set->addr_arr[page_num] |= (GET_MASK(set->_counter_width) << 2);
}

unsigned int rlu_get_counter(cache_set *set, unsigned int page_num)
{
	return GET_FIELD(set->addr_arr[page_num], 2, set->_counter_width);
}
