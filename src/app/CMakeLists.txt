cmake_minimum_required(VERSION 3.19.2)

project(livermore-qt LANGUAGES CXX)

# Version Info
set(LIVERMORE_QT_VERSION_MAJOR 0)
set(LIVERMORE_QT_VERSION_MINOR 0)
set(LIVERMORE_QT_VERSION_PATCH 1)
set(LIVERMORE_QT_VERSION ${LIVERMORE_QT_VERSION_MAJOR}.${LIVERMORE_QT_VERSION_MINOR}.${LIVERMORE_QT_VERSION_PATCH})

# Project Info
set(PACKAGE_NAME      "${PROJECT_NAME}")
set(PACKAGE_VERSION   "${LIVERMORE_QT_VERSION}-dev")
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

add_definitions(-DUSE_DEEP_DATA=true) # Use Deep Market Data

set(3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rd)
set(LIBQT ${3RD_DIR}/libqt)
set(SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../sdk)

# utf-8 enable
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /utf-8")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /utf-8")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        handler.h
        handler.cpp
        kvolumegrid.h
        kvolumegrid.cpp
        config.h
        data.h
        data.cpp
        util.h
        util.cpp
        showdetail.h
        showdetail.cpp
        klinegrid.h
        klinegrid.cpp
        tmdivisionchart.h
        tmdivisionchart.cpp
        ${LIBQT}/libqt/db/dbconnpool.h
        ${LIBQT}/libqt/db/dbconnpool.cpp
        ${LIBQT}/libqt/ui/autogrid.h
        ${LIBQT}/libqt/ui/autogrid.cpp
        ${LIBQT}/libqt/log/logger.h
)

include_directories(${LIBQT})
include_directories(${LIBQT}/3rd)
include_directories(${SDK_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message("install to ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

    install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
    )

    if(WIN32)
        include(${LIBQT}/cmakes/deployqt.cmake)
        windeployqt(${PROJECT_NAME} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    endif()
endif()