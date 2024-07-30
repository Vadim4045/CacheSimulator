
Compile from directory "./output":
	cmake ..
	cmake --build .


Run from project directory:
	./CacheSimulator.bin 	< flags >	 
							< -c path_to_config>	  
							< -t name_of_trace>   
							< -r type_of_range 2 16 ... values>

All flags and parameters are optional. 

-h -		show README
-d -		disable date raising to L1 on L2/L3 HIT
-l -		add result to statistic table
-v -		store verbosity trace log for inspect(will override if exists)
-i -		clear cach after trace(invalidation cost will add to total cost)
-m -		print cache map on end of trace(before invalidation)
-c -		path should be from root directory. Will be loaded default.congif, if not used.
-t -		path to trace file from project directory. Will run all taces in directory "./traces"., if not used.

This parameter mast be last!!!
-r -		name of parameter to range (should be as in config file). 
		Maximum number of values - 10 (each value mast be decimal)





