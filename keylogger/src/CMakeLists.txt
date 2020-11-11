find_package(Boost 1.58 COMPONENTS log log_setup filesystem REQUIRED)

set(PROJECT_LIB ${PROJECT_LIB} ${Boost_LOG_LIBRARY} ${Boost_LOG_SETUP_LIBRARY})
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -fopenmp  -DBOOST_LOG_DYN_LINK")

add_subdirectory(hooks)
add_subdirectory(logger)
add_subdirectory(utils)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
        -Wl,--start-group ${PROJECT_LIB} -Wl,--end-group
        keylogger::logger
        keylogger::utils
        keylogger::hooks)