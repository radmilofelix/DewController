cmake_minimum_required(VERSION 2.4.7)
PROJECT(indi-dewheaterandfan CXX C)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

include(GNUInstallDirs)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

find_package(INDI REQUIRED)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/indi_dewheaterandfan.xml.cmake ${CMAKE_CURRENT_BINARY_DIR}/indi_dewheaterandfan.xml)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INDI_INCLUDE_DIR})

include(CMakeCommon)

#set(GPIO_LIBRARIES "libgpiod.so")

################ ROR System ################
set(indi_dewheaterandfan_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/dewheaterandfan.cpp
   )

IF (UNITY_BUILD)
    ENABLE_UNITY_BUILD(indi_dewheaterandfan indi_dewheaterandfan_SRCS 6 cpp)
ENDIF ()

add_executable(indi_dewheaterandfan ${indi_dewheaterandfan_SRCS})
target_link_libraries(indi_dewheaterandfan ${INDI_LIBRARIES})
install(TARGETS indi_dewheaterandfan RUNTIME DESTINATION bin )
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/indi_dewheaterandfan.xml DESTINATION ${INDI_DATA_DIR})

