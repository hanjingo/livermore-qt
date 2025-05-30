cmake_minimum_required(VERSION 3.19.2)

# translate ts file
set(TS_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(TS_FILES
    "${TS_DIR}/livermore-qt_zh_CN.ts"
)

find_program(LUPDATE_EXECUTABLE lupdate)
find_program(LRELEASE_EXECUTABLE lrelease)

foreach(_chinese_file ${TS_FILES})
    execute_process(
        COMMAND ${LUPDATE_EXECUTABLE} -recursive ${CMAKE_SOURCE_DIR} -ts ${_chinese_file})
    execute_process(
        COMMAND ${LRELEASE_EXECUTABLE} ${_chinese_file})
endforeach()
# translate end

#sqlite3.exe .\DataFiles\AppControlDB.db < .\AppControlDB.sql
#execute_process(COMMAND ./sqlite3 livermore-qt.db < livermore-qt.sql)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/conf.ini DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/livermore-qt_zh_CN.qm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/livermore-qt.db DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
     FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ WORLD_WRITE)