
#include "cache.h"
#include "parser.h"

int main(int argc, char** argv){
	printf("%s start:\n", __FUNCTION__);
	cache _cache;
	DEBUG("%s Initial cache:\n", __FUNCTION__);
	cache_init(&_cache, 3, 1024 * 1024, 1024, 8);
	release_cache_resources(&_cache);
}