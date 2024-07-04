
To compile input commands:
cd output
cmake -DDEBUG_MODE=<ON/OFF> ..
cmake --build .
../CacheSimulator <trace_file path> <type of parameter to ranging> <all range values>

Types of ranging:
	page_size,
	cache_levels = 0
	level1_size = 1
	level2_size = 2
	level3_size = 3
	write_policy = 4
	replaсement = 5
	associativity = 6
	interleaving = 7
