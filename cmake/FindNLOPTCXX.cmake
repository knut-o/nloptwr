# - Find Libnlopt (c++ library)
# Find the native Libnlopt includes and library
#
#  NLOPTCXX_INCLUDE_DIR - where to find NLOpt.h, etc.
#  NLOPTCXX_LIBRARIES   - List of libraries when using NLOpt.
#  NLOPTCXX_FOUND       - True if NLOpt found.


IF (NLOPTCXX_INCLUDE_DIR)
  # Already in cache, be silent
  ####### SET (libnlopt_FIND_QUIETLY TRUE)
  SET (NLOPTCXX_FIND_QUIETLY TRUE)
ENDIF (NLOPTCXX_INCLUDE_DIR)

FIND_PATH(NLOPTCXX_INCLUDE_DIR nlopt.hpp
    HINTS
        /usr/local/include
        /usr/include
        C:/Programme/nlopt
    )

SET (NLOPTCXX_NAMES "nlopt_cxx")
FIND_LIBRARY (NLOPTCXX_LIBRARY NAMES ${NLOPTCXX_NAMES}
    HINTS
        /usr/local/lib
        /usr/lib
         C:/Programme/nlopt
)

# handle the QUIETLY and REQUIRED arguments and set NLOPTCXX_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS (NLOPTCXX DEFAULT_MSG
  NLOPTCXX_LIBRARY
  NLOPTCXX_INCLUDE_DIR)

IF (NLOPTCXX_FOUND)
  SET (NLOPTCXX_LIBRARIES ${NLOPTCXX_LIBRARY})
ELSE (NLOPTCXX_FOUND)
  SET (NLOPTCXX_LIBRARIES)
ENDIF (NLOPTCXX_FOUND)

MARK_AS_ADVANCED (NLOPTCXX_LIBRARY NLOPTCXX_INCLUDE_DIR)
