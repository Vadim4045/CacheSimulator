
To compile input commands:
	cd output
	cmake -DDEBUG_MODE=<ON/OFF> ..
	cmake --build .


To run in root directory:
	./CacheSimulator.bin < -h >	 < -c path_to_config>	  < -t name_of_trace>   < -r type_of_range 2 16 ... values>

All parameters are optional. 

-h -		show README
-v -		store verbosity trace log for inspect(will override if exists)
-i -		invalidate cach after trace(invalidation cost will add to total cost)
-m -		print cache map on end of trace(before invalidation)
-c -		path should be from root directory. If not used it, will be loaded default.congif
-t -		path to trace file from roo directory. If not used it, will run all taces in directory "traces".

This parameter mast be last!!!
-r -		name of parameter to range (should be as in config file). 
		Maximum number of values - 10 (each value mast be decimal and power of 2)
