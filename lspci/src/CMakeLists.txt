set(CMAKE_C_STANDARD 11)

add_library(lspci STATIC lspci.h lspci.c)

target_include_directories(lspci PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})