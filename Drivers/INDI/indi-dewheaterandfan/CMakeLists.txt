cmake_minimum_required(VERSION 3.28) #Ubuntu 24 CMake version
PROJECT(indi_dewheaterandfan CXX C)

# C++17 needed
set(CMAKE_CXX_STANDARD 17)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
#LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_modules/")

include(GNUInstallDirs)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

find_package(INDI REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.xml)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})

include(CMakeCommon)

set(${PROJECT_NAME}_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/StringParse.cpp
   )

IF (UNITY_BUILD)
    ENABLE_UNITY_BUILD(${PROJECT_NAME} ${PROJECT_NAME}_SRCS 6 cpp)
ENDIF ()

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})
target_link_libraries(${PROJECT_NAME} ${INDI_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.xml DESTINATION ${INDI_DATA_DIR})

