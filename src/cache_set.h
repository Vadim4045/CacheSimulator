#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"

void cache_set_init(cache_set *set, uint bank_size, uint bus_width, uint page_size, uint sets);
void release_cache_set_resources(cache_set*set);


RET_STATUS set_read_data(cache_set*set, uint addr);
RET_STATUS set_write_data(cache_set*set, uint addr);
RET_STATUS set_store_page(cache_set*set, uint addr, BOOL write);

uint get_page_status(cache_set *set, uint addr);

void set_valid(cache_set *set, uint val);
BOOL is_valid(cache_set *set, uint val);

void set_writed(cache_set *set, uint val);
BOOL is_writed(cache_set *set, uint val);

void set_tag(cache_set *set, uint addr);

void rlu_decrement(cache_set *set, uint addr);
void rlu_setmax(cache_set *set, uint addr);
uint rlu_get_counter(cache_set *set, uint val);

#endif // CACHE_SET_H

