
# =========================================================================

# compile all cpp files
set( nloptwrtest_SRCS
    src/odisplay.cpp
	src/oproblem.cpp
	src/chainwithweights.cpp
	src/oproblemtest.cpp
    src/vcompare.cpp
    src/griewank.cpp
    src/odisplay.cpp
    src/rastrigin.cpp
    src/vcompare.cpp
)

# =========================================================================

include_directories(
    ${VCPKG_INCL}
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    # /usr/local/share/vcpkg/installed/x64-linux/include/
    ${PROJECT_INCLUDE_DIR}/nloptwr/include
    #
    ${PROJECT_SOURCE_DIR}/nloptwr/include
    #
    ${PROJECT_SOURCE_DIR}/nloptwrtest/include
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/nloptwr/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}/nloptwr/>
)

target_include_directories(nloptwr
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}>
        )

# =========================================================================

link_directories(
    ${PROJECT_BINARY_DIR}/nloptwr
    ${PROJECT_BINARY_DIR}/nloptwrtest
    ${PROJECT_BINARY_DIR}
)

# =========================================================================

add_library( nloptwrtest STATIC ${nloptwrtest_SRCS} )

# =========================================================================

set(NLOPTWR_BIN_01 nloptwr_test01)
set(NLOPTWR_BIN_02 nloptwr_test02)
set(NLOPTWR_BIN_03 nloptwr_test03)
set(NLOPTWR_BIN_04 nloptwr_test04)
set(NLOPTWR_BIN_05 nloptwr_test05)
set(NLOPTWR_BIN_06 nloptwr_test06)
set(NLOPTWR_BIN_a1 selalg_test)

set(NLOPTWR_DESC_01 "parabola")
set(NLOPTWR_DESC_02 "constr_and_rosenb.")
set(NLOPTWR_DESC_03 "griewank")
set(NLOPTWR_DESC_04 "griewank")
set(NLOPTWR_DESC_05 "rastrigin")
set(NLOPTWR_DESC_06 "chain_weights")
set(NLOPTWR_DESC_a1 "sel_algorithms")

# =========================================================================

add_executable(${NLOPTWR_BIN_01} src/nloptwr_test1.cpp)
add_executable(${NLOPTWR_BIN_02} src/nloptwr_test2.cpp)
add_executable(${NLOPTWR_BIN_03} src/nloptwr_test3.cpp)
add_executable(${NLOPTWR_BIN_04} src/nloptwr_test4.cpp)
add_executable(${NLOPTWR_BIN_05} src/nloptwr_test5.cpp)
add_executable(${NLOPTWR_BIN_06} src/nloptwr_test6.cpp)
add_executable(${NLOPTWR_BIN_a1} src/selalgtest.cpp)

# =========================================================================

target_compile_features(${NLOPTWR_BIN_01} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_02} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_03} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_04} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_05} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_06} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )
target_compile_features(${NLOPTWR_BIN_a1} PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )

# =========================================================================

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_01}
        PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_02}
        PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_03}
        PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_04}
        PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_05}
        # PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)


TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_06}
        # PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

TARGET_LINK_LIBRARIES( 
        ${NLOPTWR_BIN_a1}
        PRIVATE
        nloptwrtest
        nloptwr
        OpenMP::OpenMP_CXX
        ${NLOPTCXX_LIBS}
)

# =========================================================================

install(
  TARGETS   
        ${NLOPTWR_BIN_01} 
        ${NLOPTWR_BIN_02} 
        ${NLOPTWR_BIN_03} 
        ${NLOPTWR_BIN_04} 
        ${NLOPTWR_BIN_05} 
        ${NLOPTWR_BIN_06} 
        ${NLOPTWR_BIN_a1} 
        nloptwr 
        # nloptwrtest
  RUNTIME   DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  # PUBLIC_HEADER DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR}
)

# =========================================================================

ENABLE_TESTING()
#
ADD_TEST(NAME ${NLOPTWR_DESC_01}_L-G      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_01} 1 L 10)
ADD_TEST(NAME ${NLOPTWR_DESC_01}_L-N    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_01} 0 L 10)
ADD_TEST(NAME ${NLOPTWR_DESC_01}_G-G    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_01} 1 G 20)

ADD_TEST(NAME "${NLOPTWR_DESC_02}_L-G"    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_02}  )
ADD_TEST(NAME "${NLOPTWR_DESC_03}_L-G"    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_03}  )
ADD_TEST(NAME "${NLOPTWR_DESC_04}_G-G"    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_04}  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_G-G   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_05}  2  50 G  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_GM-G   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_05}  2 50 GM  )
ADD_TEST(NAME ${NLOPTWR_DESC_05}_R-N   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_05}  2  50 R  )
# ADD_TEST(NAME ${NLOPTWR_BIN_05}_G-G WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_05} 30 600 G  )

ADD_TEST(NAME ${NLOPTWR_DESC_06}_LM-G   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_06}  13 1  )
ADD_TEST(NAME ${NLOPTWR_DESC_06}_LM-N   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_06}  13 0  )

ADD_TEST(NAME ${NLOPTWR_DESC_a1}     WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/nloptwrtest COMMAND ${NLOPTWR_BIN_a1}  )

# =========================================================================

