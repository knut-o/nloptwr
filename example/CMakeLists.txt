
cmake_minimum_required(VERSION 3.0)

SET(CMAKE_VERBOSE_MAKEFILE ON)

# =========================================================================

<<<<<<< HEAD
=======
set(VCPKG_FLAG BOOL false)
if (DEFINED $ENV{CMAKE_TOOLCHAIN_FILE} )
    SET (CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE} )
    MESSAGE("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
    SET(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake )
    set(VCPKG_FLAG BOOL true)
elseif (DEFINED ENV{VCPKG_ROOT})
    SET(VCPKG_ROOT $ENV{VCPKG_ROOT} )
    SET(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake )
    MESSAGE("VCPKG_ROOT=${VCPKG_ROOT}")
    MESSAGE("CMAKE_TOOLCHAIN_FILE:= ${CMAKE_TOOLCHAIN_FILE}")
    # MESSAGE("VCPKG_INSTALLED_DIR:  ${VCPKG_INSTALLED_DIR}")
    set(VCPKG_FLAG BOOL true)
endif()

MESSAGE("VCPKG found: ${VCPKG_FLAG}")

# =========================================================================

>>>>>>> devel
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

<<<<<<< HEAD
# do not use vcpkg
find_package(NLOPT  REQUIRED)
if (NLOPT_FOUND)
    message("  NLOPT_LIBRARY: ${NLOPT_LIBRARY} ")
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

# =========================================================================

find_package(NLOPTWR  REQUIRED)
if (NLOPTWR_FOUND)
    message("  NLOPTWR_LIBRARY: ${NLOPTWR_LIBRARY} ")
else(NLOPTWR_FOUND)
    message("NLOPTWR not found. ")
endif(NLOPTWR_FOUND)

=======

if (VCPKG_FLAG)
    # use vcpkg

    find_package(NLopt CONFIG REQUIRED)
	message("NLOPT_LIBRARIES (vcpkg):      NLopt::nlopt ")
    
    # target_link_libraries(main PRIVATE NLopt::nlopt)
    SET(NLOPTCXX_LIBS NLopt::nlopt )
    
    MESSAGE("VCPKG_INSTALLED_DIR:  ${VCPKG_INSTALLED_DIR}")
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
>>>>>>> devel

# =========================================================================

include_directories(
<<<<<<< HEAD
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    ${NLOPTWR_INCLUDE_DIR}
=======
    # .
    ### ${VCPKG_INCL}
    #### Nlopt::nlopt
    # HACK
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    # /usr/local/share/vcpkg/installed/x64-linux/include/
    $# {PROJECT_SOURCE_DIR}/nloptwr/include
>>>>>>> devel
)


add_executable(${NLOPTWR_EXPAMPLE1} ${nloptw_example_SRCS})


TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_EXPAMPLE1}
<<<<<<< HEAD
        ${NLOPTWR_LIBRARY}
        ${NLOPTCXX_LIBS}
        OpenMP::OpenMP_CXX
=======
        # nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}

>>>>>>> devel
)


ENABLE_TESTING()
ADD_TEST(NAME ${NLOPTWR_EXPAMPLE1} COMMAND ${NLOPTWR_EXPAMPLE1}  )

# =========================================================================

