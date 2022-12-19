
#
# Includes
#
include_directories(
    ${VCPKG_INCL}
    ${CMAKE_SOURCE_DIR}/lib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
)

link_directories(
    ${PROJECT_BINARY_DIR}/lib/test
    ${PROJECT_BINARY_DIR}/lib
)

set(HEADERS_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/chainwithweights.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/griewank.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/groundcos.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/groundif.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/groundlin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/odisplay.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/oproblem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/oproblemtest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/rastrigin.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/selalgtest.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/vcompare.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nloptwrtest/xh.h
)

set(TEST_SOURCES
    #    ${CMAKE_CURRENT_SOURCE_DIR}/src/test01.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chainwithweights.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/griewank.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/groundcos.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/groundif.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/groundlin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/odisplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oproblem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/oproblemtest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rastrigin.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vcompare.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/xh.cpp
)

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX source FILES ${TEST_SOURCES})
add_library(nloptwrtest ${TEST_SOURCES})


#
# Target
#
macro(add_example name)
   add_executable(${name} ${CMAKE_CURRENT_SOURCE_DIR}/src/${name}.cpp)
   target_compile_features(${name} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
   target_link_libraries(${name}
        PRIVATE
            nloptwrtest
            nloptwr
            NLopt::nlopt
            OpenMP::OpenMP_CXX
            # stdc++
            )
endmacro()

# =========================================================================

set(NLOPTWR_BIN_01 nloptwr_test1)
set(NLOPTWR_BIN_02 nloptwr_test2)
set(NLOPTWR_BIN_03 nloptwr_test3)
set(NLOPTWR_BIN_04 nloptwr_test4)
set(NLOPTWR_BIN_05 nloptwr_test5)
set(NLOPTWR_BIN_06 nloptwr_test6)
set(NLOPTWR_BIN_a1 selalgtest)

set(NLOPTWR_DESC_01 "parabola")
set(NLOPTWR_DESC_02 "constr_and_rosenb.")
set(NLOPTWR_DESC_03 "griewank")
set(NLOPTWR_DESC_04 "griewank")
set(NLOPTWR_DESC_05 "rastrigin")
set(NLOPTWR_DESC_06 "chain_weights")
set(NLOPTWR_DESC_a1 "sel_algorithms")

# =========================================================================

set(nloptwr_tests
    ${NLOPTWR_BIN_01}
    ${NLOPTWR_BIN_02}
    ${NLOPTWR_BIN_03}
    ${NLOPTWR_BIN_04}
    ${NLOPTWR_BIN_05}
    ${NLOPTWR_BIN_06}
    ${NLOPTWR_BIN_a1}
    )

add_example(${NLOPTWR_BIN_01})
add_example("${NLOPTWR_BIN_02}")
add_example("${NLOPTWR_BIN_03}")
add_example("${NLOPTWR_BIN_04}")
add_example("${NLOPTWR_BIN_05}")
add_example("${NLOPTWR_BIN_06}")
add_example("${NLOPTWR_BIN_a1}")

set(NLOPTWR_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/nloptwrtest")


target_include_directories(nloptwrtest
     PUBLIC
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
     PRIVATE
         src
)

target_link_libraries(nloptwrtest
    PRIVATE
        nloptwr
#        NLopt::nlopt
#        OpenMP::OpenMP_CXX
)


# =========================================================================

if(NLOPTWR_INSTALL_TESTS)

#
# Export headers
#
include(GenerateExportHeader)
generate_export_header(nloptwrtest
  EXPORT_FILE_NAME "nloptwrtest/nloptwrtest_export.h"
)

#
# nloptwrConfig.cmake File
#
set(NLOPTWRTEST_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/nloptwrtest")

configure_package_config_file(
    cmake/nloptwrtestConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/nloptwrtestConfig.cmake"
    INSTALL_DESTINATION "${NLOPTWRTEST_CMAKE_CONFIG_DIR}"
)

install(DIRECTORY include/nloptwrtest DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nloptwrtest/nloptwrtest_export.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nloptwrtest)

install(
    EXPORT nloptwrtestTargets
    FILE nloptwrtestTargets.cmake
    DESTINATION ${NLOPTWRTEST_CMAKE_CONFIG_DIR}
    NAMESPACE nloptwrtest::
)

# Install
install(
    TARGETS ${nloptwr_tests} nloptwrtest
    EXPORT nloptwrtestTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/nloptwrtestConfig.cmake"
    DESTINATION ${NLOPTWRTEST_CMAKE_CONFIG_DIR}
)

export(
    EXPORT nloptwrtestTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/nloptwrtestTargets.cmake"
    NAMESPACE nloptwrtest::

)
endif()

# =========================================================================

ENABLE_TESTING()
#
ADD_TEST(NAME ${NLOPTWR_DESC_01}_L-D      COMMAND ${NLOPTWR_BIN_01} 1 L 20)
# ADD_TEST(NAME ${NLOPTWR_DESC_01}_L-N    COMMAND ${NLOPTWR_BIN_01} 0 L 80)
ADD_TEST(NAME ${NLOPTWR_DESC_01}_G-D    COMMAND ${NLOPTWR_BIN_01} 1 G 40)

ADD_TEST(NAME "${NLOPTWR_DESC_02}_L-D"    COMMAND ${NLOPTWR_BIN_02}  )
ADD_TEST(NAME "${NLOPTWR_DESC_03}_L-D"    COMMAND ${NLOPTWR_BIN_03}  )
ADD_TEST(NAME "${NLOPTWR_DESC_04}_G-D"    COMMAND ${NLOPTWR_BIN_04}  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_G-D   COMMAND ${NLOPTWR_BIN_05}  2  70 G  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_GM-D   COMMAND ${NLOPTWR_BIN_05}  2 70 GM  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_R-N   COMMAND ${NLOPTWR_BIN_05}  2  150 R  )
# ADD_TEST(NAME ${NLOPTWR_BIN_05}_G-D COMMAND ${NLOPTWR_BIN_05} 30 600 G  )

# ADD_TEST(NAME ${NLOPTWR_DESC_06}_0_LM-D   COMMAND ${NLOPTWR_BIN_06}  120 0 1 LM 400 )
ADD_TEST(NAME ${NLOPTWR_DESC_06}_0_L-D   COMMAND ${NLOPTWR_BIN_06}  119 0 1 L 400  )
ADD_TEST(NAME ${NLOPTWR_DESC_06}_1_L-D   COMMAND ${NLOPTWR_BIN_06}  119 1 1 L 400  )
ADD_TEST(NAME ${NLOPTWR_DESC_06}_2_L-D   COMMAND ${NLOPTWR_BIN_06}  119 2 1 L 400  )

ADD_TEST(NAME ${NLOPTWR_DESC_a1}     COMMAND ${NLOPTWR_BIN_a1}  )

# =========================================================================
