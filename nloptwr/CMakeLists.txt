
# =========================================================================

set(NLOPWR_INCLUDE_FILES
    include/nloptwr/omphelper.h
    include/nloptwr/nloptwralgorithm.h
    include/nloptwr/nloptwrsearchalgoparam.h
    include/nloptwr/nloptwrsearchalgoparamd.h
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
    src/nloptwralgorithm.cpp
    src/nloptwrsearchalgoparam.cpp
    src/nloptwrsearchalgoparamd.cpp
    src/nloptwrsstrat.cpp
    src/optfktnbase.cpp
    src/optfktnclass.cpp
    src/nloptwr.cpp
    src/nloptwrparamfactory.cpp
)

# =========================================================================
# TODO
# https://stackoverflow.com/questions/16398937/cmake-and-finding-other-projects-and-their-dependencies

function(my_export_target _target _include_dir)
    file(
        WRITE "${CMAKE_CURRENT_BINARY_DIR}/${_target}Config.cmake"
        "
            include(\"\$\{CMAKE_CURRENT_LIST_DIR\}/${_target}Targets.cmake\")
            set_property(
                TARGET ${ _target }
                APPEND PROPERTY
                    INTERFACE_INCLUDE_DIRECTORIES \"${ _include_dir }\"
            )
        "
    )

    export(TARGETS ${_target} FILE "${CMAKE_CURRENT_BINARY_DIR}/${_target}Targets.cmake")

    # NOTE: The following call can pollute your PC's CMake package registry
    #       See comments/alternatives below
    export(PACKAGE ${_target})
endfunction(my_export_target)

# add_executable(${PROJECT_NAME} ${my_sources_SRC})
# target_link_libraries(${PROJECT_NAME} myCoolLibrary)

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
        # $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

# target_link_libraries(${PROJECT_NAME} myCoolLibrary)
TARGET_LINK_LIBRARIES(
        nloptwr
        OpenMP::OpenMP_CXX
        # ${NLOPTCXX_LIBS}
)

target_compile_features(nloptwr PRIVATE cxx_std_${CMAKE_CXX_STANDARD} )


# =========================================================================

# my_export_target( nloptwr ${CMAKE_CURRENT_SOURCE_DIR}/include )
# https://stackoverflow.com/questions/49857596/cmake-simple-config-file-example
set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

install(
  TARGETS   nloptwr
  RUNTIME   DESTINATION ${CMAKE_INSTALL_BINDIR}
  ARCHIVE   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY   DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES nloptwr-config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${NLOPWR_INCLUDE_FILES} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nloptwr COMPONENT devel)

# =========================================================================
