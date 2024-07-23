#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"

void cache_set_init(cache_set*set, uint bank_size, uint page_size, uint sets);
void release_cache_set_resources(cache_set*set);

void set_valid(cache_set*set, uint val);
BOOL is_valid(cache_set*set, uint val);

void set_writed(cache_set*set, uint val);
BOOL is_writed(cache_set*set, uint val);

void set_tag(cache_set*set, uint addr);

void rlu_decrement(cache_set*set, uint val);
void rlu_setmax(cache_set*set, uint val);
uint rlu_get_counter(cache_set*set, uint val);

RET_STATUS read_data(cache_set*set, uint addr);
RET_STATUS write_data(cache_set*set, uint addr);
RET_STATUS store_page(cache_set*set, uint addr, BOOL write);

#endif // CACHE_SET_H

