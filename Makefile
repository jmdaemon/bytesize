include make/os.mk
include make/structure.mk

#
# Project Version
# 
include make/version.mk

VERSION_MAJOR = 1
VERSION_MINOR = 5
VERSION_PATCH = 0

#
# Compiler flags
#
# Common compiler flags to every target go here

GLOBAL_CFLAGS = -Wall -Wextra -DLOG_USE_COLOR -Wno-missing-braces -Wno-discarded-qualifiers
GLOBAL_LDFLAGS = -lpcre -lm -lgmp -lmpfr

# Enable ANSI escape code colored logging statements
LOGC_FLAGS = -DLOG_USE_COLOR

# Include headers
INCLUDES = -I. -I$(PATHI) -I$(PATHD)


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

install-lib-headers:
	install $(PATHI)/bytesize.h $(DESTDIR)$(PREFIX)/include/bytesize.h
	install $(SUB_LOG_C_SRC)/log.h $(DESTDIR)$(PREFIX)/include/log.h

uninstall-lib-headers:
	$(CLEANUP) $(DESTDIR)$(PREFIX)/include/bytesize.h
	$(CLEANUP) $(DESTDIR)$(PREFIX)/include/log.h

# Build as a binary
include make/binary.mk

# Build documentation
include make/docs.mk

#
# Other rules
#

clean: clean-test clean-subprojects clean-objs clean-bin clean-lib clean-ver clean-docs
clean-subprojects: clean-logc
