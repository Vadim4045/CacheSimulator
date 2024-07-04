#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"


void cache_level_init(cache_level *cache, uint bank_size, uint page_size, uint counter_width);
void release_cache_level_resources(cache_level *cache);

void set_valid(cache_level *cache, uint set);
BOOL is_valid(cache_level *cache, uint set);

void set_writed(cache_level *cache, uint set);
BOOL is_writed(cache_level *cache, uint set);

void set_tag(cache_level *cache, uint addr);

void rlu_decrement(cache_level *cache, uint set);
void rlu_setmax(cache_level *cache, uint set);
uint rlu_get_counter(cache_level *cache, uint set);

RET_STATUS read_data(cache_level *cache, uint addr);
RET_STATUS write_data(cache_level *cache, uint addr);
RET_STATUS store_page(cache_level *cache, uint addr, BOOL write);

#endif // CACHE_LEVEL_H