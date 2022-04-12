#=========================== begin_copyright_notice ============================
#
# INTEL CONFIDENTIAL
#
# Copyright (C) 2019-2021 Intel Corporation
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they were
# provided to you ("License"). Unless the License provides otherwise,
# you may not use, modify, copy, publish, distribute, disclose or transmit this
# software or the related documents without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no express or
# implied warranties, other than those that are expressly stated in the License.
#
#============================ end_copyright_notice =============================

set(LIB_NAME zlib)

set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_RELEASEINTERNAL "")

set(ALL_SOURCES
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    infback.c
    inffast.c
    inflate.c
    inftrees.c
    trees.c
    uncompr.c
    zutil.c
)
set_source_files_properties(${ALL_SOURCES} PROPERTIES LANGUAGE C)

set(INCLUDES_DIRECTORIES
    ${BS_DIR_COMMON}/zlib-1.2.11
)

set(COMMON_DEFINES
    $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>:WINAPI_FAMILY=WINAPI_FAMILY_PC_APP>
)
set(DEBUG_DEFINES
    ${COMMON_DEFINES}
    _DEBUG
)
set(RELEASE_DEFINES
    ${COMMON_DEFINES}
    NDEBUG
)
set(RELEASE_INTERNAL_DEFINES
    ${COMMON_DEFINES}
    _RELEASE_INTERNAL
    _INTERNAL
)

set(COMMON_COMPILER_OPTIONS
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/GS,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/MP,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/TC,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/W3,-Wall>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/wd4996,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Gy-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zc:wchar_t-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/guard:cf,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zi,-g>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Gm-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/FI$(KIT_SHARED_IncludePath)/warning.h,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zc:inline,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/fp:precise,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zp8,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/errorReport:prompt,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/GF,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/WX-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zc:forScope,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/GR-,-fno-rtti>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Oy-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Oi,-Os>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/MT,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/openmp-,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/EHsc,-fexceptions>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/nologo,>
)
set(DEBUG_COMPILER_OPTIONS
    ${COMMON_COMPILER_OPTIONS}
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Od,-O0>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Gd,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/MTd,>
)
set(RELEASE_COMPILER_OPTIONS
    ${COMMON_COMPILER_OPTIONS}
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/GL,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/O2,-O3>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Ob2,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/GT,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Gd,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Ot,-O3>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Zc:arch:SSE2,>
    $<IF:$<STREQUAL:${CMAKE_SYSTEM_NAME},Windows>,/Qspectre,>
)
set(RELEASE_INTERNAL_COMPILER_OPTIONS
    ${RELEASE_COMPILER_OPTIONS}
)

add_library(${LIB_NAME} STATIC ${ALL_SOURCES})

target_include_directories(${LIB_NAME} PRIVATE
    ${INCLUDES_DIRECTORIES}
)
target_compile_definitions(${LIB_NAME} PRIVATE
    $<$<CONFIG:Debug>: ${DEBUG_DEFINES}>
    $<$<CONFIG:Release>: ${RELEASE_DEFINES}>
    $<$<CONFIG:ReleaseInternal>: ${RELEASE_INTERNAL_DEFINES}>
)
target_compile_options(${LIB_NAME} PRIVATE
    $<$<CONFIG:Debug>: ${DEBUG_COMPILER_OPTIONS}>
    $<$<CONFIG:Release>: ${RELEASE_COMPILER_OPTIONS}>
    $<$<CONFIG:ReleaseInternal>: ${RELEASE_INTERNAL_COMPILER_OPTIONS}>
)
if(MSVC)
    # bs_set_wdk(${LIB_NAME})
    set_target_properties(${LIB_NAME} PROPERTIES VS_PLATFORM_TOOLSET WindowsUserModeDriver10.0)
    set_target_properties(${LIB_NAME} PROPERTIES VS_GLOBAL_SignMode Off )
endif()
