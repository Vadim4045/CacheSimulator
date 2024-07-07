
To compile input commands:
cd output
cmake -DDEBUG_MODE=<ON/OFF> ..
cmake --build .
../CacheSimulator	-f <trace_file name without extetion>			// default - linpack_vav.trc
					-c <confif file name without extetion>			// default - default_config.config
					-r <type of parameter to ranging> <all values>	// default - no range running

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
