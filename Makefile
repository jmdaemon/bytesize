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
# Project Structure
#

# These are used to generate the build structure:
# - src
# - test
# - include
# - subprojects
# - build
# - build/{debug, release}
# - build/{debug, release}/lib/
# - build/{debug, release}/bin/

PATHS = src
PATHT = test
PATHB = build
PATHI = include

PREFIX_BIN = bin
PREFIX_LIB = lib

SUBPROJECTS = subprojects

#
# Subprojects
#

# Log.c

SUBPROJECT_LOG_C_NAME = log.c
SUBPROJECT_LOG_C_SRCS = log.c
SUBPROJECT_LOG_C_OBJS = $(SUBPROJECT_LOG_C_SRCS:.c=.o)

SUBPROJECTS_LOG_C_SRC = $(SUBPROJECTS)/$(SUBPROJECT_LOG_C_NAME)/src

# Subproject directory
#SUBPROJECTS_LOG_C_DIR = $(SUBPROJECTS)/$(LOG_C)
#SUBPROJECTS_LOG_C_SRC = $(LOG_C_DIR)/src
#SUBPROJECTS_LOG_C_SRC = $(SUBPROJECTS_LOG_C_DIR)/src

SUBPROJECTS_LOG_C_INCLUDES = -I$(LOG_C_SRC)
#SUBPROJECTS_LOG_C_CFLAGS = $(GLOBAL_CFLAGS) -I $(LOG_C_INCLUDES)
#SUBPROJECTS_LOG_C_LDFLAGS = $(GLOBAL_LDFLAGS)

#LOG_C_INCLUDES 	= $(LOG_C_DIR)/src
#LOG_C_SRC 			= $(LOG_C_DIR)/src/$(LOG_C)

#
# Compiler flags
#
# Common compiler flags to every target go here
GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = -lpcre -lm

# Library compiler flags
LIB_CFLAGS = -fPIC
LIB_LDFLAGS = -shared

# Release / Debug compiler flags
REL_CFLAGS = -O3 -DNDEBUG
DBG_CFLAGS = -g -O0 -DDEBUG 

# Include headers
INCLUDE_DIRS = -I. -I$(PATHI) -I$(LOG_C_INCLUDES)
INCLUDES = $(INCLUDE_DIRS)

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
TEST_CFLAGS= $(GLOBAL_CFLAGS) $(INCLUDES) -I$(PATHU) -I$(PATHS) -DTEST

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
TARGET := release
TARGET_FLAGS := $(REL_CFLAGS)

# Release settings
ifeq ($(filter release,$(MAKECMDGOALS)),release)
TARGET = release
TARGET_FLAGS = $(REL_CFLAGS)
endif

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = $(DBG_CFLAGS)
endif

# Debug or Release target directory
TARGET_DIR = $(PATHB)/$(TARGET)

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

#.PHONY: all debug release test lib bin clean clean-bin clean-test remake
.PHONY: all subprojects logc debug release test lib bin clean clean-bin clean-test remake

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

#
# Subprojects
#
# Every subproject that must be compiled alongside the main project will be
# given the SUB prefix.


# Subprojects that must be built
subprojects: logc


#
# Log.c Dependency
#

# Output files to build/depends/log.c
#SUB_LOGC_DIR = $(PATHD)/$(LOG_C)
#SUB_LOGC_SRCS = $(addprefix $(LOG_C_SRC)/, $(BINARY_SRCS))
#SUB_LOGC_SRCS = $(addprefix $(LOG_C_SRC)/, $(SUBPROJECT_LOG_C_SRCS))

SUB_LOGC_DIR = $(PATHD)/$(SUBPROJECT_LOG_C_NAME)
SUB_LOGC_SRCS = $(addprefix $(SUBPROJECTS_LOG_C_SRC)/, $(SUBPROJECT_LOG_C_SRCS))
SUB_LOGC_OBJS = $(addprefix $(SUB_LOGC_DIR)/, $(SUBPROJECT_LOG_C_OBJS))

# Create the logc output file
#logc: $(SUB_LOGC_DIR) $(SUB_LOGC_DIR)/%.o

#logc: $(SUB_LOGC_DIR) $(SUB_LOGC)
logc: $(SUB_LOGC_DIR) $(SUB_LOGC_OBJS)

#$(SUB_LOGC): $(SUB_LOGC_DIR)/%.o
	#$(CC) -c $(LOG_C_CFLAGS) $(LOG_C_LDFLAGS) -o $@ $^

# Compile build/depends/log.c/log.o
#$(SUB_LOGC_DIR)/%.o: $(LOG_C_SRC)/%.c $(LOG_C_SRC)/%.h

#$(SUB_LOGC_DIR)/%.o: $(LOG_C_SRC)/%.c
#$(SUB_LOGC_OBJS): $(LOG_C_SRC)/%.c
#$(SUB_LOGC_OBJS): $(SUBPROJECTS_LOG_C_SRC)/%.c
$(SUB_LOGC_OBJS): $(SUB_LOGC_SRCS)
	$(CC) -c $(LOG_C_CFLAGS) -o $@ $^

# Make build/depends/log.c
$(SUB_LOGC_DIR):
	$(MKDIR) $(SUB_LOGC_DIR)

#SUB_LOGC_DIR = $(PATHD)/$(LOG_C)
#SUB_LOGC_SRCS = $(addprefix $(LOG_C_SRC)/, $(LOG_C_SRCS))
#logc: $(SUB_LOGC) $(SUB_LOGC)/%.o

#$(SUB_LOGC)/%.o: $(LOG_C_SRC)/log.c $(LOG_C_SRC)/log.h
	#$(CC) -c $(LOG_C_CFLAGS) -o $@ $^

# Compile build/depends/log.c/log.o
#$(SUB_LOGC_DIR)/%.o: $(LOG_C_SRC)/%.c $(LOG_C_SRC)/log.h
#$(SUB_LOGC_DIR)/%.o: $(LOG_C_SRC)/%.c $(LOG_C_SRC)/log.h

#$(SUB_LOGC_DIR)/%.o: $(SUB_LOGC_SRCS)
	#$(CC) -c $(LOG_C_CFLAGS) -I$(LOG_C_SRC) -o $@ $^

	#$(CC) -c $(LOG_C_CFLAGS) -o -I$(LOG_C_SRC) $@ $^



#$(LIB_DEPS)/$(LOG_C)/%.o:
#build/depends/$(LOG_C)/%.o:

# Log.c
# Lib
#$(LIB_DEPS)/$(LOG_C)/%.o: $(LOG_C_SRC)
	#$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(LIB_FLAGS) -o $@ $^
	


#LOG_C_SRCS = $(addprefix $(LOG_C_SRC)/, $(BINARY_OBJS))
#LOG_C_SRCS = $(addprefix $(LOG_C_SRC)/, $(_LOG_C_SRCS))


#SUB_LOGC = $(PATHD)/$(LOG_C)



# Build Log.c output files
#$(SUB_LOGC)/%.o: $(LOG_C_SRC)/%.c $(LOG_C_SRC)/%.h
#$(SUB_LOGC)/%.o: $(LOG_C_SRC)/log.c $(LOG_C_SRC)/log.h
#log.c: $(SUB_LOGC) $(LOG_C_SRCS)
#log.c: $(SUB_LOGC) $(SUB_LOGC_SRCS)

#log.c: $(SUB_LOGC) $(SUB_LOGC)/%.o
#logc: $(SUB_LOGC) $(SUB_LOGC)/%.o


	#$(CC) $(LOG_C_CFLAGS) $(TARGET_FLAGS) $(LIB_LDFLAGS) $(LIB_FLAGS) -o $@ $^

# Build both targets
build: lib bin

#
# Library builds
#

lib: subprojects $(LIBRARY_DIR) $(LIB_DEPS) $(LIB)

# Compile the shared library target
# Depends upon log.c
#$(LIB): $(LIB_DEPS)/%.o
#$(LIB): $(LIB_DEPS)/%.o $(PATHD)/$(LOG_C)/%.o
#$(LIB): $(LIB_DEPS)/%.o $(SUB_LOGC)/%.o
#$(LIB): $(LIB_DEPS)/%.o $(log.c)
$(LIB): $(LIB_DEPS)/%.o
	$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS) $(LIB_FLAGS) -o $@ $^

# Compile all library object files
# This target depends on the source files and the headers
$(LIB_DEPS)/%.o: $(LIB_SRCS) $(PATHI)/%.h
	$(CC) $(LIB_CFLAGS) -c $(EXE_FLAGS) -o $@ $<

# Create $(LIBRARY_DIR)
$(LIBRARY_DIR):
	$(MKDIR) $(LIBRARY_DIR)

$(LIB_DEPS):
	$(MKDIR) $(LIB_DEPS)

#
# Binary builds
#

bin: subprojects $(BINARY_DIR) $(EXE_DEPS) $(EXE)

# Compile the executable binary target
#$(EXE): $(EXE_OBJS)
#$(EXE): $(EXE_OBJS) $(PATHD)/$(LOG_C)/%.o
#$(EXE): $(EXE_OBJS) $(SUB_LOGC)/%.o
#$(EXE): $(EXE_OBJS) $(log.c)
$(EXE): $(EXE_OBJS)
	$(CC) $(EXE_FLAGS) -o $@ $^

# Compile all $(EXE_OBJS) object files
# This target depends on the source files and the headers
$(EXE_DEPS)/%.o: $(PATHS)/%.c $(PATHI)/%.h
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
