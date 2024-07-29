#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include "defines.h"

void cache_level_init(cache_level *level, unsigned int l, cache_level_config *cfg, unsigned int bus_width, unsigned int page_size);

RET_STATUS level_read_data(cache_level *level, unsigned int addr);
RET_STATUS level_write_data(cache_level *level, unsigned int addr);
RET_STATUS level_store_page(cache_level *level, unsigned int *addr, BOOL *write);
RET_STATUS level_invalidate_step(cache_level *level, unsigned int *addr);

void level_rlu_decrement(cache_level *level, unsigned int addr);
void print_level(cache_level *level);

void release_cache_level(cache_level *level);

#endif // CACHE_LEVEL_H