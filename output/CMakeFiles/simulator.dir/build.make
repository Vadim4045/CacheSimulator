# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.1/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/boss/CacheSimulator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/boss/CacheSimulator/output

# Include any dependencies generated for this target.
include CMakeFiles/simulator.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/simulator.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/simulator.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simulator.dir/flags.make

CMakeFiles/simulator.dir/src/cache.c.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/src/cache.c.o: /Users/boss/CacheSimulator/src/cache.c
CMakeFiles/simulator.dir/src/cache.c.o: CMakeFiles/simulator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/boss/CacheSimulator/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/simulator.dir/src/cache.c.o"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/simulator.dir/src/cache.c.o -MF CMakeFiles/simulator.dir/src/cache.c.o.d -o CMakeFiles/simulator.dir/src/cache.c.o -c /Users/boss/CacheSimulator/src/cache.c

CMakeFiles/simulator.dir/src/cache.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/simulator.dir/src/cache.c.i"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/boss/CacheSimulator/src/cache.c > CMakeFiles/simulator.dir/src/cache.c.i

CMakeFiles/simulator.dir/src/cache.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/simulator.dir/src/cache.c.s"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/boss/CacheSimulator/src/cache.c -o CMakeFiles/simulator.dir/src/cache.c.s

CMakeFiles/simulator.dir/src/cache_level.c.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/src/cache_level.c.o: /Users/boss/CacheSimulator/src/cache_level.c
CMakeFiles/simulator.dir/src/cache_level.c.o: CMakeFiles/simulator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/boss/CacheSimulator/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/simulator.dir/src/cache_level.c.o"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/simulator.dir/src/cache_level.c.o -MF CMakeFiles/simulator.dir/src/cache_level.c.o.d -o CMakeFiles/simulator.dir/src/cache_level.c.o -c /Users/boss/CacheSimulator/src/cache_level.c

CMakeFiles/simulator.dir/src/cache_level.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/simulator.dir/src/cache_level.c.i"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/boss/CacheSimulator/src/cache_level.c > CMakeFiles/simulator.dir/src/cache_level.c.i

CMakeFiles/simulator.dir/src/cache_level.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/simulator.dir/src/cache_level.c.s"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/boss/CacheSimulator/src/cache_level.c -o CMakeFiles/simulator.dir/src/cache_level.c.s

CMakeFiles/simulator.dir/src/main.c.o: CMakeFiles/simulator.dir/flags.make
CMakeFiles/simulator.dir/src/main.c.o: /Users/boss/CacheSimulator/src/main.c
CMakeFiles/simulator.dir/src/main.c.o: CMakeFiles/simulator.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/boss/CacheSimulator/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/simulator.dir/src/main.c.o"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/simulator.dir/src/main.c.o -MF CMakeFiles/simulator.dir/src/main.c.o.d -o CMakeFiles/simulator.dir/src/main.c.o -c /Users/boss/CacheSimulator/src/main.c

CMakeFiles/simulator.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/simulator.dir/src/main.c.i"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/boss/CacheSimulator/src/main.c > CMakeFiles/simulator.dir/src/main.c.i

CMakeFiles/simulator.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/simulator.dir/src/main.c.s"
	/opt/homebrew/bin/gcc-13 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/boss/CacheSimulator/src/main.c -o CMakeFiles/simulator.dir/src/main.c.s

# Object files for target simulator
simulator_OBJECTS = \
"CMakeFiles/simulator.dir/src/cache.c.o" \
"CMakeFiles/simulator.dir/src/cache_level.c.o" \
"CMakeFiles/simulator.dir/src/main.c.o"

# External object files for target simulator
simulator_EXTERNAL_OBJECTS =

/Users/boss/CacheSimulator/simulator: CMakeFiles/simulator.dir/src/cache.c.o
/Users/boss/CacheSimulator/simulator: CMakeFiles/simulator.dir/src/cache_level.c.o
/Users/boss/CacheSimulator/simulator: CMakeFiles/simulator.dir/src/main.c.o
/Users/boss/CacheSimulator/simulator: CMakeFiles/simulator.dir/build.make
/Users/boss/CacheSimulator/simulator: CMakeFiles/simulator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/boss/CacheSimulator/output/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable /Users/boss/CacheSimulator/simulator"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simulator.dir/build: /Users/boss/CacheSimulator/simulator
.PHONY : CMakeFiles/simulator.dir/build

CMakeFiles/simulator.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simulator.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simulator.dir/clean

CMakeFiles/simulator.dir/depend:
	cd /Users/boss/CacheSimulator/output && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/boss/CacheSimulator /Users/boss/CacheSimulator /Users/boss/CacheSimulator/output /Users/boss/CacheSimulator/output /Users/boss/CacheSimulator/output/CMakeFiles/simulator.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/simulator.dir/depend

