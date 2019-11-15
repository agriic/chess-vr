# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/dpetrovs/Documents/GIT/chess-vr

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/dpetrovs/Documents/GIT/chess-vr

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.15.5/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.15.5/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/dpetrovs/Documents/GIT/chess-vr/CMakeFiles /Users/dpetrovs/Documents/GIT/chess-vr/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/dpetrovs/Documents/GIT/chess-vr/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named generate_version

# Build rule for target.
generate_version: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 generate_version
.PHONY : generate_version

# fast build rule for target.
generate_version/fast:
	$(MAKE) -f CMakeFiles/generate_version.dir/build.make CMakeFiles/generate_version.dir/build
.PHONY : generate_version/fast

#=============================================================================
# Target rules for targets named chess-vr

# Build rule for target.
chess-vr: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 chess-vr
.PHONY : chess-vr

# fast build rule for target.
chess-vr/fast:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/build
.PHONY : chess-vr/fast

src/App.o: src/App.cpp.o

.PHONY : src/App.o

# target to build an object file
src/App.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/App.cpp.o
.PHONY : src/App.cpp.o

src/App.i: src/App.cpp.i

.PHONY : src/App.i

# target to preprocess a source file
src/App.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/App.cpp.i
.PHONY : src/App.cpp.i

src/App.s: src/App.cpp.s

.PHONY : src/App.s

# target to generate assembly for a file
src/App.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/App.cpp.s
.PHONY : src/App.cpp.s

src/Log.o: src/Log.cpp.o

.PHONY : src/Log.o

# target to build an object file
src/Log.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Log.cpp.o
.PHONY : src/Log.cpp.o

src/Log.i: src/Log.cpp.i

.PHONY : src/Log.i

# target to preprocess a source file
src/Log.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Log.cpp.i
.PHONY : src/Log.cpp.i

src/Log.s: src/Log.cpp.s

.PHONY : src/Log.s

# target to generate assembly for a file
src/Log.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Log.cpp.s
.PHONY : src/Log.cpp.s

src/Utils.o: src/Utils.cpp.o

.PHONY : src/Utils.o

# target to build an object file
src/Utils.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Utils.cpp.o
.PHONY : src/Utils.cpp.o

src/Utils.i: src/Utils.cpp.i

.PHONY : src/Utils.i

# target to preprocess a source file
src/Utils.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Utils.cpp.i
.PHONY : src/Utils.cpp.i

src/Utils.s: src/Utils.cpp.s

.PHONY : src/Utils.s

# target to generate assembly for a file
src/Utils.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/Utils.cpp.s
.PHONY : src/Utils.cpp.s

src/VR.o: src/VR.cpp.o

.PHONY : src/VR.o

# target to build an object file
src/VR.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VR.cpp.o
.PHONY : src/VR.cpp.o

src/VR.i: src/VR.cpp.i

.PHONY : src/VR.i

# target to preprocess a source file
src/VR.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VR.cpp.i
.PHONY : src/VR.cpp.i

src/VR.s: src/VR.cpp.s

.PHONY : src/VR.s

# target to generate assembly for a file
src/VR.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VR.cpp.s
.PHONY : src/VR.cpp.s

src/VideoInput.o: src/VideoInput.cpp.o

.PHONY : src/VideoInput.o

# target to build an object file
src/VideoInput.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VideoInput.cpp.o
.PHONY : src/VideoInput.cpp.o

src/VideoInput.i: src/VideoInput.cpp.i

.PHONY : src/VideoInput.i

# target to preprocess a source file
src/VideoInput.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VideoInput.cpp.i
.PHONY : src/VideoInput.cpp.i

src/VideoInput.s: src/VideoInput.cpp.s

.PHONY : src/VideoInput.s

# target to generate assembly for a file
src/VideoInput.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/VideoInput.cpp.s
.PHONY : src/VideoInput.cpp.s

src/main.o: src/main.cpp.o

.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i

.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s

.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) -f CMakeFiles/chess-vr.dir/build.make CMakeFiles/chess-vr.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... generate_version"
	@echo "... chess-vr"
	@echo "... src/App.o"
	@echo "... src/App.i"
	@echo "... src/App.s"
	@echo "... src/Log.o"
	@echo "... src/Log.i"
	@echo "... src/Log.s"
	@echo "... src/Utils.o"
	@echo "... src/Utils.i"
	@echo "... src/Utils.s"
	@echo "... src/VR.o"
	@echo "... src/VR.i"
	@echo "... src/VR.s"
	@echo "... src/VideoInput.o"
	@echo "... src/VideoInput.i"
	@echo "... src/VideoInput.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

