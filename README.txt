
To compile input commands:
cd output
cmake -DDEBUG_MODE=<ON/OFF> ..
cmake --build .
../CacheSimulator <trace_file name without extetion> <type of parameter to ranging> <all values>

Types of ranging:
	page_size
	cache_levels
	level1_size
	level2_size
	level3_size
	write_policy
	replaсement
	associativity
	interleaving
