
# =========================================================================

set(NLOPWR_INCLUDE_FILES 
    include/nloptwr/omphelper.h
    include/nloptwr/gauss.h
    include/nloptwr/lregx.h
    include/nloptwr/nloptwralgorithm.h
    include/nloptwr/nloptwrsearchalgoparam.h
    include/nloptwr/nloptwrsearchalgoparam2.h
    include/nloptwr/nloptwrsearchalgoparam3.h
    include/nloptwr/nloptwrsstrat.h
    include/nloptwr/optfktnbase.h
    include/nloptwr/optfktnclass.h
    include/nloptwr/nloptwrparamfactory.h
    include/nloptwr/nloptwr.h
    include/nloptwr/nloptwra.h
)

# =========================================================================

# compile all cpp files
set( nloptwr_SRCS 
    src/omphelper.cpp
    src/gauss.cpp
    src/lregx.cpp
    src/nloptwralgorithm.cpp
    src/nloptwrsearchalgoparam.cpp
    src/nloptwrsearchalgoparam2.cpp
    src/nloptwrsearchalgoparam3.cpp
    src/nloptwrsstrat.cpp
    src/optfktnbase.cpp
    src/optfktnclass.cpp
    src/nloptwr.cpp
    src/nloptwrparamfactory.cpp
)

# =========================================================================

# include directories
include_directories(
    # .
    ${VCPKG_INCL}
    #### Nlopt::nlopt
    # HACK
    ${NLOPTCXX_INCLUDE_DIR}
    ${NLOPT_INCLUDE_DIR}
    # /usr/local/share/vcpkg/installed/x64-linux/include/
    ${PROJECT_SOURCE_DIR}/nloptwr/include
    )

# =========================================================================

# Make sure the linker can find the Hello library once it is built.
link_directories( 
	${PROJECT_BINARY_DIR}/nloptwr 
	${PROJECT_BINARY_DIR}/nloptwrtest
	) 

# =========================================================================

add_library( nloptwr STATIC ${nloptwr_SRCS} )

# =========================================================================

set_target_properties(nloptwr PROPERTIES PUBLIC_HEADER "${MY_PUBLIC_HEADERS_OPTWR}")

target_include_directories(nloptwr
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_DIR}>
        )

TARGET_LINK_LIBRARIES( 
        nloptwr
        OpenMP::OpenMP_CXX
        # ${NLOPTCXX_LIBS}
)

target_compile_features(nloptwr PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )


# =========================================================================

set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

install(
  TARGETS   nloptwr 
  RUNTIME   DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install(FILES nloptwr-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${NLOPWR_INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nloptwr COMPONENT devel)

# =========================================================================
