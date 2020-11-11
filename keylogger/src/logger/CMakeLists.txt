add_library(
    logger
        STATIC
            mouse_logger.cpp
            keyboard_logger.cpp
)
add_library(keylogger::logger ALIAS logger)

target_include_directories(
    logger
        PUBLIC
            $<BUILD_INTERFACE:${KEYLOGGER_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${KEYLOGGER_INSTALL_INCLUDE_DIR}>
)

set_target_properties(
    logger
        PROPERTIES
            OUTPUT_NAME "keylogger-logger"
)

install(
    DIRECTORY ${KEYLOGGER_INCLUDE_DIR}/keylogger/logger
    DESTINATION ${KEYLOGGER_INSTALL_INCLUDE_DIR}/keylogger
)

install(
    TARGETS logger
    EXPORT logger-targets
    ARCHIVE DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
)