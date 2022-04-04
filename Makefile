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
# build/{debug,release}/$(BIN_PREFIX)/

#
# Project files
#
SRCS = cli.c bytesize.c
OBJS = $(SRCS:.c=.o)
EXE  = bytesize

#
# Library
#
#CFLAGS_LIB = -fPIC -g
CFLAGS_LIB = -fPIC
LDFLAGS_LIB = -shared
LIB = libbytesize.so
LIB_PREFIX = lib

.PHONY: all clean prep debug release lib remake

# Default build
all: prep release

# Set install
ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif

# Release build settings
TARGET:=release
TARGET_FLAGS:= -O3 -DNDEBUG $(LDFLAGS)

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET = debug
TARGET_FLAGS = -g -O0 -DDEBUG $(LDFLAGS)
endif

# Library build settings
ifeq ($(filter lib,$(MAKECMDGOALS)),lib)
TARGET_FLAGS = $(LDFLAGS) $(CFLAGS_LIB) $(LDFLAGS_LIB) 
BUILD_LIB = $(BUILD_DIR)/$(LIB_PREFIX)/$(LIB)
endif

BUILD_DIR = $(BUILD_PREFIX)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(BIN_PREFIX)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS))

# Rules

## Install/Uninstall
install: release $(BUILD_EXEC)
	install $(BUILD_EXEC) $(DESTDIR)$(PREFIX)/bin/$(EXE)

uninstall: release $(BUILD_EXEC)
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)

# Library
lib: prep-library $(BUILD_LIB)

$(BUILD_LIB): $(BUILD_OBJS) 
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $@ $^

# Debug/Release builds
debug release: prep $(BUILD_EXEC)

$(BUILD_EXEC): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_EXEC) $^

$(BUILD_DIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<
#
# Other rules
#

prep-library:
	@mkdir -p $(BUILD_DIR)/$(LIB_PREFIX)

prep:
	@mkdir -p $(BUILD_DIR)/$(BIN_PREFIX)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
