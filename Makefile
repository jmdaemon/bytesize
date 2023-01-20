.SECONDEXPANSION:

include make/os.mk
include make/structure.mk
include make/config.mk
include make/install.mk
include make/templates.mk

#
# Project Version
# 
include make/version.mk

VERSION_MAJOR = 1
VERSION_MINOR = 10
VERSION_PATCH = 0

#
# Compiler flags
#
# Common compiler flags to every target go here

GLOBAL_CFLAGS = -Wall -Wextra -Wno-missing-braces -Wno-discarded-qualifiers
#GLOBAL_LDFLAGS = -lpcre -lm -lgmp -lmpfr
GLOBAL_LDFLAGS = -lpcre -lm -lgmp -lmpfr -llogc -lutility

# Enable ANSI escape code colored logging statements
SP_FLAGS_logc = -DLOG_USE_COLOR

# Include headers
INCLUDES = -I. -I$(PATHI) -I$(PATHD)

#
# Subprojects
#
# SP_DEPENDS 	: Object files to be included into lib, bin targets
# SP_INCLUDES	: Header files to be included into lib,bin
# SP_NAMES 		: Subprojects to include

#SP_NAMES := utility
SP_NAMES :=

include make/subproject.mk
#include make/log.c.mk
include make/unity.mk
#include make/utility.mk

# Add optional dbgsp target to show subproject eval expansion info
ifeq ($(filter dbgsp,$(MAKECMDGOALS)),dbgsp)
$(info $(foreach subproject,$(SP_NAMES),$(call subproject_template,$(subproject))))
else
$(eval $(foreach subproject,$(SP_NAMES),$(call subproject_template,$(subproject))))
endif

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
LIBRARY_HDRS = bytesize.h

#
# Rules
#

.PHONY: build subprojects clean

# Subprojects that must be built
subprojects: $(SP_NAMES)

# Build both targets
build: lib bin

# Build as a library
include make/library.mk

# Build as a binary
include make/binary.mk

# Build documentation
include make/docs.mk

# Install/Uninstall rules
install-subprojects: $(INSTALL_SP_TARGET)
uninstall-subprojects: $(UNINSTALL_SP_TARGET)

# Clean specific output files
clean: $(CLEAN_TARGET)
clean-subprojects: $(CLEAN_SP_TARGET)
