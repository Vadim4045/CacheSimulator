#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include <stdlib.h>
#include <stdio.h>

#include "defines.h"

void cache_level_init(cache_level *cache, cache_level_config *cfg, uint page_size);
void release_cache_level_resources(cache_level *cache);


RET_STATUS read(cache_level *cache, uint addr);
RET_STATUS write(cache_level *cache, uint addr);
RET_STATUS store(cache_level *cache, uint addr, BOOL write);

#endif // CACHE_LEVEL_H