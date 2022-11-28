#
# Includes
#
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

#
# Dependencies
#
# define dependencies
if (OPENMP_FOUND)
    find_package(OpenMP REQUIRED)
endif()
if (OPENMP_FOUND)
     message("OpenMP found.")
    SET (HAVE_OPENMP 1)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} ")
endif()

find_package(NLopt CONFIG REQUIRED)

# Sources
set(HEADERS_PUB_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwralgorithm.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwra.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwr.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwrsearchalgoparamd.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/optfktnbase.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwrsstrat.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwrparamfactory.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/optfktnclass.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/omphelper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwr/nloptwrsearchalgoparam.h"
    )


set(HEADERS_PUB ${HEADERS_PUB_ROOT})

set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwralgorithm.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwr.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwrparamfactory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwrsearchalgoparam.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwrsearchalgoparamd.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/nloptwrsstrat.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/omphelper.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/optfktnbase.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/optfktnclass.cpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${SOURCES})

include_directories(
    ${NLOPTWR_INCLUDE_DIR}/lib/include
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
)

#
# Target
#
add_library(nloptwr ${SOURCES})

target_include_directories(nloptwr
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        src
)

target_link_libraries(nloptwr
    PRIVATE
#        NLopt::nlopt
        nlopt
        OpenMP::OpenMP_CXX
)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(nloptwr
  EXPORT_FILE_NAME "nloptwr/nloptwr_export.h"
)

#
# nloptwrConfig.cmake File
#
set(NLOPTWR_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/nloptwr")

configure_package_config_file(
    cmake/nloptwrConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/nloptwrConfig.cmake"
    INSTALL_DESTINATION "${NLOPTWR_CMAKE_CONFIG_DIR}"
)

#
# Install
#
install(
    TARGETS nloptwr
    EXPORT nloptwrTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/nloptwr DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nloptwr/nloptwr_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nloptwr)

install(
    EXPORT nloptwrTargets
    FILE nloptwrTargets.cmake
    DESTINATION ${NLOPTWR_CMAKE_CONFIG_DIR}
    NAMESPACE nloptwr::
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/nloptwrConfig.cmake"
    DESTINATION ${NLOPTWR_CMAKE_CONFIG_DIR}
)

export(
    EXPORT nloptwrTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/nloptwrTargets.cmake"
    NAMESPACE nloptwr::
)
