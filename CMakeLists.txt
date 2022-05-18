cmake_minimum_required(VERSION 3.16)
project(foo_subsonic LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()
message("Generated with config types: ${CMAKE_CONFIGURATION_TYPES}")
message(${CMAKE_BUILD_TYPE})

add_definitions(-DUNICODE -D_UNICODE)

set(CMAKE_CXX_FLAGS_RELEASE "/Ox /Ot /GS- /GL /Oi /Oy /we4715 /DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /Oy /GS- /GL /we4715 /D_DEBUG")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /INCREMENTAL:NO /OPT:REF")

# directories
set(SDK_ROOT_DIR "SDK")
set(FB2K_SDK_DIR "${SDK_ROOT_DIR}/foobar2000/SDK")
set(FB2K_SDK_helpers_DIR "${SDK_ROOT_DIR}/foobar2000/helpers")
set(FB2K_CLIENT_DIR "${SDK_ROOT_DIR}/foobar2000/foobar2000_component_client")
set(PFC_DIR "${SDK_ROOT_DIR}/pfc")
set(PPUI_DIR "${SDK_ROOT_DIR}/libPPUI")
set(TINYXML_DIR "${SDK_ROOT_DIR}/tinyxml")
set(SQLITE_DIR "${SDK_ROOT_DIR}/sqlite3")

##########################################################################
# Foobar 2000 SDK
##########################################################################
file(GLOB_RECURSE FB2K_SDK_SRC "${FB2K_SDK_DIR}/*.cpp")

add_library(foobar2000_sdk STATIC ${FB2K_SDK_SRC})

target_include_directories(foobar2000_sdk PRIVATE "${FB2K_SDK_DIR}/")
target_precompile_headers(foobar2000_sdk PRIVATE "${FB2K_SDK_DIR}/foobar2000.h")

##########################################################################
# Foobar 2000 SDK helpers
##########################################################################
file(GLOB_RECURSE FB2K_SDK_helpers_SRC "${FB2K_SDK_helpers_DIR}/*.cpp")

add_library(foobar2000_helpers STATIC ${FB2K_SDK_helpers_SRC})

target_include_directories(foobar2000_helpers PRIVATE "${FB2K_SDK_helpers_DIR}/" "${SDK_ROOT_DIR}" "SDK/wtl/Include")
target_precompile_headers(foobar2000_helpers PRIVATE "${FB2K_SDK_helpers_DIR}/stdafx.h")

##########################################################################
# Foobar 2000 component client
##########################################################################
file(GLOB_RECURSE FB2K_CLIENT_SRC "${FB2K_CLIENT_DIR}/*.cpp")

add_library(foobar2000_client STATIC ${FB2K_CLIENT_SRC})

target_include_directories(foobar2000_client PRIVATE "${FB2K_SDK_DIR}/")

##########################################################################
# PFC
##########################################################################
file(GLOB_RECURSE PFC_SRC "${PFC_DIR}/*.cpp")

add_library(pfc STATIC ${PFC_SRC})

target_include_directories(pfc PRIVATE "${PFC_DIR}/")
target_precompile_headers(pfc PRIVATE "${PFC_DIR}/pfc-lite.h")

##########################################################################
# libPPUI
##########################################################################
file(GLOB_RECURSE PPUI_SRC "${PPUI_DIR}/*.cpp")

add_library(libPPUI STATIC ${PPUI_SRC})

target_include_directories(libPPUI PRIVATE "${PPUI_DIR}/" "${SDK_ROOT_DIR}" "SDK/wtl/Include")
target_precompile_headers(libPPUI PRIVATE "${PPUI_DIR}/stdafx.h")

##########################################################################
# tinyxml
##########################################################################
set(TINYXML_SRC "${TINYXML_DIR}/tinyxml.cpp" "${TINYXML_DIR}/tinystr.cpp" "${TINYXML_DIR}/tinyxmlparser.cpp" "${TINYXML_DIR}/tinyxmlerror.cpp")

add_library(tinyxml STATIC ${TINYXML_SRC})
target_include_directories(tinyxml PRIVATE "${TINYXML_DIR}/")

##########################################################################
# sqlite3 & SQLiteCpp
##########################################################################
file(GLOB_RECURSE SQLITE3_SRC "${SQLITE_DIR}/*.c")
file(GLOB_RECURSE SQLITECPP_SRC "${SQLITE_DIR}/*.cpp")

add_library(sqlite3 STATIC ${SQLITE3_SRC} ${SQLITECPP_SRC})
target_include_directories(sqlite3 PRIVATE "${SQLITE_DIR}")

##########################################################################
# foo_subsonic
##########################################################################
file(GLOB FOO_SUBSONIC_SRC "./*.cpp")

add_library(foo_subsonic SHARED ${FOO_SUBSONIC_SRC} "foo_subsonic.rc")

target_include_directories(foo_subsonic PRIVATE "." "${SDK_ROOT_DIR}" "${SQLITE_DIR}" "SDK/wtl/Include" "${FB2K_SDK_helpers_DIR}")
target_link_directories(foo_subsonic PRIVATE "${SDK_ROOT_DIR}/foobar2000/shared")
target_link_libraries(foo_subsonic
        foobar2000_sdk
        foobar2000_helpers
        foobar2000_client
        pfc
        libPPUI
        tinyxml
        sqlite3
        shared
        Winhttp
        )
