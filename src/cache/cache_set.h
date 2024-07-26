#ifndef CACHE_SET_H
#define CACHE_SET_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"

void cache_set_init(cache_set *set, unsigned int bank_size, unsigned int bus_width, unsigned int page_size, unsigned int sets);
void release_cache_set_resources(cache_set*set);


RET_STATUS set_read_data(cache_set*set, unsigned int addr);
RET_STATUS set_write_data(cache_set*set, unsigned int addr);
RET_STATUS set_store_page(cache_set*set, unsigned int *addr, BOOL *write);

unsigned int get_page_status(cache_set *set, unsigned int addr);

void set_valid(cache_set *set, unsigned int page_num);
BOOL is_valid(cache_set *set, unsigned int page_num);

void set_writed(cache_set *set, unsigned int page_num);
BOOL is_writed(cache_set *set, unsigned int page_num);

void set_tag(cache_set *set, unsigned int page_num, unsigned int tag);

void rlu_decrement(cache_set *set, unsigned int addr);
void rlu_setmax(cache_set *set, unsigned int addr);
unsigned int rlu_get_counter(cache_set *set, unsigned int page_num);
void print_set(cache_set *page_num);
#endif // CACHE_SET_H

