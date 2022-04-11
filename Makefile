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
LIB_CFLAGS = -fPIC
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
INCLUDES = include

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

BUILD_PATHS = $(PATHB) $(PATHD) $(PATHO) $(PATHR)

SRCT = $(wildcard $(PATHT)/*.c)

# Unit Test Compiler Flags:
# -MM : Output single header dependencies for the compile files
# -MG : Run without being able to run into headers gcc can't find
# -MF : Write header dependencies to a file
TEST_COMPILE=gcc -c
TEST_LINK=gcc
TEST_DEPEND=gcc -MM -MG -MF
TEST_CFLAGS= $(GLOBAL_CFLAGS) -I. -I$(PATHU) -I$(PATHS) -I$(INCLUDES) -DTEST

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
	$(TEST_LINK) $(TEST_CFLAGS) -o $@ $^

# Compile unity sources
$(PATHO)/%.o:: $(PATHU)/%.c $(PATHU)/%.h
	$(TEST_COMPILE) $(TEST_CFLAGS) $< -o $@

# Compile files in src directory
$(PATHO)/%.o:: $(PATHS)/%.c
	$(TEST_COMPILE) $(TEST_CFLAGS) $(GLOBAL_LDFLAGS) $< -o $@

# Compile files in test directory
$(PATHO)/%.o:: $(PATHT)/%.c
	$(TEST_COMPILE) $(TEST_CFLAGS) $(GLOBAL_LDFLAGS) $< -o $@

# Create a depends directory
$(PATHD)/%.d:: $(PATHT)/%.c
	$(TEST_DEPEND) $@ $<

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
# Binary Sources
#
# Build the project as an executable binary

BINARY_SRCS = cli.c bytesize.c main.c
BINARY_OBJS = $(BINARY_SRCS:.c=.o)
#EXE  				= bytesize
BINARY_NAME = bytesize

#
# Library Sources
#
# Build the project as a library

LIBRARY_SRCS = bytesize.c
LIBRARY_OBJS = $(SRCS:.c=.o)
LIBRARY_NAME = libbytesize.so

# Note: We can't reuse the same sources as the 
# binary target, since GNU Argp cannot be compiled
# as a shared library.

# Set installation directory
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

#
# Build settings
#
# Toggle between release and debug configurations

# Release build settings
TARGET:=release
TARGET_FLAGS:= -O3 -DNDEBUG

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = -g -O0 -DDEBUG
endif

# Debug or Release target directory
TARGET_DIR = $(PATHB)/$(TARGET)

# Library settings
# TARGET_FLAGS: 	The library flags to build the library
# BUILD_LIB: 			The directory of the target library
# BUILD_LIB_OBJS: The object files of the library target

#TARGET_FLAGS = $(GLOBAL_LDFLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) 
#BUILD_LIB_FLAGS = $(GLOBAL_LDFLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) 
##BUILD_LIB = $(PATHB)/$(PREFIX_LIB)/$(LIB)
#BUILD_LIB = $(TARGET_DIR)/$(PREFIX_LIB)/$(LIB)
#BUILD_LIB_OBJS = $(addprefix $(PATHB)/, $(LIBRARY_OBJS))

#LIBRARY_FLAGS = $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) 
#BUILD_LIB = $(PATHB)/$(PREFIX_LIB)/$(LIB)
#BUILD_LIB = $(TARGET_DIR)/$(PREFIX_LIB)/$(LIB)
#BUILD_LIB_OBJS = $(addprefix $(PATHB)/, $(LIBRARY_OBJS))

LIBRARY_DIR = $(TARGET_DIR)/$(PREFIX_LIB)
LIB_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) 
LIB_SRCS 		= $(addprefix $(PATHS)/, $(LIBRARY_SRCS))
LIB_OBJS 		= $(addprefix $(TARGET_DIR)/, $(LIBRARY_OBJS))
LIB 				= $(LIBRARY_DIR)/$(LIBRARY_NAME)



# Executable settings
# BINARY_DIR: Target directory
# EXE_FLAGS : Target flags
# EXE_SRCS 	: Target sources
# EXE_OBJS 	: Target object files
# EXE 			: Target
BINARY_DIR 	= $(TARGET_DIR)/$(PREFIX_BIN)
EXE_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS)
EXE_SRCS 		= $(addprefix $(PATHS)/, $(BINARY_SRCS))
EXE_OBJS 		= $(addprefix $(TARGET_DIR)/, $(BINARY_OBJS))
EXE 				= $(BINARY_DIR)/$(BINARY_NAME)

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
#lib: prep-library $(BUILD_LIB)
#lib: $(BUILD_LIB)
lib: $(LIB)

# Compiles the shared library target and its object files
#$(BUILD_LIB): $(BUILD_LIB_OBJS)
	#$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $@ $^

# Compile the shared library target
$(LIB): $(LIB_OBJS)
	$(CC) $(LIB_FLAGS) -o $(LIB) $^

# Compile all $(LIB_OBJS) object files
$(LIB_OBJS):
	$(CC) -c $(LIB_FLAGS) -o $@ $<

#
# Debug/Release builds
#

#debug release: prep $(EXE)
debug release: $(EXE)

# Compile the executable binary target
$(EXE): $(EXE_OBJS)
	$(CC) $(EXE_FLAGS) -o $(EXE) $^

# Compile all $(EXE_OBJS) object files
$(EXE_OBJS): 
	$(CC) -c $(EXE_FLAGS) -o $@ $<

#
# Other rules
#

# prep, prep-library: Creates the directories for the bin and lib targets

# Creates build/$(PREFIX_LIB)
#prep-library:
	#@mkdir -p $(BUILD_DIR)/$(PREFIX_LIB)

# Create $(LIBRARY_DIR)
$(LIBRARY_DIR):
	$(MKDIR) $(LIBRARY_DIR)

# Create $(BINARY_DIR)
$(BINARY_DIR):
	$(MKDIR) $(BINARY_DIR)

remake: clean all

clean: clean-test clean-bin

# Remove output files for executables
clean-bin:
	$(CLEANUP) $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
