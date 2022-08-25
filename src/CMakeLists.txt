set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_LOG_C} ${HEADERS_UTILITY})

set(TARGET bytesize)

# Bytesize bin
make_bin(
    NAME ${TARGET}
    SRCS bytesize.c main.c cli.c
    HDRS ${HEADERS}
    DEPS ${PKG_DEPENDS})

make_ssl(
    NAME ${TARGET}
    SRCS bytesize.c
    HDRS ${HEADERS}
    DEPS ${PKG_DEPENDS})

# Manual Installation
install(TARGETS ${BIN_NAME} DESTINATION bin COMPONENT bin)

install(TARGETS ${TARGET}
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)

install(TARGETS ${TARGET}_static
    DESTINATION lib
    PUBLIC_HEADER DESTINATION include
    COMPONENT lib)
