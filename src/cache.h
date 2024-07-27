#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "defines.h"

void cache_init(cache *cache, config *cfg);

RET_STATUS cache_read(cache *cache, unsigned int *addr, unsigned int *log);
RET_STATUS cache_write(cache *cache, unsigned int *addr, unsigned int *log);
unsigned int cache_invalidate_step(cache *cache);

unsigned int cache_invalidate_step(cache *cache);
void print_cache(cache *cache);

#endif // CACHE_SIMULATOR_H