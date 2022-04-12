include make/os.mk

include make/structure.mk

#
# Compiler flags
#
# Common compiler flags to every target go here
GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = -lpcre -lm

# Include headers
INCLUDES = -I. -I$(PATHI)

include make/unity.mk

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

include make/install.mk
include make/config.mk

#
# Rules
#

.PHONY: all subprojects clean remake

#
# Subprojects
#
# Build subprojects alongside our project

# Subprojects that must be built
subprojects: logc

# SP_DEPENDS : Object files to be included into lib, bin targets
# SP_INCLUDES: Header files to be included into lib,bin
SP_DEPENDS =
SP_INCLUDES =

include make/log.c.mk

# Build both targets
build: lib bin

# Build as a library
include make/library.mk

# Build as a binary
include make/binary.mk

#
# Other rules
#

remake: clean build

clean: clean-test clean-subprojects clean-objs clean-bin clean-lib

clean-subprojects: clean-logc
	
clean-objs:
	@echo "Removing build object output"
	$(CLEANUP) $(PATHB)/debug/*.o $(PATHB)/release/*.o
