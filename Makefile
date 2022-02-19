#
# Compiler flags
#
#CC     = gcc
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
#OBJS = $(notdir $(SRCS:.c=.o))
EXE  = bytesize

SRC_LIST = $(addprefix $(SRC_PREFIX)/, $(SRCS))

# bytesize.c bytesize.o bytesize

#
# Debug build settings
#
# src/bytesize.c build/debug/bytesize.o build/debug/bytesize
DBGDIR = $(BUILD_PREFIX)/debug
#DBGEXE = $(DBGDIR)/$(EXE)
#DBGEXE = $(BUILD_PREFIX)/$(DBGDIR)/$(EXE)
#DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
#DBGOBJS = $(addprefix $(BUILD_PREFIX)/, $(addprefix $(DBGDIR)/, $(OBJS)))
#DBGOBJS = $(BUILD_PREFIX)/$(DBGDIR)/bytesize.o
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0 -DDEBUG

#
# Release build settings
#
RELDIR = $(BUILD_PREFIX)/release
#RELEXE = $(RELDIR)/$(EXE)
#RELEXE = $(BUILD_PREFIX)/$(RELDIR)/$(EXE)
#RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
#RELOBJS = $(addprefix $(BUILD_PREFIX)/, $(addprefix $(RELDIR)/, $(OBJS)))
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

#.PHONY: all clean debug prep release remake
.PHONY: all clean prep debug release remake

# Default build
#all: prep release
all: prep

#
# Debug rules
#
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(CFLAGS) $(DBGCFLAGS) -o $(DBGEXE) $^

#$(DBGDIR)/%.o: %.c
#$(DBGDIR)/%.o: $(SRC_PREFIX)/%.c
#$(BUILD_PREFIX)/$(DBGDIR)/$(OBJS)/%.o: $(SRC_PREFIX)/%.c
#$(DBGDIR)/%.o: %.c
#$(DBGOBJS): $(SRC_PREFIX)/$(SRCS)
#$(DBGOBJS): $(SRCS)
#$(DBGDIR)/%.o: $(SRC_PREFIX)/$(SRCS)
#$(DBGOBJS): $(SRC_LIST)

#$(DBGDIR)/%.o: $(SRC_LIST)
$(DBGDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(DBGCFLAGS) -o $@ $<

#
# Release rules
#
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) $(CFLAGS) $(RELCFLAGS) -o $(RELEXE) $^

#$(RELDIR)/%.o: %.c
#$(RELDIR)/%.o: $(SRC_LIST)
$(RELDIR)/%.o: $(SRC_PREFIX)/%.c
	$(CC) -c $(CFLAGS) $(RELCFLAGS) -o $@ $<

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR)
	#@mkdir -p $(BUILD_PREFIX)/$(DBGDIR) $(BUILD_PREFIX)/$(RELDIR)
	#@mkdir -p $(BUILD_PREFIX)/$(DBGDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
