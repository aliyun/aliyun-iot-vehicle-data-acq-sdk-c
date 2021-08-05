# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eason/CLionProjects/aliyun-iot-vehicle-data-acq-sdk-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eason/CLionProjects/aliyun-iot-vehicle-data-acq-sdk-c

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/home/eason/clion-2020.1.3/bin/cmake/linux/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/eason/CLionProjects/aliyun-iot-vehicle-data-acq-sdk-c/CMakeFiles /home/eason/CLionProjects/aliyun-iot-vehicle-data-acq-sdk-c/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/eason/CLionProjects/aliyun-iot-vehicle-data-acq-sdk-c/CMakeFiles 0
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
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named safety_engine_app_demo

# Build rule for target.
safety_engine_app_demo: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 safety_engine_app_demo
.PHONY : safety_engine_app_demo

# fast build rule for target.
safety_engine_app_demo/fast:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/build
.PHONY : safety_engine_app_demo/fast

#=============================================================================
# Target rules for targets named vehicle_daq_demo

# Build rule for target.
vehicle_daq_demo: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 vehicle_daq_demo
.PHONY : vehicle_daq_demo

# fast build rule for target.
vehicle_daq_demo/fast:
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/build
.PHONY : vehicle_daq_demo/fast

src/example/safety_engine_demo.o: src/example/safety_engine_demo.c.o

.PHONY : src/example/safety_engine_demo.o

# target to build an object file
src/example/safety_engine_demo.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/example/safety_engine_demo.c.o
.PHONY : src/example/safety_engine_demo.c.o

src/example/safety_engine_demo.i: src/example/safety_engine_demo.c.i

.PHONY : src/example/safety_engine_demo.i

# target to preprocess a source file
src/example/safety_engine_demo.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/example/safety_engine_demo.c.i
.PHONY : src/example/safety_engine_demo.c.i

src/example/safety_engine_demo.s: src/example/safety_engine_demo.c.s

.PHONY : src/example/safety_engine_demo.s

# target to generate assembly for a file
src/example/safety_engine_demo.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/example/safety_engine_demo.c.s
.PHONY : src/example/safety_engine_demo.c.s

src/example/vehicle_daq_demo.o: src/example/vehicle_daq_demo.c.o

.PHONY : src/example/vehicle_daq_demo.o

# target to build an object file
src/example/vehicle_daq_demo.c.o:
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/example/vehicle_daq_demo.c.o
.PHONY : src/example/vehicle_daq_demo.c.o

src/example/vehicle_daq_demo.i: src/example/vehicle_daq_demo.c.i

.PHONY : src/example/vehicle_daq_demo.i

# target to preprocess a source file
src/example/vehicle_daq_demo.c.i:
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/example/vehicle_daq_demo.c.i
.PHONY : src/example/vehicle_daq_demo.c.i

src/example/vehicle_daq_demo.s: src/example/vehicle_daq_demo.c.s

.PHONY : src/example/vehicle_daq_demo.s

# target to generate assembly for a file
src/example/vehicle_daq_demo.c.s:
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/example/vehicle_daq_demo.c.s
.PHONY : src/example/vehicle_daq_demo.c.s

src/http_client/http_client_internal.o: src/http_client/http_client_internal.c.o

.PHONY : src/http_client/http_client_internal.o

# target to build an object file
src/http_client/http_client_internal.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_client/http_client_internal.c.o
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_client/http_client_internal.c.o
.PHONY : src/http_client/http_client_internal.c.o

src/http_client/http_client_internal.i: src/http_client/http_client_internal.c.i

.PHONY : src/http_client/http_client_internal.i

# target to preprocess a source file
src/http_client/http_client_internal.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_client/http_client_internal.c.i
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_client/http_client_internal.c.i
.PHONY : src/http_client/http_client_internal.c.i

src/http_client/http_client_internal.s: src/http_client/http_client_internal.c.s

.PHONY : src/http_client/http_client_internal.s

# target to generate assembly for a file
src/http_client/http_client_internal.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_client/http_client_internal.c.s
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_client/http_client_internal.c.s
.PHONY : src/http_client/http_client_internal.c.s

src/http_server/http_server_internal.o: src/http_server/http_server_internal.c.o

.PHONY : src/http_server/http_server_internal.o

# target to build an object file
src/http_server/http_server_internal.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_server/http_server_internal.c.o
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_server/http_server_internal.c.o
.PHONY : src/http_server/http_server_internal.c.o

src/http_server/http_server_internal.i: src/http_server/http_server_internal.c.i

.PHONY : src/http_server/http_server_internal.i

# target to preprocess a source file
src/http_server/http_server_internal.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_server/http_server_internal.c.i
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_server/http_server_internal.c.i
.PHONY : src/http_server/http_server_internal.c.i

src/http_server/http_server_internal.s: src/http_server/http_server_internal.c.s

.PHONY : src/http_server/http_server_internal.s

# target to generate assembly for a file
src/http_server/http_server_internal.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/http_server/http_server_internal.c.s
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/http_server/http_server_internal.c.s
.PHONY : src/http_server/http_server_internal.c.s

src/util/log.o: src/util/log.c.o

.PHONY : src/util/log.o

# target to build an object file
src/util/log.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/log.c.o
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/log.c.o
.PHONY : src/util/log.c.o

src/util/log.i: src/util/log.c.i

.PHONY : src/util/log.i

# target to preprocess a source file
src/util/log.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/log.c.i
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/log.c.i
.PHONY : src/util/log.c.i

src/util/log.s: src/util/log.c.s

.PHONY : src/util/log.s

# target to generate assembly for a file
src/util/log.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/log.c.s
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/log.c.s
.PHONY : src/util/log.c.s

src/util/pack_util.o: src/util/pack_util.c.o

.PHONY : src/util/pack_util.o

# target to build an object file
src/util/pack_util.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/pack_util.c.o
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/pack_util.c.o
.PHONY : src/util/pack_util.c.o

src/util/pack_util.i: src/util/pack_util.c.i

.PHONY : src/util/pack_util.i

# target to preprocess a source file
src/util/pack_util.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/pack_util.c.i
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/pack_util.c.i
.PHONY : src/util/pack_util.c.i

src/util/pack_util.s: src/util/pack_util.c.s

.PHONY : src/util/pack_util.s

# target to generate assembly for a file
src/util/pack_util.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/util/pack_util.c.s
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/util/pack_util.c.s
.PHONY : src/util/pack_util.c.s

src/vehicle_property_acq.o: src/vehicle_property_acq.c.o

.PHONY : src/vehicle_property_acq.o

# target to build an object file
src/vehicle_property_acq.c.o:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/vehicle_property_acq.c.o
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/vehicle_property_acq.c.o
.PHONY : src/vehicle_property_acq.c.o

src/vehicle_property_acq.i: src/vehicle_property_acq.c.i

.PHONY : src/vehicle_property_acq.i

# target to preprocess a source file
src/vehicle_property_acq.c.i:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/vehicle_property_acq.c.i
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/vehicle_property_acq.c.i
.PHONY : src/vehicle_property_acq.c.i

src/vehicle_property_acq.s: src/vehicle_property_acq.c.s

.PHONY : src/vehicle_property_acq.s

# target to generate assembly for a file
src/vehicle_property_acq.c.s:
	$(MAKE) -f CMakeFiles/safety_engine_app_demo.dir/build.make CMakeFiles/safety_engine_app_demo.dir/src/vehicle_property_acq.c.s
	$(MAKE) -f CMakeFiles/vehicle_daq_demo.dir/build.make CMakeFiles/vehicle_daq_demo.dir/src/vehicle_property_acq.c.s
.PHONY : src/vehicle_property_acq.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... safety_engine_app_demo"
	@echo "... vehicle_daq_demo"
	@echo "... src/example/safety_engine_demo.o"
	@echo "... src/example/safety_engine_demo.i"
	@echo "... src/example/safety_engine_demo.s"
	@echo "... src/example/vehicle_daq_demo.o"
	@echo "... src/example/vehicle_daq_demo.i"
	@echo "... src/example/vehicle_daq_demo.s"
	@echo "... src/http_client/http_client_internal.o"
	@echo "... src/http_client/http_client_internal.i"
	@echo "... src/http_client/http_client_internal.s"
	@echo "... src/http_server/http_server_internal.o"
	@echo "... src/http_server/http_server_internal.i"
	@echo "... src/http_server/http_server_internal.s"
	@echo "... src/util/log.o"
	@echo "... src/util/log.i"
	@echo "... src/util/log.s"
	@echo "... src/util/pack_util.o"
	@echo "... src/util/pack_util.i"
	@echo "... src/util/pack_util.s"
	@echo "... src/vehicle_property_acq.o"
	@echo "... src/vehicle_property_acq.i"
	@echo "... src/vehicle_property_acq.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
