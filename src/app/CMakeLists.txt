cmake_minimum_required(VERSION 3.19.2)

# Version Info
set(LIVERMORE_QT_VERSION_MAJOR 0)
set(LIVERMORE_QT_VERSION_MINOR 0)
set(LIVERMORE_QT_VERSION_PATCH 1)
set(LIVERMORE_QT_VERSION ${LIVERMORE_QT_VERSION_MAJOR}.${LIVERMORE_QT_VERSION_MINOR}.${LIVERMORE_QT_VERSION_PATCH})

# Project Info
set(PACKAGE_NAME      "livermore-qt")
set(PACKAGE_VERSION   "${LIVERMORE_QT_VERSION}-dev")
set(PACKAGE_TARNAME   "${PACKAGE_NAME}-${PACKAGE_VERSION}")
set(PACKAGE_BUGREPORT "https://github.com/hanjingo/livermore-qt/issues")

project(livermore-qt LANGUAGES CXX)

add_definitions(-DVERSION_MAJOR="${LIVERMORE_QT_VERSION_MAJOR}")
add_definitions(-DVERSION_MINOR="${LIVERMORE_QT_VERSION_MINOR}")
add_definitions(-DVERSION_PATCH="${LIVERMORE_QT_VERSION_PATCH}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        autogrid.h
        autogrid.cpp
        kvolumegrid.h
        kvolumegrid.cpp
        datafile.h
        datafile.cpp
        config.h
        config.cpp
        db.h
        db.cpp
)

set(3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../3rd)
set(COMM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../comm)

include_directories(${3RD_DIR}/libcpp)
include_directories(${COMM_DIR})

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
    spdlog::spdlog
)
