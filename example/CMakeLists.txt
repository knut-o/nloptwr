
cmake_minimum_required(VERSION 3.0)

SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_BUILD_TYPE Debug)

# =========================================================================
set(VCPKG_FLAG BOOL OFF)
if (DEFINED ENV{CMAKE_TOOLCHAIN_FILE} )
    SET (CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE} )
    MESSAGE("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
    set(VCPKG_FLAG BOOL ON)
elseif (DEFINED ENV{VCPKG_ROOT})
    SET(VCPKG_ROOT_VAR $ENV{VCPKG_ROOT} )
    SET(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT_VAR}/scripts/buildsystems/vcpkg.cmake )
    MESSAGE("VCPKG_ROOT=${VCPKG_ROOT_VAR}")
    MESSAGE("CMAKE_TOOLCHAIN_FILE:= ${CMAKE_TOOLCHAIN_FILE}")
    set(VCPKG_FLAG BOOL ON)
else()
    MESSAGE("VCPKG not found..")
endif()

# =========================================================================

MESSAGE("VCPKG found: ${VCPKG_FLAG}")

project(NLOPTWR_EXAMPLE)

# =========================================================================
# =========================================================================

# compile all cpp files
set( nloptw_example_SRCS
    nloptwr_example1.cpp
)

set(NLOPTWR_EXPAMPLE1 nloptw_example1)


# =========================================================================

# Compile with C++11 standard
SET(CMAKE_CXX_STANDARD 11)

# SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)
# SET(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

# =========================================================================

#link_directories(
#    ${PROJECT_BINARY_DIR}/nloptwr
#    ${PROJECT_BINARY_DIR}/nloptwrtest
#    ${PROJECT_BINARY_DIR}
#)

# =========================================================================

FIND_PACKAGE (OpenMP  REQUIRED)
if (OPENMP_FOUND)
    SET (HAVE_OPENMP 1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()


# =========================================================================

message("#1.) VCPKG_FLAG=${VCPKG_FLAG}")
message("#1.) CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} ")

if (VCPKG_FLAG)
    # use vcpkg
	  message("VCPKG_FLAG=${VCPKG_FLAG}")

    find_package(NLopt CONFIG REQUIRED)
	  message("NLOPT_LIBRARIES (vcpkg):      NLopt::nlopt ")

    # target_link_libraries(main PRIVATE NLopt::nlopt)
    SET(NLOPTCXX_LIBS NLopt::nlopt )

    # MESSAGE("VCPKG_INSTALLED_DIR:  ${VCPKG_INSTALLED_DIR}")
    set(VCPKG_INCL "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")
    MESSAGE("VCPKG_INSTALLED_DIR/VCPKG_TARGET_TRIPLET/include:  ${VCPKG_INCL}")

else()
    # do not use vcpkg

    find_package(NLOPT  REQUIRED)
    if (NLOPT_FOUND)
	    message("NLOPT_LIBRARY (no vcpkg): ${NLOPT_LIBRARY} ")
    else(NLOPT_FOUND)
        message("NLOPT not found. ")
    endif(NLOPT_FOUND)

    find_package(NLOPTCXX REQUIRED)
    if (NLOPTCXX_FOUND)
        message("  NLOPTCXX_LIBRARIES: ${NLOPTCXX_LIBRARIES} ")
    else(NLOPTCXX_FOUND)
        message("NLOPTCXX not found. ")
    endif(NLOPTCXX_FOUND)

    SET(NLOPTCXX_LIBS ${NLOPT_LIBRARY} ${NLOPTCXX_LIBRARY} )

endif()

# =========================================================================

include_directories(
    ${NLOPTWR_INCLUDE_DIR}
    # HACK
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
)


add_executable(${NLOPTWR_EXPAMPLE1} ${nloptw_example_SRCS})


TARGET_LINK_LIBRARIES(
        ${NLOPTWR_EXPAMPLE1}
        ${NLOPTWR_LIBRARY}
        ${NLOPTCXX_LIBS}
        OpenMP::OpenMP_CXX
        nloptwr
)


ENABLE_TESTING()
ADD_TEST(NAME ${NLOPTWR_EXPAMPLE1} COMMAND ${NLOPTWR_EXPAMPLE1}  )

# =========================================================================
