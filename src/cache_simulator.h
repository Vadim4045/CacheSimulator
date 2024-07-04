#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "defines.h"

void cache_init(cache *cache, uint levels_num, uint bank_size, uint page_size, uint counter_width);
void release_cache_resources(cache *cache);

#endif // CACHE_SIMULATOR_H