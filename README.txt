
To compile input commands:
cd output
cmake -DDEBUG_MODE=<ON/OFF> ..
cmake --build .
cd ..
./CacheSimulator.bin	-f <trace_file name without extetion>			// default - linpack_val.trc
						-c <confif file name without extetion>			// default - default_config.config
