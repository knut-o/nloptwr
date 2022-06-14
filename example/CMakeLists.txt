
cmake_minimum_required(VERSION 3.0)

SET(CMAKE_VERBOSE_MAKEFILE ON)

# =========================================================================

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


# =========================================================================

include_directories(
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    ${NLOPTWR_INCLUDE_DIR}
)


add_executable(${NLOPTWR_EXPAMPLE1} ${nloptw_example_SRCS})


TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_EXPAMPLE1}
        ${NLOPTWR_LIBRARY}
        ${NLOPTCXX_LIBS}
        OpenMP::OpenMP_CXX
)


ENABLE_TESTING()
ADD_TEST(NAME ${NLOPTWR_EXPAMPLE1} COMMAND ${NLOPTWR_EXPAMPLE1}  )

# =========================================================================

