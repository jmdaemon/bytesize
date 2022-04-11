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
	TARGET_EXTENSION=exe
	SHARED_LIBRARY_EXT=dll
	SHARED_LIBRARY_EXT=lib
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=out
	SHARED_LIBRARY_EXT=so
	STATIC_LIBRARY_EXT=a
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
PATHI = include

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
BINARY_NAME = bytesize

#
# Library Sources
#
# Build the project as a library

LIBRARY_SRCS = bytesize.c
#LIBRARY_OBJS = $(SRCS:.c=.o)
LIBRARY_OBJS = $(LIBRARY_SRCS:.c=.o)
LIBRARY_NAME = libbytesize.$(SHARED_LIBRARY_EXT)

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

# Build in release mode by default
TARGET:=release
TARGET_FLAGS:= -O3 -DNDEBUG

# Release settings
ifeq ($(filter release,$(MAKECMDGOALS)),release)
TARGET = release
TARGET_FLAGS = -O3 -DNDEBUG
endif

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = -g -O0 -DDEBUG
endif

# Debug or Release target directory
TARGET_DIR = $(PATHB)/$(TARGET)

# Set includes 
#INCLUDES_DIRS = $(addprefix -I$(PATHI), $(INCLUDES))
INCLUDES = $(addprefix -I,$(PATHI))

# Set compiler
CC = gcc

# Library settings
# LIBRARY_DIR : Target directory
# LIB_FLAGS 	: Target specific flags
# LIB_SRCS 		: Target sources
# LIB_OBJS 		: Target object files
# LIB 				: Target
LIBRARY_DIR = $(TARGET_DIR)/$(PREFIX_LIB)
#LIB_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(INCLUDES_DIRS)
LIB_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(INCLUDES)
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
#EXE_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES_DIRS)
EXE_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES)
EXE_SRCS 		= $(addprefix $(PATHS)/, $(BINARY_SRCS))
EXE_OBJS 		= $(addprefix $(TARGET_DIR)/, $(BINARY_OBJS))
EXE 				= $(BINARY_DIR)/$(BINARY_NAME)

#EXE_DEPS = $(EXE_OBJS:.o=.d) # one dependency file for each source
#EXE_INCLUDES = -I$(PATHI) -I$(EXE_DEPS)

#
# Rules
#

.PHONY: all debug release test lib bin clean clean-bin clean-test remake

# Dummy rules
debug:

release:

#
# Install / Uninstall
#

# Install both targets
install: install-bin install-lib

# Uninstall both targets
uninstall:  uninstall-bin uninstall-lib

# Install the binary
install-bin: release $(EXE)
	install $(EXE) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall-bin: release $(EXE)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/bin/$(EXE)

# Install the library
install-lib: $(LIB)
	install $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIB)

uninstall-lib: release $(LIB)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/lib/$(LIB)

# Build both targets
build: lib bin

#
# Library builds
#

lib: $(LIBRARY_DIR) $(LIB)

# Compile the shared library target
$(LIB): $(LIB_OBJS)
	$(CC) $(LIB_FLAGS) -o $@ $^
	#$(CC) $(LIB_FLAGS) -o $(LIB) $@ $^

# Compile all $(LIB_OBJS) object files
#$(LIB_OBJS): $(LIB_SRCS)
#$(LIB_OBJS): $(PATHS)/%*.c
$(LIB_OBJS): $(LIB_SRCS)
	$(CC) -c $(LIB_FLAGS) -o $@ $<

# Create $(LIBRARY_DIR)
$(LIBRARY_DIR):
	$(MKDIR) $(LIBRARY_DIR)

#
# Binary builds
#

#bin: $(BINARY_DIR) $(EXE_OBJS) $(EXE)
bin: $(BINARY_DIR) $(EXE)

# Compile the executable binary target
#$(EXE): $(TARGET_DIR)/*.o
	#$(CC) $(EXE_FLAGS) -o $(EXE) $@ $^
#$(EXE): $(TARGET_DIR)/*.o

$(EXE): $(EXE_OBJS)
	$(CC) $(EXE_FLAGS) -o $@ $^

	#$(CC) $(EXE_FLAGS) -o $(EXE_OBJS)
	#$(CC) $(EXE_FLAGS) -o $@ $^
	#$(CC) $(EXE_FLAGS) -o $@ $^

# Compile all $(EXE_OBJS) object files
#$(EXE_OBJS): $(EXE_SRCS)
#$(EXE_OBJS): src/bytesize.c src/cli.c src/main.c
#$(EXE_OBJS): $(EXE_SRCS)

#$(TARGET_DIR)/%.o: $(EXE_SRCS)
	#$(CC) -c $(EXE_FLAGS) -o $@ $<

#$(TARGET_DIR)/%.o:
	#$(CC) -c $(EXE_FLAGS) -o $@ $<

#$(TARGET_DIR)/.c.o:
#$(EXE_OBJS): $(PATHS)/%.c

#$(TARGET_DIR)/%.d: $(PATHS)/%.c
		#$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

#$(EXE_OBJS): $(EXE_SRCS)
	#$(CC) $(EXE_INCLUDES) -c $(EXE_FLAGS) -o $(EXE_OBJS) $< -MMD
$(TARGET_DIR)/%.o: $(PATHS)/%.c
	$(CC) -c $(EXE_FLAGS) -o $@ $<
	#$(CC) $(EXE_INCLUDES) -c $(EXE_FLAGS) -o $(EXE_OBJS) $< -MMD


#$(CC) -c $(EXE_FLAGS) -o $@ $(@:.o=.c)

#$(CC) -c $(EXE_FLAGS) -o $@ $(@:.o=.c)

# Create $(BINARY_DIR)
$(BINARY_DIR):
	$(MKDIR) $(BINARY_DIR)

#
# Other rules
#

remake: clean all

clean: clean-test clean-objs clean-bin clean-lib
	
clean-objs:
	$(CLEANUP) $(PATHB)/debug/*.o $(PATHB)/release/*.o

# Remove output files for executables
clean-lib: clean-objs
	$(CLEANUP) $(PATHB)/debug/lib/$(LIBRARY_NAME) $(PATHB)/release/lib/$(LIBRARY_NAME)

# Remove output files for executables
	#$(CLEANUP) $(EXE_OBJS) $(EXE)
clean-bin: clean-objs
	$(CLEANUP) $(PATHB)/debug/bin/$(BINARY_NAME) $(PATHB)/release/bin/$(BINARY_NAME)
