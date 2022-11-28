#
# Includes
#

cmake_minimum_required(VERSION 3.12)
project(example01 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================================
# =========================================================================

SET(CMAKE_BUILD_TYPE Debug)

# =========================================================================
set(VCPKG_FLAG BOOL OFF)
if (DEFINED ENV{CMAKE_TOOLCHAIN_FILE} )
    SET (CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE} )
    set(VCPKG_FLAG BOOL ON)
    include(${CMAKE_TOOLCHAIN_FILE})
    MESSAGE("VCPKG_FLAG=${VCPKG_FLAG}")
    MESSAGE("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
elseif (DEFINED ENV{VCPKG_ROOT})
    SET(VCPKG_ROOT_VAR $ENV{VCPKG_ROOT} )
    SET(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT_VAR}/scripts/buildsystems/vcpkg.cmake )
    set(VCPKG_FLAG BOOL ON)
    include(${CMAKE_TOOLCHAIN_FILE})
    MESSAGE("VCPKG_FLAG=${VCPKG_FLAG}")
    MESSAGE("VCPKG_ROOT=${VCPKG_ROOT_VAR}")
    MESSAGE("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
else()
    MESSAGE("VCPKG not found..")
endif()

# =========================================================================

message("VCPKG_FLAG=${VCPKG_FLAG}")
message("CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} ")

# define dependencies
if (OPENMP_FOUND)
    find_package(OpenMP REQUIRED)
endif()
if (OPENMP_FOUND)
     message("OpenMP found.")
    SET (HAVE_OPENMP 1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()

find_package(NLopt   CONFIG REQUIRED)

find_package(nloptwr CONFIG REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    ${NLOPTWR_INCLUDE_DIR}
)

add_executable(nloptwr_example1 "nloptwr_example1.cpp")

target_include_directories(nloptwr_example1
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src
)

target_link_libraries(nloptwr_example1
    PRIVATE
        nloptwr
        ## nloptwr::nloptwr
        ## nlopt
        NLopt::nlopt
        OpenMP::OpenMP_CXX
        stdc++
)
