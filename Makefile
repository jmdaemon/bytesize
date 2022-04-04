#
# Compiler flags
#
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -lpcre -lm

#
# Prefixes
#
BUILD_PREFIX = build
SRC_PREFIX = src
BIN_PREFIX = bin
# These are used to generate the build structure
# build/{debug,release}

#
# Project files
#
# Builds bytesize as an executable binary
#
# Note: $(SRCS:.c=.o) replaces all *.c sources with *.o extensions
SRCS = cli.c bytesize.c
OBJS = $(SRCS:.c=.o)
EXE  = bytesize

#
# Library
#
# Builds bytesize as a library
#
# Note: that we cannot reuse the same sources as the 
# binary target, since GNU Argp cannot be compiled
# as a shared library.
#CFLAGS_LIB = -fPIC -g
CFLAGS_LIB = -fPIC
LDFLAGS_LIB = -shared
LIB_SRCS = bytesize.c
LIB_OBJS = $(SRCS:.c=.o)
LIB = libbytesize.so
LIB_PREFIX = lib

.PHONY: all clean prep debug release lib remake

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
TARGET_FLAGS = $(LDFLAGS) $(CFLAGS_LIB) $(LDFLAGS_LIB) 
BUILD_LIB = $(BUILD_DIR)/$(LIB_PREFIX)/$(LIB)
BUILD_LIB_OBJS = $(addprefix $(BUILD_DIR)/, $(LIB_OBJS))
endif


# Executable settings
# BUILD_DIR: 	The directory of the target.
# BUILD_EXEC: The output directory of the binary target
# BUILD_OBJS: The object files of the binary target

BUILD_DIR = $(BUILD_PREFIX)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(BIN_PREFIX)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS))

# Rules

## Install/Uninstall
install: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall: release $(BUILD_EXEC)
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)

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

# Compile all object targets
$(BUILD_DIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<

#
# Other rules
#

# prep, prep-lirary: Creates the directories for the bin and lib targets

# Creates build/$(BIN_PREFIX)/lib
prep-library:
	@mkdir -p $(BUILD_DIR)/$(LIB_PREFIX)

# Creates build/$(BIN_PREFIX)
prep:
	@mkdir -p $(BUILD_DIR)/$(BIN_PREFIX)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
