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

LIBRARY_SRCS = $(BINARY_SRCS)
LIBRARY_OBJS = $(LIBRARY_SRCS:.c=.o)
LIBRARY_NAME = libbytesize.$(SHARED_LIBRARY_EXT)

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
INCLUDES = $(addprefix -I,$(PATHI))

# Library settings
LIBRARY_DIR = $(TARGET_DIR)/$(PREFIX_LIB)
LIB_DEPS 		= $(TARGET_DIR)/_$(PREFIX_LIB)_deps
LIB_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES)
LIB_SRCS 		= $(addprefix $(PATHS)/, $(LIBRARY_SRCS))
LIB 				= $(LIBRARY_DIR)/$(LIBRARY_NAME)

# Executable settings
BINARY_DIR 	= $(TARGET_DIR)/$(PREFIX_BIN)
EXE_DEPS 		= $(TARGET_DIR)/_$(PREFIX_BIN)_deps
EXE_FLAGS 	= $(GLOBAL_CFLAGS) $(GLOBAL_LDFLAGS) $(TARGET_FLAGS) $(INCLUDES)
EXE_SRCS 		= $(addprefix $(PATHS)/, $(BINARY_SRCS))
EXE_OBJS 		= $(addprefix $(EXE_DEPS)/, $(BINARY_OBJS))
EXE 				= $(BINARY_DIR)/$(BINARY_NAME)

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
	install $(EXE) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

uninstall-bin: release $(EXE)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

# Install the library
install-lib: $(LIB)
	install $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIBRARY_NAME)

uninstall-lib: release $(LIB)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/lib/$(LIBRARY_NAME)

# Build both targets
build: lib bin

#
# Library builds
#

lib: $(LIBRARY_DIR) $(LIB_DEPS) $(LIB)

# Compile the shared library target
$(LIB): $(LIB_DEPS)/%.o
	$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(LIB_FLAGS) -o $@ $^

# Compile all library object files
$(LIB_DEPS)/%.o: $(LIB_SRCS)
	$(CC) $(LIB_CFLAGS) -c $(EXE_FLAGS) -o $@ $<

# Create $(LIBRARY_DIR)
$(LIBRARY_DIR):
	$(MKDIR) $(LIBRARY_DIR)

$(LIB_DEPS):
	$(MKDIR) $(LIB_DEPS)

#
# Binary builds
#

bin: $(BINARY_DIR) $(EXE_DEPS) $(EXE)

# Compile the executable binary target
$(EXE): $(EXE_OBJS)
	$(CC) $(EXE_FLAGS) -o $@ $^

# Compile all $(EXE_OBJS) object files
$(EXE_DEPS)/%.o: $(PATHS)/%.c
	$(CC) -c $(EXE_FLAGS) -o $@ $<

# Create $(BINARY_DIR)
$(BINARY_DIR):
	$(MKDIR) $(BINARY_DIR)

$(EXE_DEPS):
	$(MKDIR) $(EXE_DEPS)

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
clean-bin: clean-objs
	$(CLEANUP) $(PATHB)/debug/bin/$(BINARY_NAME) $(PATHB)/release/bin/$(BINARY_NAME)
	$(CLEANUP) $(PATHB)/debug/_$(PREFIX_BIN)_deps/*.o $(PATHB)/release/_$(PREFIX_BIN)_deps/*.o
