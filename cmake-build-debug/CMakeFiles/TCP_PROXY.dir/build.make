# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jberegon/Desktop/TCP_PROXY

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TCP_PROXY.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/TCP_PROXY.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TCP_PROXY.dir/flags.make

CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o: CMakeFiles/TCP_PROXY.dir/flags.make
CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o: ../tcpproxy_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o -c /Users/jberegon/Desktop/TCP_PROXY/tcpproxy_server.cpp

CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jberegon/Desktop/TCP_PROXY/tcpproxy_server.cpp > CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.i

CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jberegon/Desktop/TCP_PROXY/tcpproxy_server.cpp -o CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.s

# Object files for target TCP_PROXY
TCP_PROXY_OBJECTS = \
"CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o"

# External object files for target TCP_PROXY
TCP_PROXY_EXTERNAL_OBJECTS =

TCP_PROXY: CMakeFiles/TCP_PROXY.dir/tcpproxy_server.cpp.o
TCP_PROXY: CMakeFiles/TCP_PROXY.dir/build.make
TCP_PROXY: /Users/jberegon/.brew/Cellar/boost/1.78.0_1/lib/libboost_regex-mt.dylib
TCP_PROXY: CMakeFiles/TCP_PROXY.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable TCP_PROXY"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TCP_PROXY.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TCP_PROXY.dir/build: TCP_PROXY
.PHONY : CMakeFiles/TCP_PROXY.dir/build

CMakeFiles/TCP_PROXY.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TCP_PROXY.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TCP_PROXY.dir/clean

CMakeFiles/TCP_PROXY.dir/depend:
	cd /Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jberegon/Desktop/TCP_PROXY /Users/jberegon/Desktop/TCP_PROXY /Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug /Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug /Users/jberegon/Desktop/TCP_PROXY/cmake-build-debug/CMakeFiles/TCP_PROXY.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TCP_PROXY.dir/depend

