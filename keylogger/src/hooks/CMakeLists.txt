set(CMAKE_CXX_STANDARD 17)

add_library(
    hooks
        STATIC
        keyboard.cpp
        mouse.cpp
        events.cpp
)
add_library(keylogger::hooks ALIAS hooks)

target_include_directories(
    hooks
        PUBLIC
            $<BUILD_INTERFACE:${KEYLOGGER_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${KEYLOGGER_INSTALL_INCLUDE_DIR}>
)

set_target_properties(
    hooks
        PROPERTIES
            OUTPUT_NAME "keylogger-hooks"
)

install(
    DIRECTORY ${KEYLOGGER_INCLUDE_DIR}/keylogger/hooks
    DESTINATION ${KEYLOGGER_INSTALL_INCLUDE_DIR}/keylogger
)

install(
    TARGETS hooks
    EXPORT hooks-targets
    ARCHIVE DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
    LIBRARY DESTINATION ${KEYLOGGER_INSTALL_LIB_DIR}
)
