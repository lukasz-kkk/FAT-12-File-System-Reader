# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FAT_12_FILE_READER.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/FAT_12_FILE_READER.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FAT_12_FILE_READER.dir/flags.make

CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj: CMakeFiles/FAT_12_FILE_READER.dir/flags.make
CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj: CMakeFiles/FAT_12_FILE_READER.dir/includes_C.rsp
CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj: ../file_reader.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\FAT_12_FILE_READER.dir\file_reader.c.obj -c C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\file_reader.c

CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.i"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\file_reader.c > CMakeFiles\FAT_12_FILE_READER.dir\file_reader.c.i

CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.s"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\file_reader.c -o CMakeFiles\FAT_12_FILE_READER.dir\file_reader.c.s

CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj: CMakeFiles/FAT_12_FILE_READER.dir/flags.make
CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj: CMakeFiles/FAT_12_FILE_READER.dir/includes_C.rsp
CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\FAT_12_FILE_READER.dir\main.c.obj -c C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\main.c

CMakeFiles/FAT_12_FILE_READER.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/FAT_12_FILE_READER.dir/main.c.i"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\main.c > CMakeFiles\FAT_12_FILE_READER.dir\main.c.i

CMakeFiles/FAT_12_FILE_READER.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/FAT_12_FILE_READER.dir/main.c.s"
	D:\mingw\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\main.c -o CMakeFiles\FAT_12_FILE_READER.dir\main.c.s

# Object files for target FAT_12_FILE_READER
FAT_12_FILE_READER_OBJECTS = \
"CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj" \
"CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj"

# External object files for target FAT_12_FILE_READER
FAT_12_FILE_READER_EXTERNAL_OBJECTS =

FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/file_reader.c.obj
FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/main.c.obj
FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/build.make
FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/linklibs.rsp
FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/objects1.rsp
FAT_12_FILE_READER.exe: CMakeFiles/FAT_12_FILE_READER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable FAT_12_FILE_READER.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\FAT_12_FILE_READER.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FAT_12_FILE_READER.dir/build: FAT_12_FILE_READER.exe
.PHONY : CMakeFiles/FAT_12_FILE_READER.dir/build

CMakeFiles/FAT_12_FILE_READER.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\FAT_12_FILE_READER.dir\cmake_clean.cmake
.PHONY : CMakeFiles/FAT_12_FILE_READER.dir/clean

CMakeFiles/FAT_12_FILE_READER.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug C:\Users\lukal\OneDrive\Pulpit\FAT-12-FILE-READER\cmake-build-debug\CMakeFiles\FAT_12_FILE_READER.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FAT_12_FILE_READER.dir/depend

