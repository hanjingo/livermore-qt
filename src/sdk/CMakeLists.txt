cmake_minimum_required(VERSION 3.19.2)

project(livermore-sdk LANGUAGES CXX)

# Version Info
set(LIVERMORE_SDK_VERSION_MAJOR 0)
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

# # enable to create pdf file in RELEASE mode
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
# set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")

add_definitions(-DUSE_DEEP_DATA=true) # Use Deep Market Data

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rd)
set(LIBCPP ${3RD_DIR}/libcpp)
set(LIBQT ${3RD_DIR}/libqt)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Concurrent)

set(PROJECT_SOURCES
    main.cpp
    livermoresdk.h
    proto.h
    market_data.h
    handler.h
    handler.cpp
    broker.h
    broker.cpp
    util.h
    util.cpp
    ${LIBQT}/libqt/net/tcp/tcpclient.cpp
    ${LIBQT}/libqt/net/nethandler.cpp
    ${LIBQT}/libqt/net/codec.cpp
)

include_directories(${LIBCPP})
include_directories(${LIBQT})

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)
# add_library(${PROJECT_NAME} SHARED
#     ${PROJECT_SOURCES}
# )

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    spdlog::spdlog
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_LIBRARY)
