# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ewan/Development/codeplay/FloatFormat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ewan/Development/codeplay/FloatFormat/build-clang

# Include any dependencies generated for this target.
include CMakeFiles/FloatFormat.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FloatFormat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FloatFormat.dir/flags.make

CMakeFiles/FloatFormat.dir/FF.cpp.o: CMakeFiles/FloatFormat.dir/flags.make
CMakeFiles/FloatFormat.dir/FF.cpp.o: ../FF.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ewan/Development/codeplay/FloatFormat/build-clang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FloatFormat.dir/FF.cpp.o"
	/home/ewan/Development/codeplay/ComputeAorta/build/llvm/bin/clang  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FloatFormat.dir/FF.cpp.o -c /home/ewan/Development/codeplay/FloatFormat/FF.cpp

CMakeFiles/FloatFormat.dir/FF.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FloatFormat.dir/FF.cpp.i"
	/home/ewan/Development/codeplay/ComputeAorta/build/llvm/bin/clang $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ewan/Development/codeplay/FloatFormat/FF.cpp > CMakeFiles/FloatFormat.dir/FF.cpp.i

CMakeFiles/FloatFormat.dir/FF.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FloatFormat.dir/FF.cpp.s"
	/home/ewan/Development/codeplay/ComputeAorta/build/llvm/bin/clang $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ewan/Development/codeplay/FloatFormat/FF.cpp -o CMakeFiles/FloatFormat.dir/FF.cpp.s

# Object files for target FloatFormat
FloatFormat_OBJECTS = \
"CMakeFiles/FloatFormat.dir/FF.cpp.o"

# External object files for target FloatFormat
FloatFormat_EXTERNAL_OBJECTS =

libFloatFormat.so.1.0: CMakeFiles/FloatFormat.dir/FF.cpp.o
libFloatFormat.so.1.0: CMakeFiles/FloatFormat.dir/build.make
libFloatFormat.so.1.0: CMakeFiles/FloatFormat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ewan/Development/codeplay/FloatFormat/build-clang/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libFloatFormat.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FloatFormat.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_library libFloatFormat.so.1.0 libFloatFormat.so.1 libFloatFormat.so

libFloatFormat.so.1: libFloatFormat.so.1.0
	@$(CMAKE_COMMAND) -E touch_nocreate libFloatFormat.so.1

libFloatFormat.so: libFloatFormat.so.1.0
	@$(CMAKE_COMMAND) -E touch_nocreate libFloatFormat.so

# Rule to build all files generated by this target.
CMakeFiles/FloatFormat.dir/build: libFloatFormat.so

.PHONY : CMakeFiles/FloatFormat.dir/build

CMakeFiles/FloatFormat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FloatFormat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FloatFormat.dir/clean

CMakeFiles/FloatFormat.dir/depend:
	cd /home/ewan/Development/codeplay/FloatFormat/build-clang && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ewan/Development/codeplay/FloatFormat /home/ewan/Development/codeplay/FloatFormat /home/ewan/Development/codeplay/FloatFormat/build-clang /home/ewan/Development/codeplay/FloatFormat/build-clang /home/ewan/Development/codeplay/FloatFormat/build-clang/CMakeFiles/FloatFormat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FloatFormat.dir/depend
