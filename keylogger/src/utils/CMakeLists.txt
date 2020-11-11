add_library(
    utils
        STATIC
            utils.cpp
)

add_library(keylogger::utils ALIAS utils)

target_include_directories(
    utils
        PUBLIC
            $<BUILD_INTERFACE:${KEYLOGGER_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${KEYLOGGER_INSTALL_INCLUDE_DIR}>
)

set_target_properties(
    utils
        PROPERTIES
            OUTPUT_NAME "keylogger-utils"
)

install(
    DIRECTORY ${KEYLOGGER_INCLUDE_DIR}/keylogger/utils
    DESTINATION ${KEYLOGGER_INSTALL_INCLUDE_DIR}/keylogger
)

install(
    TARGETS utils
    EXPORT utils-targets
    ARCHIVE DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
)