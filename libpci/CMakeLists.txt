cmake_minimum_required(VERSION 3.10)
project(pci VERSION 1.0.0 LANGUAGES C)
include(GNUInstallDirs)

add_library(${PROJECT_NAME} STATIC src/libpci.c)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE tests)
target_include_directories(${PROJECT_NAME} PUBLIC include)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION ${PROJECT_VERSION}
        PUBLIC_HEADER include/libpci.h
        SOVERSION 1)

install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(tests)