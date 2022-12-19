#
# Includes
#

cmake_minimum_required(VERSION 3.12)

project(example01 LANGUAGES CXX)

# =========================================================================
# =========================================================================

# Set a default build type if none was specified

# SET(CMAKE_BUILD_TYPE Release)
set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# =========================================================================
# =========================================================================

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================================================================
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
    set(VCPKG_FLAG BOOL OFF)
    MESSAGE("VCPKG not found..")
endif()

# =========================================================================

message("VCPKG_FLAG=${VCPKG_FLAG}")

# =========================================================================
# =========================================================================

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

# =========================================================================
# =========================================================================

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_PREFIX_PATH}/include
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    ${NLOPTWR_INCLUDE_DIR}
)

set(NLOPWR_EXAMPLE01 nloptwr_example1)

add_executable(${NLOPWR_EXAMPLE01} "nloptwr_example1.cpp")

target_link_libraries(${NLOPWR_EXAMPLE01}
    PRIVATE
        nloptwr::nloptwr
        NLopt::nlopt
        OpenMP::OpenMP_CXX
)

# =========================================================================

ENABLE_TESTING()
#
ADD_TEST(NAME ${NLOPWR_EXAMPLE01}     COMMAND ${NLOPWR_EXAMPLE01})

# =========================================================================

