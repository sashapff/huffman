# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.13.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.13.4/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alexandra/Desktop/huffman

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alexandra/Desktop/huffman/build

# Include any dependencies generated for this target.
include CMakeFiles/testing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testing.dir/flags.make

CMakeFiles/testing.dir/gtest/gtest-all.cc.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/gtest/gtest-all.cc.o: ../gtest/gtest-all.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandra/Desktop/huffman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testing.dir/gtest/gtest-all.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/gtest/gtest-all.cc.o -c /Users/alexandra/Desktop/huffman/gtest/gtest-all.cc

CMakeFiles/testing.dir/gtest/gtest-all.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/gtest/gtest-all.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandra/Desktop/huffman/gtest/gtest-all.cc > CMakeFiles/testing.dir/gtest/gtest-all.cc.i

CMakeFiles/testing.dir/gtest/gtest-all.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/gtest/gtest-all.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandra/Desktop/huffman/gtest/gtest-all.cc -o CMakeFiles/testing.dir/gtest/gtest-all.cc.s

CMakeFiles/testing.dir/gtest/gtest_main.cc.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/gtest/gtest_main.cc.o: ../gtest/gtest_main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandra/Desktop/huffman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/testing.dir/gtest/gtest_main.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/gtest/gtest_main.cc.o -c /Users/alexandra/Desktop/huffman/gtest/gtest_main.cc

CMakeFiles/testing.dir/gtest/gtest_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/gtest/gtest_main.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandra/Desktop/huffman/gtest/gtest_main.cc > CMakeFiles/testing.dir/gtest/gtest_main.cc.i

CMakeFiles/testing.dir/gtest/gtest_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/gtest/gtest_main.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandra/Desktop/huffman/gtest/gtest_main.cc -o CMakeFiles/testing.dir/gtest/gtest_main.cc.s

CMakeFiles/testing.dir/test/testing_huffman.cpp.o: CMakeFiles/testing.dir/flags.make
CMakeFiles/testing.dir/test/testing_huffman.cpp.o: test/testing_huffman.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alexandra/Desktop/huffman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/testing.dir/test/testing_huffman.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testing.dir/test/testing_huffman.cpp.o -c /Users/alexandra/Desktop/huffman/build/test/testing_huffman.cpp

CMakeFiles/testing.dir/test/testing_huffman.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testing.dir/test/testing_huffman.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alexandra/Desktop/huffman/build/test/testing_huffman.cpp > CMakeFiles/testing.dir/test/testing_huffman.cpp.i

CMakeFiles/testing.dir/test/testing_huffman.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testing.dir/test/testing_huffman.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alexandra/Desktop/huffman/build/test/testing_huffman.cpp -o CMakeFiles/testing.dir/test/testing_huffman.cpp.s

# Object files for target testing
testing_OBJECTS = \
"CMakeFiles/testing.dir/gtest/gtest-all.cc.o" \
"CMakeFiles/testing.dir/gtest/gtest_main.cc.o" \
"CMakeFiles/testing.dir/test/testing_huffman.cpp.o"

# External object files for target testing
testing_EXTERNAL_OBJECTS =

testing: CMakeFiles/testing.dir/gtest/gtest-all.cc.o
testing: CMakeFiles/testing.dir/gtest/gtest_main.cc.o
testing: CMakeFiles/testing.dir/test/testing_huffman.cpp.o
testing: CMakeFiles/testing.dir/build.make
testing: libhuffman_lib.a
testing: CMakeFiles/testing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alexandra/Desktop/huffman/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable testing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testing.dir/build: testing

.PHONY : CMakeFiles/testing.dir/build

CMakeFiles/testing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testing.dir/clean

CMakeFiles/testing.dir/depend:
	cd /Users/alexandra/Desktop/huffman/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alexandra/Desktop/huffman /Users/alexandra/Desktop/huffman /Users/alexandra/Desktop/huffman/build /Users/alexandra/Desktop/huffman/build /Users/alexandra/Desktop/huffman/build/CMakeFiles/testing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testing.dir/depend

