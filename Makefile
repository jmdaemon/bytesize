#
# Operating Systems
#
# Enable build system compatibility for other operating systems
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
endif

#
# Global Compiler flags
#
# Common compiler flags to every target go here
GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = -lpcre -lm

# Library compiler flags
LIB_CFLAGS = -fPIC -g
LIB_LDFLAGS = -shared

#
# Project Structure
#

# These are used to generate the build structure:
# - build
# - build/{debug, release}
# - build/{debug, release}/lib/
# - build/{debug, release}/bin/
# - build/{debug, release}/subprojects/

PATHS = src
PATHT = test
PATHB = build

PREFIX_BIN = bin
PREFIX_LIB = lib

#
# Unit Testing
#

# Unit Testing Directories
PATHU = subprojects/unity/src
PATHD = $(PATHB)/depends
PATHO = $(PATHB)/objs
PATHR = $(PATHB)/results

INCLUDES = include/

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)/*.c)

# Unit Test Compiler Flags:
# -MM : Output single header dependencies for the compile files
# -MG : Run without being able to run into headers gcc can't find
# -MF : Write header dependencies to a file
COMPILE=gcc -c
LINK=gcc
DEPEND=gcc -MM -MG -MF
CFLAGS= $(GLOBAL_CFLAGS) -I. -I$(PATHU) -I$(PATHS) -I$(INCLUDES) -DTEST

# Unit Tests Results
# Note: Our files will be named:
# [source].c, test_[source].c
# Due to these substitutions they must be named like this
# in order for the tests to work and compile
RESULTS = $(patsubst $(PATHT)/test_%.c,$(PATHR)/test_%.txt,$(SRCT) )
PASSED = `grep -s PASS $(PATHR)*.txt`
FAIL = `grep -s FAIL $(PATHR)*.txt`
IGNORE = `grep -s IGNORE $(PATHR)*.txt`

# Unit Test Rules

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

# Rules for finding source files in sub directories

# Create test results
$(PATHR)/%.txt: $(PATHB)/%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1

# Link unit tests with the unity test framework and our sources
$(PATHB)/test_%.$(TARGET_EXTENSION): $(PATHO)/test_%.o $(PATHO)/%.o $(PATHU)/unity.o
	$(LINK) $(CFLAGS) -o $@ $^

# Compile unity sources
$(PATHO)/%.o:: $(PATHU)/%.c $(PATHU)/%.h
	$(COMPILE) $(CFLAGS) $< -o $@

# Compile files in src directory
$(PATHO)/%.o:: $(PATHS)/%.c
	$(COMPILE) $(CFLAGS) $(LDFLAGS) $< -o $@

# Compile files in test directory
$(PATHO)/%.o:: $(PATHT)/%.c
	$(COMPILE) $(CFLAGS) $(LDFLAGS) $< -o $@

# Create a depends directory
$(PATHD)/%.d:: $(PATHT)/%.c
	$(DEPEND) $@ $<

#
# Unit test build paths
#

# Create build/
$(PATHB):
	$(MKDIR) $(PATHB)

# Create build/depends
$(PATHD):
	$(MKDIR) $(PATHD)

# Create build/objs
$(PATHO):
	$(MKDIR) $(PATHO)

# Create build/results
$(PATHR):
	$(MKDIR) $(PATHR)

# Remove output files for tests
clean-test:
	$(CLEANUP) $(PATHO)/*.o
	$(CLEANUP) $(PATHB)/*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATHR)/*.txt

# Keep test results & output
.PRECIOUS: $(PATHB)/test_%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATHD)/%.d
.PRECIOUS: $(PATHO)/%.o
.PRECIOUS: $(PATHR)/%.txt

#
# Binary
#
# Build the project as an executable binary

BINARY_SRCS = cli.c bytesize.c main.c
BINARY_OBJS = $(SRCS:.c=.o)
EXE  				= bytesize

#
# Library
#
# Build the project as a library

#CFLAGS_LIB = -fPIC -g
LIBRARY_SRCS = bytesize.c
LIBRARY_OBJS = $(SRCS:.c=.o)
LIB = libbytesize.so


#LIB_SRCS = bytesize.c
#LIB_OBJS = $(SRCS:.c=.o)
#LIB_PREFIX = lib

# Note: that we cannot reuse the same sources as the 
# binary target, since GNU Argp cannot be compiled
# as a shared library.


# Default build
all: prep release

# Set install
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

#
# Build settings
#

# Release build settings
TARGET:=release
TARGET_FLAGS:= -O3 -DNDEBUG $(LDFLAGS)

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = -g -O0 -DDEBUG $(LDFLAGS)
endif

# Library build settings
# TARGET_FLAGS: 	The library flags to build the library
# BUILD_LIB: 			The directory of the target library
# BUILD_LIB_OBJS: The object files of the library target
ifeq ($(filter lib,$(MAKECMDGOALS)),lib)
#TARGET_FLAGS = $(LDFLAGS) $(CFLAGS_LIB) $(LDFLAGS_LIB) 
TARGET_FLAGS = $(GLOBAL_LDFLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) 
BUILD_LIB = $(PATHB)/$(PREFIX_LIB)/$(LIB)
BUILD_LIB_OBJS = $(addprefix $(PATHB)/, $(LIBRARY_OBJS))
endif


# Executable settings
# BUILD_DIR: 	The directory of the target.
# BUILD_EXEC: The output directory of the binary target
# BUILD_OBJS: The object files of the binary target

BUILD_DIR = $(PATHB)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(PREFIX_BIN)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS))

#
# Rules
#

.PHONY: all clean clean-bin clean-test prep test debug release lib remake

## Default build
all: prep release



#
# Install / Uninstall
#

install: install-bin

# Install the binary
install-bin: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall-bin: release $(BUILD_EXEC)
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)

# Install the library
install-lib: $(BUILD_LIB)
	install $(BUILD_LIB) $(DESTDIR)$(PREFIX)/lib/$(LIB)

uninstall-lib: release $(BUILD_LIB)
	rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB)

#
# Library builds
#
lib: prep-library $(BUILD_LIB)

# Compiles the shared library target and its object files
$(BUILD_LIB): $(BUILD_LIB_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $@ $^

#
# Debug/Release builds
#

debug release: prep $(BUILD_EXEC)

# Compile the executable binary target and its object files
$(BUILD_EXEC): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_EXEC) $^

# Compile all object targets in $(BUILD_DIR)
$(BUILD_DIR)/%.o: $(PATHS)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<

#
# Other rules
#



# prep, prep-library: Creates the directories for the bin and lib targets

# Creates build/$(PREFIX_LIB)
prep-library:
	@mkdir -p $(BUILD_DIR)/$(PREFIX_LIB)

# Creates build/$(PREFIX_BIN)
prep:
	@mkdir -p $(BUILD_DIR)/$(PREFIX_BIN)

remake: clean all

clean: clean-test clean-bin

# Remove output files for executables
clean-bin:
	$(CLEANUP) $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
