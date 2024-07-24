#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"

void cache_level_init(cache_level *cache, cache_level_config *cfg, uint bus_width, uint page_size);
void release_cache_level_resources(cache_level *cache);


RET_STATUS read(cache_level *cache, uint addr);
RET_STATUS write(cache_level *cache, uint addr);
RET_STATUS store_page(cache_level *cache, uint addr, BOOL write);

void level_rlu_decrement(cache_level *cache, uint addr, uint idx);

#endif // CACHE_LEVEL_H