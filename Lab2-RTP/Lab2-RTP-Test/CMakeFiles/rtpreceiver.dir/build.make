# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test

# Include any dependencies generated for this target.
include CMakeFiles/rtpreceiver.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rtpreceiver.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rtpreceiver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtpreceiver.dir/flags.make

CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o: CMakeFiles/rtpreceiver.dir/flags.make
CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o: src/receiver_def.cpp
CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o: CMakeFiles/rtpreceiver.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o -MF CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o.d -o CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o -c /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/receiver_def.cpp

CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/receiver_def.cpp > CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.i

CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/receiver_def.cpp -o CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.s

# Object files for target rtpreceiver
rtpreceiver_OBJECTS = \
"CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o"

# External object files for target rtpreceiver
rtpreceiver_EXTERNAL_OBJECTS =

librtpreceiver.a: CMakeFiles/rtpreceiver.dir/src/receiver_def.cpp.o
librtpreceiver.a: CMakeFiles/rtpreceiver.dir/build.make
librtpreceiver.a: CMakeFiles/rtpreceiver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library librtpreceiver.a"
	$(CMAKE_COMMAND) -P CMakeFiles/rtpreceiver.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtpreceiver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtpreceiver.dir/build: librtpreceiver.a
.PHONY : CMakeFiles/rtpreceiver.dir/build

CMakeFiles/rtpreceiver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtpreceiver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtpreceiver.dir/clean

CMakeFiles/rtpreceiver.dir/depend:
	cd /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles/rtpreceiver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtpreceiver.dir/depend

