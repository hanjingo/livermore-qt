cmake_minimum_required(VERSION 3.19.2)

add_subdirectory(spdlog)

add_subdirectory(libqt)

add_subdirectory(libcpp)

# # config 3rd/breakpad
# set(BREAKPAD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/breakpad)
# if(NOT ${BREAKPAD_ROOT})
#     execute_process(cd ${BREAKPAD_ROOT} && pwd && ./configure --prefix=${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#     execute_process(cd ${BREAKPAD_ROOT} && make)
#     execute_process(cd ${BREAKPAD_ROOT} && make install)
# endif()
