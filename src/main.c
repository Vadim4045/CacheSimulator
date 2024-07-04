
#include "cache.h"
#include "parser.h"

#define MAX_PARAMS 10
typedef enum
{
	page_size,
	cache_levels,
	level1_size,
	level2_size,
	level3_size,
	write_policy,
	replaсement,
	associativity,
	interleaving
	
} range_type;



void runner(char *config_file, range_type type, uint first, ...)
{
	va_list args;
	va_start(args, type);

	int params[MAX_PARAMS];
	int num_params = 0;

	va_start(args, first);
	while (num_params < MAX_PARAMS)
	{
		int param = va_arg(args);
		if (param == -1)
			break;
		params[num_params++] = param;
	}
	va_end(args);

	for (int i = 0; i < num_params; ++i)
	{
		switch (type)
		{
		case page_size:
			test_function(params[i]);
			break;
		case cache_levels:
			test_function(params[i]);
			break;
		case level1_size:
			test_function(params[i]);
			break;
		case level2_size:
			test_function(params[i]);
			break;
		case level3_size:
			test_function(params[i]);
			break;
		case write_policy:
			test_function(params[i]);
			break;
		case replaсement:
			test_function(params[i]);
			break;
		case associativity:
			test_function(params[i]);
			break;
		case interleaving:
			test_function(params[i]);
			break;
		default:
			printf("Unknown range_type\n");
			break;
		}
	}
}

int main(int argc, char** argv){
	printf("%s start:\n", __FUNCTION__);
	cache _cache;
	DEBUG("%s Initial cache:\n", __FUNCTION__);
	cache_init(&_cache, 3, 1024 * 1024, 1024, 8);
	release_cache_resources(&_cache);
}