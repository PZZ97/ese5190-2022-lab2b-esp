﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

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

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\build

# Utility rule file for flashlight_ws2812_pio_h.

# Include any custom commands dependencies for this target.
include CMakeFiles\flashlight_ws2812_pio_h.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\flashlight_ws2812_pio_h.dir\progress.make

CMakeFiles\flashlight_ws2812_pio_h: C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\generated\ws2812.pio.h

C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\generated\ws2812.pio.h: C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\ws2812.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/generated/ws2812.pio.h"
	pioasm\pioasm.exe -o c-sdk C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/ws2812.pio C:/Users/PP_ju/Documents/GitHub/ese5190-2022-lab2b-esp/lab/02_repl/REPL/generated/ws2812.pio.h

flashlight_ws2812_pio_h: CMakeFiles\flashlight_ws2812_pio_h
flashlight_ws2812_pio_h: C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\generated\ws2812.pio.h
flashlight_ws2812_pio_h: CMakeFiles\flashlight_ws2812_pio_h.dir\build.make
.PHONY : flashlight_ws2812_pio_h

# Rule to build all files generated by this target.
CMakeFiles\flashlight_ws2812_pio_h.dir\build: flashlight_ws2812_pio_h
.PHONY : CMakeFiles\flashlight_ws2812_pio_h.dir\build

CMakeFiles\flashlight_ws2812_pio_h.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\flashlight_ws2812_pio_h.dir\cmake_clean.cmake
.PHONY : CMakeFiles\flashlight_ws2812_pio_h.dir\clean

CMakeFiles\flashlight_ws2812_pio_h.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\build C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\build C:\Users\PP_ju\Documents\GitHub\ese5190-2022-lab2b-esp\lab\02_repl\REPL\build\CMakeFiles\flashlight_ws2812_pio_h.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\flashlight_ws2812_pio_h.dir\depend

