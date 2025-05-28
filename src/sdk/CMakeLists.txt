cmake_minimum_required(VERSION 3.19.2)

project(livermore-sdk LANGUAGES CXX)

# Version Info
set(LIVERMORE_SDK_VERSION_MAJOR 1)
set(LIVERMORE_SDK_VERSION_MINOR 0)
set(LIVERMORE_SDK_VERSION_PATCH 1)
set(LIVERMORE_SDK_VERSION ${LIVERMORE_SDK_VERSION_MAJOR}.${LIVERMORE_SDK_VERSION_MINOR}.${LIVERMORE_SDK_VERSION_PATCH})

# Project Info
set(PACKAGE_NAME      "${PROJECT_NAME}")
set(PACKAGE_VERSION   "${LIVERMORE_SDK_VERSION}-dev")
set(PACKAGE_TARNAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "https://github.com/hanjingo/livermore-qt/issues")

add_definitions(-DVERSION_MAJOR="${LIVERMORE_QT_VERSION_MAJOR}")
add_definitions(-DVERSION_MINOR="${LIVERMORE_QT_VERSION_MINOR}")
add_definitions(-DVERSION_PATCH="${LIVERMORE_QT_VERSION_PATCH}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## utf-8 enable
#add_compile_options(
#    /source-charset:utf-8 
#    /execution-charset:utf-8)

# # enable to create pdb file in RELEASE mode
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
# set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

add_definitions(-DUSE_DEEP_DATA=true) # Use Deep Market Data

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rd)
set(LIBQT ${3RD_DIR}/libqt)

# enable debug info in release/debug model
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
# disable optimize in release/debug model
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0") # prefer to -O2
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O0") # prefer to -O2
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0")
if(MSVC)
    # generate PDB file
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Zi")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
endif()

# utf-8 enable
add_compile_options(
    /source-charset:utf-8 
    /execution-charset:utf-8)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Concurrent)

set(PROJECT_SOURCES
    broker.h
    broker.cpp
    handler.h
    handler.cpp
    livermoresdk.h
    livermoresdk.cpp
    market_data.h
    proto.h
    util.h
    util.cpp
    ${LIBQT}/libqt/net/tcp/tcpclient.cpp
    ${LIBQT}/libqt/net/nethandler.cpp
    ${LIBQT}/libqt/net/codec.cpp
)

include_directories(${LIBQT})
include_directories(${LIBQT}/3rd)

if(MSVC)
    set(PACKAGE_NAME "lib${PROJECT_NAME}")
else()
    set(PACKAGE_NAME "${PROJECT_NAME}")
endif()

add_library(${PACKAGE_NAME} SHARED
    ${PROJECT_SOURCES}
)

target_link_libraries(${PACKAGE_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
)

target_compile_definitions(${PACKAGE_NAME} PRIVATE ${PROJECT_NAME}_LIBRARY)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/livermoresdk.h DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE)