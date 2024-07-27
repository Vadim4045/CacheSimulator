#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include "defines.h"

void cache_level_init(cache_level *cache, unsigned int level, cache_level_config *cfg, unsigned int bus_width, unsigned int page_size);
void release_cache_level_resources(cache_level *cache);

RET_STATUS level_read_data(cache_level *cache, unsigned int addr);
RET_STATUS level_write_data(cache_level *cache, unsigned int addr);
RET_STATUS level_store_page(cache_level *cache, unsigned int *addr, BOOL *write);
RET_STATUS level_invalidate_step(cache_level *cache, unsigned int *addr);

void level_rlu_decrement(cache_level *cache, unsigned int addr);
void print_level(cache_level *cache);

#endif // CACHE_LEVEL_H