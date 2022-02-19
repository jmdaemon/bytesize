#
# Compiler flags
#
#CFLAGS = -Wall -Werror -Wextra

#
# Directories
#
BUILD_PREFIX = build
SRC_PREFIX = src

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
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = $(BUILD_PREFIX)/release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean prep debug release remake

# Default build
all: prep release

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

#$(DBGDIR)/%.o: $(SRC_LIST)
$(DBGDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

#$(RELDIR)/%.o: $(SRC_LIST)
$(RELDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
