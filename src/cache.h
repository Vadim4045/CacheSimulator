#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "defines.h"

void cache_init(cache *cache, config *cfg);

RET_STATUS cache_read(cache *cache, uint *addr, uint *cost);
RET_STATUS cache_write(cache *cache, uint *addr, uint *cost);

void print_cache(cache *cache);

#endif // CACHE_SIMULATOR_H