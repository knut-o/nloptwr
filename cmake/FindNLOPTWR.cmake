# - Find Libnlopt (c++ library)
# Find the native Libnlopt includes and library
#
#  NLOPTWR_INSTALL_PREFIX - installation dir
#
#  NLOPTWR_INCLUDE_DIR - where to find NLOpt.h, etc.
#  NLOPTWR_LIBRARIES   - List of libraries when using NLOpt.
#  NLOPTWR_FOUND       - True if NLOpt found.


IF (NLOPTWR_INCLUDE_DIR)
  # Already in cache, be silent
  SET (NLOPTWR_FIND_QUIETLY TRUE)
ENDIF (NLOPTWR_INCLUDE_DIR)

FIND_PATH(NLOPTWR_INCLUDE_DIR nloptwr/nloptwr.h
    HINTS
        ${NLOPTWR_INSTALL_PREFIX}/include
        ENV(${HOME}/install
        /usr/local/include
        /usr/include
        C:/Programme/nloptwr/include
    )

SET (NLOPTWR_NAMES "nloptwr")
FIND_LIBRARY (NLOPTWR_LIBRARY NAMES ${NLOPTWR_NAMES}
    HINTS
        ${NLOPTWR_INSTALL_PREFIX}/lib
        ENV(${HOME}/lib
        /usr/local/lib
        /usr/lib
        C:/Programme/nlopt/lib
)

# handle the QUIETLY and REQUIRED arguments and set NLOPTWR_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (NLOPTWR DEFAULT_MSG
  NLOPTWR_LIBRARY
  NLOPTWR_INCLUDE_DIR)

IF (NLOPTWR_FOUND)
  SET (NLOPTWR_LIBRARIES ${NLOPTWR_LIBRARY})
ELSE (NLOPTWR_FOUND)
  SET (NLOPTWR_LIBRARIES)
ENDIF (NLOPTWR_FOUND)

MARK_AS_ADVANCED (NLOPTWR_LIBRARY NLOPTWR_INCLUDE_DIR)


