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
include CMakeFiles/rtpsender.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rtpsender.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rtpsender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rtpsender.dir/flags.make

CMakeFiles/rtpsender.dir/src/sender_def.cpp.o: CMakeFiles/rtpsender.dir/flags.make
CMakeFiles/rtpsender.dir/src/sender_def.cpp.o: src/sender_def.cpp
CMakeFiles/rtpsender.dir/src/sender_def.cpp.o: CMakeFiles/rtpsender.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rtpsender.dir/src/sender_def.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rtpsender.dir/src/sender_def.cpp.o -MF CMakeFiles/rtpsender.dir/src/sender_def.cpp.o.d -o CMakeFiles/rtpsender.dir/src/sender_def.cpp.o -c /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/sender_def.cpp

CMakeFiles/rtpsender.dir/src/sender_def.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rtpsender.dir/src/sender_def.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/sender_def.cpp > CMakeFiles/rtpsender.dir/src/sender_def.cpp.i

CMakeFiles/rtpsender.dir/src/sender_def.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rtpsender.dir/src/sender_def.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/src/sender_def.cpp -o CMakeFiles/rtpsender.dir/src/sender_def.cpp.s

# Object files for target rtpsender
rtpsender_OBJECTS = \
"CMakeFiles/rtpsender.dir/src/sender_def.cpp.o"

# External object files for target rtpsender
rtpsender_EXTERNAL_OBJECTS =

librtpsender.a: CMakeFiles/rtpsender.dir/src/sender_def.cpp.o
librtpsender.a: CMakeFiles/rtpsender.dir/build.make
librtpsender.a: CMakeFiles/rtpsender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library librtpsender.a"
	$(CMAKE_COMMAND) -P CMakeFiles/rtpsender.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rtpsender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rtpsender.dir/build: librtpsender.a
.PHONY : CMakeFiles/rtpsender.dir/build

CMakeFiles/rtpsender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rtpsender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rtpsender.dir/clean

CMakeFiles/rtpsender.dir/depend:
	cd /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test /home/jijianan/ComputerNetwork/lab2-rtp-NorthmanPKU/Lab2-RTP-Test/CMakeFiles/rtpsender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rtpsender.dir/depend

