#
# Compiler flags
#
#CFLAGS = -Wall -Werror -Wextra

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

SRC_LIST = $(addprefix $(SRC_PREFIX)/, $(SRCS))

#
# Debug build settings
#
DBGDIR = $(BUILD_PREFIX)/debug
DBGEXE = $(DBGDIR)/$(BIN_PREFIX)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = $(BUILD_PREFIX)/release
RELEXE = $(RELDIR)/$(BIN_PREFIX)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean prep debug release remake

# Default build
all: prep release

#
# Debug rules
#
debug: prep $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

$(DBGDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: prep $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

$(RELDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR)/$(BIN_PREFIX) $(RELDIR)/$(BIN_PREFIX)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
