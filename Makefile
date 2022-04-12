include make/os.mk

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

SUBPROJECTS = subprojects

#
# Subprojects
#

#
# Compiler flags
#
# Common compiler flags to every target go here
GLOBAL_CFLAGS = -Wall -Wextra
GLOBAL_LDFLAGS = -lpcre -lm

# Library compiler flags
LIB_CFLAGS = -fPIC
LIB_LDFLAGS = -shared

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

#.PHONY: all debug release test lib bin clean clean-bin clean-test remake
#.PHONY: all subprojects logc debug release test lib bin clean clean-bin clean-test remake
.PHONY: all subprojects logc test lib bin clean clean-bin clean-test remake

# Install the binary
install-bin: release $(EXE)
	install $(EXE) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

uninstall-bin: release $(EXE)
	$(CLEANUP) $(DESTDIR)$(PREFIX)/bin/$(BINARY_NAME)

# Install the library
#install-lib: $(LIB)
	#install $(LIB) $(DESTDIR)$(PREFIX)/lib/$(LIBRARY_NAME)

#uninstall-lib: release $(LIB)
	#$(CLEANUP) $(DESTDIR)$(PREFIX)/lib/$(LIBRARY_NAME)

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

remake: clean all

clean: clean-test clean-subprojects clean-objs clean-bin clean-lib

clean-subprojects:
	@echo "Removing subprojects output"
	$(CLEANUP) $(SP_LOGC_OBJS)
	
clean-objs:
	@echo "Removing build object output"
	$(CLEANUP) $(PATHB)/debug/*.o $(PATHB)/release/*.o
