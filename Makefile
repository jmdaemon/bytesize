#
# Compiler flags
#
CFLAGS = -Wall -Wextra

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
SRCS = bytesize.c
OBJS = $(SRCS:.c=.o)
EXE  = bytesize

.PHONY: all clean prep debug release remake

# Default build
all: prep release

# Debug build settings
ifeq ($(filter debug,$(MAKECMDGOALS)),debug)
TARGET=debug
TARGET_FLAGS= -g -O0 -DDEBUG
endif

# Release build settings
ifeq ($(filter release,$(MAKECMDGOALS)),release)
TARGET=release
TARGET_FLAGS= -O3 -DNDEBUG
endif

BUILD_DIR = $(BUILD_PREFIX)/$(TARGET)
BUILD_EXEC= $(BUILD_DIR)/$(BIN_PREFIX)/$(EXE)
BUILD_OBJS= $(addprefix $(BUILD_DIR)/, $(OBJS))

# Rules

debug release: prep $(BUILD_EXEC)

$(BUILD_EXEC): $(BUILD_OBJS)
	$(CC) $(CFLAGS) $(TARGET_FLAGS) -o $(BUILD_EXEC) $^

$(BUILD_DIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(TARGET_FLAGS) -o $@ $<
#
# Other rules
#
prep:
	@mkdir -p $(BUILD_DIR)/$(BIN_PREFIX)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
