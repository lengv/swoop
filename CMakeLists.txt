cmake_minimum_required(VERSION 3.0.0)
project(swoop VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include(CTest)
enable_testing()

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HEADER_FILES ${INCLUDE_DIR}/laser_error.h ${INCLUDE_DIR}/utils.h ${INCLUDE_DIR}/scene.h ${INCLUDE_DIR}/drone.h)

# add_library(swooplib libsrc.cpp ${HEADER_FILES})
# 


set(SOURCES ${SOURCE_DIR}/laser_error.cpp ${SOURCE_DIR}/scene.cpp ${SOURCE_DIR}/drone.cpp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )
add_executable(swoop)
target_sources(swoop PRIVATE main.cpp )
target_include_directories(swoop PRIVATE ${INCLUDE_DIR})
target_sources(swoop PRIVATE ${SOURCES} ${HEADER_FILES})
# install(Targets swoop)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
