include make/os.mk
include make/structure.mk

#
# Compiler flags
#
# Common compiler flags to every target go here

GLOBAL_CFLAGS = -Wall -Wextra -DLOG_USE_COLOR
GLOBAL_LDFLAGS = -lpcre -lm

# Enable ANSI escape code colored logging statements
LOGC_FLAGS = -DLOG_USE_COLOR

# Include headers
INCLUDES = -I. -I$(PATHI)

#
# Subprojects
#

include make/log.c.mk
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

.PHONY: all subprojects clean

#
# Subprojects
#
# Build subprojects alongside our project

# Subprojects that must be built
subprojects: logc

# SP_DEPENDS : Object files to be included into lib, bin targets
# SP_INCLUDES: Header files to be included into lib,bin
#SP_SOURCES =
#SP_DEPENDS =
#SP_INCLUDES =


# Build both targets
build: lib bin

# Build as a library
include make/library.mk

# Build as a binary
include make/binary.mk

#
# Other rules
#

clean: clean-test clean-subprojects clean-objs clean-bin clean-lib
clean-subprojects: clean-logc
