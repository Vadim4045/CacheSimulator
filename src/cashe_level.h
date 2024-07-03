#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include <stdlib.h>
#include <stdio.h>

#ifndef SYS_BUS

#define SYS_BUS 4

#define MASK(bits) ((1 << (bits)) - 1)
#define PART_NUM(num, pos, bits) (((num) >> (pos)) & MASK(bits))

#ifdef DEBUG_MODE
#define DEBUG(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG(fmt, ...)
#endif

typedef enum
{
	False = 0,
	True = 1
} BOOL;

typedef enum
{
	HIT,
	MISS,
	WRITE_BACK
} RET_STATUS;

#endif

/**
 * _tag_width bits - TAG
 *  1 bit - is valid
 *  1 bit - is writed
 *  _counter_width - LRU counter
 */
typedef struct {
	unsigned int _bank_size;
	unsigned int _page_size;
	unsigned int _pages;
	unsigned int _offcet_width;
	unsigned int _tag_width;
	unsigned int _set_width;
	unsigned int _counter_width;
	// unsigned char* pages_arr;
	unsigned int *addr_arr;

} cache_level;

void cache_level_init(cache_level *cache, unsigned int bank_size, unsigned int page_size, unsigned int counter_width);
void release_cache_level_resources(cache_level *cache);

void set_valid(cache_level *cache, unsigned int set);
BOOL is_valid(cache_level *cache, unsigned int set);

void set_writed(cache_level *cache, unsigned int set);
BOOL is_writed(cache_level *cache, unsigned int set);

void set_tag(cache_level *cache, unsigned int addr);

void rlu_decrement(cache_level *cache, unsigned int set);
void rlu_setmax(cache_level *cache, unsigned int set);
unsigned int rlu_get_counter(cache_level *cache, unsigned int set);

RET_STATUS read_data(cache_level *cache, unsigned int addr);
RET_STATUS write_data(cache_level *cache, unsigned int addr);
RET_STATUS store_page(cache_level *cache, unsigned int addr, BOOL write);

#endif // CACHE_LEVEL_H