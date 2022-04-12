cmake_minimum_required(VERSION 3.0.0)
project(CompressionTest VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(CompressionTest zpipe2.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(zlib-1.2.11)

target_include_directories(${PROJECT_NAME} PUBLIC zlib-1.2.11)

target_link_directories(${PROJECT_NAME} PRIVATE zlib-1.2.11)

target_link_libraries(${PROJECT_NAME} zlib)